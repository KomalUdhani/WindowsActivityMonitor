#ifndef UTILITY_H
#define UTILITY_H

#define DELETE_OBJECT( ptr )\
if( ptr != NULL)    \
{                  \
    delete ptr;    \
    ptr = NULL;    \
}


#include "configparser.h"
#include <QString>
#include <QFile>

class Utility
{
public:
    static QString getCurrentTimeStamp();
    static QString getFormattedCurrentTimeStamp();
    static QString getFormatedCurrentDate();
    static QString CreateLogFolder(FEATURE feature, QString logFolderPath);
    static QFile* CreateLogFile(QString logFolderPath, QString fileNamePrefix);

    static const QString FolderNames[FEATURE_COUNT];
};

#endif // UTILITY_H
