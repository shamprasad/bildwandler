#include <sys/types.h>
#include <sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<string.h>
const char *
get_filename_base (const char *filename)
{
  const char *dot = strrchr (filename, '/');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

int
logInProcess (int connfd)
{
  FILE *fp;
  fp = fopen ("userdetails", "r");


  char userName[20] = "bild", passWord[20] = "wandler", buf[2] = "0";
  char ruserName[20] = { '\0' }, rpassWord[20] =
  {
  '\0'};
  char sendname[6] = "SEND";
  int size;
  if ((size = read (connfd, ruserName, sizeof (ruserName))) <= 0)
    {
      printf ("Closing Connection %d\n", connfd);
      close (connfd);
      exit (0);
    }
  write (connfd, sendname, 5);
  size = read (connfd, rpassWord, sizeof (rpassWord));
  while (fscanf (fp, "%s %s", userName, passWord) != EOF)
    {
      if (!strcmp (userName, ruserName) && !strcmp (passWord, rpassWord))
	{
	  buf[0] = '1';
	  printf ("%s Logged in from %d\n", userName, connfd);
	}
    }
  if (buf[0] == '0')
    printf ("Invalid user attempting from %d \n", connfd);
  buf[1] = '\0';
  write (connfd, buf, 2);
  fclose (fp);
  return (buf[0] == '1');
}

int
receiveImage (int connfd, char fname[], unsigned int *fileSize,
	      char Extension[])
{
  char sendname[6] = "SEND";
  int readdata, fd,grayscale = 0;
  char command[75];
  unsigned int readsofar = 0;
  unsigned char buf[512];
  if ((readdata = read (connfd, fname, 100)) <= 0)
    {
      printf ("Closing Connection %d\n", connfd);
      close (connfd);
      exit (0);
    }
  fname[readdata - 1] = '\0';
  write (connfd, sendname, 5);
  read (connfd, buf, 5);
  *fileSize =
    ((buf[0] | 0x00000000) << 24) | ((buf[1] | 0x00000000) << 16) |
    ((buf[2] | 0x00000000) << 8) | ((buf[3] | 0x00000000));

  write (connfd, sendname, 5);
  read (connfd, Extension, 7);
  write (connfd, sendname, 5);

  if(*(Extension + strlen(Extension)-1) == '1')
   grayscale = 1;
  *(Extension + strlen(Extension)-1) = '\0';
  printf ("Getting file %s of size %u bytes to be converted to %s format \n",
	  fname, *fileSize, Extension);
  fname = get_filename_base (fname);
  printf ("%s\n", fname);
  fd = open (fname, O_CREAT | O_WRONLY, 0777);

  while ((readdata = read (connfd, buf, 512)) > 0)
    {
      readsofar += readdata;
      write (fd, buf, readdata);
      write (connfd, sendname, 5);
      if (readsofar >= *fileSize)
	break;

    }
  close (fd);
  printf ("Received image \n");
  sprintf (command, "./bildWandler %s %u.%s %d", fname, *fileSize, Extension,grayscale);
  system (command);
  sleep (1);
  write (connfd, sendname, 5);	// Done converting ... message the client
  printf ("File converted successfully to %s format\n", Extension);
  remove (fname);
}

int
sendImage (int connfd, unsigned int *fileSize, char Extension[])
{
  int fd, cont, reply;
  unsigned int fileSize1;
  char fname[50];
  char sendname[6] = "SEND";
  unsigned char buf[512];
  sprintf (fname, "%u.%s", *fileSize, Extension);
  if ((reply = read (connfd, buf, sizeof (buf))) <= 0)
    {
      printf ("Closing Connection %d\n", connfd);
      close (connfd);
      remove (fname);
      exit (0);
    }
  write (connfd, fname, 50);
  reply = read (connfd, buf, sizeof (buf));
  if (fd = open (fname, O_RDONLY))
    printf ("converted file opened for sending\n");
  fileSize1 = lseek (fd, 0, SEEK_END);
  lseek (fd, 0, SEEK_SET);
  buf[0] = (fileSize1 & 0xff000000) >> 24;
  buf[1] = (fileSize1 & 0x00ff0000) >> 16;
  buf[2] = (fileSize1 & 0x0000ff00) >> 8;
  buf[3] = (fileSize1 & 0x000000ff);
  buf[4] = '\0';
  write (connfd, buf, 5);
  reply = read (connfd, buf, sizeof (buf));

  while ((cont = read (fd, buf, 512)) > 0)
    {
      write (connfd, buf, cont);
      reply = read (connfd, buf, sizeof (buf));

    }
  printf ("Server transfer complete\n");
  close (fd);
  remove (fname);
}

int
main (int argc, char *argv[])
{
  pid_t pid;
  int listenfd, connfd, clilen, messageLen;
  char fname[100];
  unsigned int fileSize;
  char Extension[6];
  struct sockaddr_in servaddr, cliaddr;
  if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) > 0)
    printf ("Socket is created\n");
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons (1500);

  if ((bind (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr))) ==
      0)
    printf ("Socket is binding\n");
  listen (listenfd, 3);

  for (;;)
    {
      clilen = sizeof (cliaddr);
      if ((connfd =
	   accept (listenfd, (struct sockaddr *) &cliaddr, &clilen)) > 0)
	printf ("Connection from client %s is accepted\n",
		inet_ntoa (cliaddr.sin_addr));

      if ((pid = fork ()) == 0)
	{

	  close (listenfd);	/* child closes listening socket */

	  while (!logInProcess (connfd));

	  while (1)
	    {
	      receiveImage (connfd, fname, &fileSize, Extension);
	      sendImage (connfd, &fileSize, Extension);

	    }
	  printf ("Closing Connection %d\n", connfd);
	  close (connfd);
	  exit (0);		/* child terminates */
	}
      close (connfd);		/*parent closes connected socket */
    }
}
