#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QByteArray>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Receive()
{
    char fname[50],sendname[6] = "SEND";
    unsigned int fileSize = 0,readsofar = 0;
    int read;
    QString convertedPath;

    servaddr->write(sendname,strlen(sendname)+1);
    if(servaddr->waitForReadyRead(2000))
    {
        servaddr->read(fname,512);
        qDebug()<<"SEND";
    }
    //QFile file(fname);
    convertedPath = "D:\\convertedfiles";
    convertedPath.append(fname);
    QFile file(convertedPath);

    if(file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"file opened for writing";
    }
    servaddr->write(sendname,strlen(sendname)+1);
    if(servaddr->waitForReadyRead(2000))
    {
        servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
    }
    fileSize = ((buf[0] | 0x00000000) << 24) | ((buf[1] | 0x00000000) << 16) | ((buf[2] | 0x00000000) << 8) | ((buf[3] | 0x00000000));
    qDebug()<<fileSize;
    servaddr->write(sendname,strlen(sendname)+1);
    clearCharArray();
    while(servaddr->waitForReadyRead(2000))
    {
        read = servaddr->read((char *)buf,512);
        file.write((char *)buf,read);
        //clearCharArray();
        readsofar += read;
        servaddr->write(sendname,strlen(sendname));
        if(readsofar >= fileSize)
            break;

    }
file.close();
ui->label_8->setDisabled(true);
ui->GetImage->setDisabled(true);
ui->dispLabel2->setText(convertedPath);
}
void MainWindow::getImageName()
{
    tempFname = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "D:\\", tr("Image Files (*.*)"));
    if(tempFname.isEmpty() == false)
    {
        ui->browse->setText("Done");

    }
    ui->dispPath->setText(tempFname);
}

void MainWindow::Send()
{
    char *fname = "/home/ubuntu/Downloads/ppmset/stop05.ppm",*Extension;
    QString extens;
    QByteArray ba;
    if(tempFname.isNull() == true)
   {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Critical);
       msgBox.setText("Image path not specified");
       msgBox.exec();
       return;

   }
   ba = tempFname.toLocal8Bit();
   fname = ba.data();
    QFile file(fname);
    if(file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"file opened\n";
    }
    extens =  ui->comboBox->currentText();
    ba = extens.toLocal8Bit();
    Extension = ba.data();

    if(servaddr->waitForConnected(2000))
    {
        int read = 0;
        unsigned int fileSize = 0;

        servaddr->write(fname,strlen(fname)+1);
        if(servaddr->waitForReadyRead(2000))
        {
            servaddr->read((char *)buf,512);
            qDebug()<<"SEND";
        }
        fileSize = (unsigned int)file.size();
        qDebug()<<fileSize;
        buf[0] = (fileSize & 0xff000000) >> 24;
        buf[1] = (fileSize & 0x00ff0000) >> 16;
        buf[2] = (fileSize & 0x0000ff00) >> 8;
        buf[3] = (fileSize & 0x000000ff);
        buf[4] = '\0';
        servaddr->write((char *)buf,5);

        if(servaddr->waitForReadyRead(2000))
        {
            servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
        }
        servaddr->write(Extension,strlen(Extension)+1);
        qDebug()<<Extension;

        if(servaddr->waitForReadyRead(2000))
        {
            servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
        }

        while((read = file.read((char*)buf, 512)) > 0)
        {
            servaddr->write((char*)buf,read);

            if(servaddr->waitForReadyRead(2000))
                servaddr->read((char *)buf,512);
        }
        file.close();
        if(servaddr->waitForReadyRead(2000))
        {
            servaddr->read((char *)buf,512);
            ui->GetImage->setEnabled(true);
            ui->label_8->setEnabled(true);
            ui->textBrowser->append("Succesfully converted");
        qDebug()<<"SEND";
        }

    }

}
    void MainWindow::Connectip()
    {
        QString ipaddress;
        servaddr = new QTcpSocket(this);
        ipaddress = ui->lineEdit->text();
        if(ipaddress.isEmpty())
       {
           QMessageBox msgBox;
           msgBox.setIcon(QMessageBox::Critical);
           msgBox.setText("Ip Address not specified");
           msgBox.exec();
           return;

       }
        connect(servaddr,SIGNAL(connected()),this,SLOT(dosomething()));
        servaddr->connectToHost(ipaddress,1500,QIODevice::ReadWrite);
        if(!servaddr->waitForConnected(3000))
        {
            ui->textBrowser->append("Server not found");
        }

    }

void MainWindow::dosomething()
{

    ui->textBrowser->setText("Connected Succesfully");
    ui->label_4->setDisabled(true);
    ui->label_2->setDisabled(true);
    ui->Connect->setDisabled(true);
    ui->lineEdit->setDisabled(true);
    ui->label_6->setEnabled(true);
    ui->label_10->setEnabled(true);
    ui->label_11->setEnabled(true);
    ui->userName->setEnabled(true);
    ui->passWord->setEnabled(true);
    ui->logIn->setEnabled(true);

}
void MainWindow::clearCharArray()
{
    for(int i = 0; i < 512 ;i++)
        buf[i] = '\0';
}
void MainWindow::logInServer()
{
    char *username,*password;
    QString uname,pword;
    QByteArray ba;

    uname = ui->userName->text();
    pword = ui->passWord->text();
    if(uname.isEmpty())
   {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Critical);
       msgBox.setText("Username not specified");
       msgBox.exec();
       return;

   }
    if(pword.isEmpty())
   {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Critical);
       msgBox.setText("Password not specified");
       msgBox.exec();
       return;

   }
    ba = uname.toLocal8Bit();
    username = ba.data();

    ba = pword.toLocal8Bit();
    password = ba.data();

    servaddr->write(username,strlen(username)+1);
    if(servaddr->waitForReadyRead(2000))
    {
        servaddr->read((char *)buf,512);
    qDebug()<<"SEND";
    }
    servaddr->write(password,strlen(password)+1);
    if(servaddr->waitForReadyRead(2000))
    {
        servaddr->read((char *)buf,512);
    qDebug()<<"SEND";
    }
    if(buf[0] == '1'){
    ui->label_7->setEnabled(true);
    ui->label_9->setEnabled(true);
    ui->label->setEnabled(true);
    ui->browse->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->send->setEnabled(true);
    ui->label_6->setDisabled(true);
    ui->label_10->setDisabled(true);
    ui->label_11->setDisabled(true);
    ui->userName->setDisabled(true);
    ui->passWord->setDisabled(true);
    ui->logIn->setDisabled(true);
    ui->textBrowser->append("Logged In Successfully");
    }
    else
        ui->textBrowser->append("Invalid username/password");
}
