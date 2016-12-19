#include "unamepword.h"
#include "ui_unamepword.h"
#include<QMessageBox>
unamepword::unamepword(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::unamepword)
{
    ui->setupUi(this);
    this->setGeometry(600,300,339,238);
}

unamepword::~unamepword()
{
    delete ui;
}
void unamepword::GetAuthDetails()
{
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
    emit copyAuthDetails();
}
