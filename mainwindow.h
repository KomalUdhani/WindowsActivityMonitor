#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();        

signals:    
    void TakeScreenShotPressed();

//private slots:
//    void setTimeStampValue();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
