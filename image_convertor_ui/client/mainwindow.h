#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtNetwork/QTcpSocket>
#include "connecttoserver.h"
#include "unamepword.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
connectToServer *w1;
unamepword *w2;
QTcpSocket *servaddr;
    QString IpAdd;
    QString uname1;
    QString pword1;
    QString tempFname;
    unsigned char buf[512];
    void clearCharArray();

public slots:
    void getIpaddress();
    void getDetails();
    void getImageName();
    void sendImage();
    void ReceiveImage();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
