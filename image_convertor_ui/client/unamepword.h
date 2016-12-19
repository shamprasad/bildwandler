#ifndef UNAMEPWORD_H
#define UNAMEPWORD_H

#include <QMainWindow>

namespace Ui {
class unamepword;
}

class unamepword : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit unamepword(QWidget *parent = 0);
    ~unamepword();
    QString uname;
    QString pword;
public slots:
    void GetAuthDetails();
signals:
    void copyAuthDetails();

private:
    Ui::unamepword *ui;
};

#endif // UNAMEPWORD_H
