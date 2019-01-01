#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include <QDebug>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->TakeScreenShot, SIGNAL(pressed()), this, SIGNAL(TakeScreenShotPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::setTimeStampValue()
//{
//    ui->TimeStampLabel->setText(Utility::getCurrentTimeStamp());
//}
