#ifndef TAKESCREENSHOT_H
#define TAKESCREENSHOT_H

#include <QWidget>
#include "configparser.h"

class TakeScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit TakeScreenShot(QWidget *parent = 0);
    ~TakeScreenShot();
    QString getCurrentFileName();   

public slots:
    void StartCapturingScreenShots();
    void TakeCurrentScreenShot();


private:
    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_ScreenShotCaptureEnabled;
    QString m_ImageDirectoryPath;
    QString m_LogFolder;
    int m_CaptureDuration;
    QString m_DirectoryPath;
    QString m_ImageFormat;
    IMAGE_TYPE m_ImageFormatEnum;
    int m_ImageQualityRatio;
};

#endif // TAKESCREENSHOT_H
