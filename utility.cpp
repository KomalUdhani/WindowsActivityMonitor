#include "utility.h"
#include "QDebug"
#include <QDateTime>
#include <QDir>



const QString Utility::FolderNames[FEATURE_COUNT] = {"MouseNotMovedLogs","ScreenShotLogs","NetworkPortsLogs","KeyPressEventLogs","ClipBoardCaptureLogs","URLCaptureLogs"};

QString Utility::getFormattedCurrentTimeStamp()
{
    QDateTime current = QDateTime::currentDateTime();
    QString currentTime = current.toString(ConfigParser::getInstance()->getTimeStampFormat());
    if(currentTime.isEmpty())
         qDebug()<<"Invalid timestamp format\n";
    return currentTime;
}

QString Utility::getCurrentTimeStamp()
{
    QDateTime current = QDateTime::currentDateTime();
    QString currentTime = QString::number(current.toTime_t());
    if(currentTime.isEmpty())
         qDebug()<<"Invalid timestamp format\n";
    return currentTime;
}

QString Utility::getFormatedCurrentDate()
{
    QDateTime current = QDateTime::currentDateTime();
    QString currentTime = current.toString(ConfigParser::getInstance()->getDateFormat());
    if(currentTime.isEmpty())
         qDebug()<<"Invalid timestamp format\n";
    return currentTime;
}

QString Utility::CreateLogFolder(FEATURE feature, QString logFolderPath)
{
    QString FolderName = logFolderPath + "/" + FolderNames[feature];

    if(!QDir(FolderName).exists())
        QDir().mkdir(FolderName);

    if(!QDir(FolderName).exists())
    {
        qDebug()<<"Couldnt create log folder:"<<FolderName<<"\n";
    }

    return FolderName;
}

QFile* Utility::CreateLogFile(QString logFolderPath, QString fileNamePrefix)
{
    QFile* File = new QFile(logFolderPath + "/"+fileNamePrefix+"_"+ getFormatedCurrentDate()+".csv");
    qDebug()<<"Created log file:"<<File->fileName()<<"\n";
    return File;
}
