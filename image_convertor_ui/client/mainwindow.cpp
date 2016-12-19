#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QMessageBox>
#include<QByteArray>
#include<QPixmap>
#include<QImage>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(500,200,726,517);
    w1 = new connectToServer(this);
    connect(w1,SIGNAL(copyIp()),this,SLOT(getIpaddress()));
    w2 = new unamepword(this);
    connect(w2,SIGNAL(copyAuthDetails()),this,SLOT(getDetails()));
    ui->progressBar->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getIpaddress()
{
IpAdd = w1->IpAddress;
qDebug()<<IpAdd;

servaddr = new QTcpSocket(this);

servaddr->connectToHost(IpAdd,1500,QIODevice::ReadWrite);
if(!servaddr->waitForConnected(3000))
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Server not found");
    msgBox.exec();
    return;
}

w2->show();
w1->close();


}
void MainWindow::getDetails()
{
    QByteArray ba;
    char *username,*password;

    uname1 = w2->uname;
    pword1 = w2->pword;
    qDebug()<<uname1<<pword1;
    ba = uname1.toLocal8Bit();
    username = ba.data();

    ba = pword1.toLocal8Bit();
    password = ba.data();
    servaddr->write(username,strlen(username)+1);
    if(servaddr->waitForReadyRead(-1))
    {
        servaddr->read((char *)buf,512);
    qDebug()<<"SEND";
    }
    servaddr->write(password,strlen(password)+1);
    if(servaddr->waitForReadyRead(-1))
    {
        servaddr->read((char *)buf,512);
    qDebug()<<"SEND";
    }
    if(buf[0] == '1'){
        w2->close();
        ui->label->setEnabled(true);
        ui->label_9->setEnabled(true);
        ui->browse->setEnabled(true);
        ui->comboBox->setEnabled(true);
        ui->send->setEnabled(true);
        ui->checkBox->setEnabled(true);


    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Invalid Username/Password");
        msgBox.exec();
        return;
    }


}
void MainWindow::getImageName()
{
    tempFname = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/ubuntu", tr("Image Files (*.*)"));
    if(tempFname.isEmpty() == false)
    {
        ui->browse->setText("Done");

    }
    ui->dispPath->setText(tempFname);
    ui->dispLabel2->clear();
    ui->imgDisp2->clear();
    ui->label_2->clear();
    QImage im1(tempFname);
    QPixmap pm = QPixmap::fromImage(im1,Qt::AutoColor);
    ui->imgDisp->setPixmap(pm.scaled(311,247,Qt::IgnoreAspectRatio,Qt::FastTransformation));
}
void MainWindow::sendImage()
{
    char *fname = "/home/ubuntu/Downloads/ppmset/stop05.ppm",*Extension;
    QString extens;
    QByteArray ba;
    int length;
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
    length = strlen(Extension);
    if(ui->checkBox->isChecked())
        *(Extension+length) = '1';
    else
        *(Extension+length) = '0';
    *(Extension+length+1) = '\0';

    if(servaddr->waitForConnected(-1))
    {
        int read = 0;
        unsigned int fileSize = 0,sentsoFar = 0;

        servaddr->write(fname,strlen(fname)+1);
        if(servaddr->waitForReadyRead(-1))
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

        if(servaddr->waitForReadyRead(-1))
        {
            servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
        }
        servaddr->write(Extension,strlen(Extension)+1);
        qDebug()<<Extension;

        if(servaddr->waitForReadyRead(-1))
        {
            servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
        }
        ui->Sending->setText("Sending image");
        ui->progressBar->show();
        ui->progressBar->setValue(0);

        while((read = file.read((char*)buf, 512)) > 0)
        {
            servaddr->write((char*)buf,read);
            sentsoFar += read;
            ui->progressBar->setValue((sentsoFar*100)/fileSize);

            if(servaddr->waitForReadyRead(-1))
                servaddr->read((char *)buf,512);
        }
        file.close();
        ui->Sending->clear();
        ui->Sending->setText("Waiting for server");
        ui->progressBar->hide();

        if(servaddr->waitForReadyRead(-1))
        {
            servaddr->read((char *)buf,512);
            ui->label_9->setDisabled(true);
            ui->label->setDisabled(true);
            ui->browse->setDisabled(true);
            ui->comboBox->setDisabled(true);
            ui->send->setDisabled(true);
            ui->dispPath->setDisabled(true);
            ui->checkBox->setDisabled(true);
            ui->GetImage->setEnabled(true);
            ui->dispLabel2->clear();
            ui->imgDisp2->clear();
            ui->label_2->setDisabled(true);
            ui->Sending->clear();
        qDebug()<<"SEND";
        }

    }
}
void MainWindow::ReceiveImage()
{
    char fname[50],sendname[6] = "SEND";
    unsigned int fileSize = 0,readsofar = 0;
    int read;
    QString convertedPath;

    servaddr->write(sendname,strlen(sendname)+1);
    if(servaddr->waitForReadyRead(-1))
    {
        servaddr->read(fname,512);
        qDebug()<<"SEND";
    }
    //QFile file(fname);
    convertedPath = QFileDialog::getSaveFileName(this,
         tr("Save Image"), "/home/ubuntu", tr("Image Files (*.*)"));;
    //convertedPath.append(fname);
    QFile file(convertedPath);

    if(file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"file opened for writing";
    }
    servaddr->write(sendname,strlen(sendname)+1);
    if(servaddr->waitForReadyRead(-1))
    {
        servaddr->read((char *)buf,512);
        qDebug()<<"SEND";
    }
    fileSize = ((buf[0] | 0x00000000) << 24) | ((buf[1] | 0x00000000) << 16) | ((buf[2] | 0x00000000) << 8) | ((buf[3] | 0x00000000));
    qDebug()<<fileSize;
    servaddr->write(sendname,strlen(sendname)+1);
    clearCharArray();
    ui->Sending->setText("Receiving image");
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    while(servaddr->waitForReadyRead(-1))
    {
        read = servaddr->read((char *)buf,512);
        file.write((char *)buf,read);
        //clearCharArray();
        readsofar += read;
        ui->progressBar->setValue((readsofar*100)/fileSize);
        servaddr->write(sendname,strlen(sendname));
        if(readsofar >= fileSize)
            break;

    }
file.close();

ui->dispLabel2->setText(convertedPath);
ui->GetImage->setDisabled(true);
ui->label_9->setEnabled(true);
ui->label->setEnabled(true);
ui->browse->setEnabled(true);
ui->comboBox->setEnabled(true);
ui->send->setEnabled(true);
ui->dispPath->setEnabled(true);
ui->label_2->setEnabled(true);
ui->checkBox->setEnabled(true);
ui->Sending->clear();
ui->progressBar->hide();
QImage im1(convertedPath);
QPixmap pm = QPixmap::fromImage(im1,Qt::AutoColor);
ui->imgDisp2->setPixmap(pm.scaled(311,247,Qt::IgnoreAspectRatio,Qt::FastTransformation));
}
void MainWindow::clearCharArray()
{
    for(int i = 0; i < 512 ;i++)
        buf[i] = '\0';
}
