#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QDebug>
#include<QMainWindow>
#include <QKeyEvent>

class MyWindow: public QMainWindow
{
  Q_OBJECT

  public:
    MyWindow()
    {}
    ~ MyWindow(){}
   public :
    void keyPressEvent(QKeyEvent* e)
    {
//          QMessageBox* box = new QMessageBox();
//          box->setWindowTitle(QString("Hello"));
//          box->setText(QString("You Pressed: ")+ e->text());
//          box->show();
          qDebug()<<"You Pressed:"<< e->text() ;
    }
};


#endif // MYWINDOW_H
