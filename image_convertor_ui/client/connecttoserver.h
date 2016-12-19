#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QMainWindow>

namespace Ui {
class connectToServer;
}

class connectToServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit connectToServer(QWidget *parent = 0);
    ~connectToServer();
    QString IpAddress;
public slots:
    void GetIp();
signals:
    void copyIp();
    
private:
    Ui::connectToServer *ui;
};

#endif // CONNECTTOSERVER_H
