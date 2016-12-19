#include "connecttoserver.h"
#include "ui_connecttoserver.h"
#include<QMessageBox>
connectToServer::connectToServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::connectToServer)
{
    ui->setupUi(this);
    this->setGeometry(600,300,260,175);
}

connectToServer::~connectToServer()
{
    delete ui;
}
void connectToServer::GetIp()
{
    IpAddress = ui->serverIp->text();
    if(IpAddress.isEmpty())
    {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Critical);
       msgBox.setText("Ip Address not specified");
       msgBox.exec();
       return;

    }

    emit copyIp();
}
