#include "takescreenshot.h"
#include "utility.h"
#include <QDir>
#include <QScreen>
#include <QWindow>
#include <QGuiApplication>
#include <qwidget.h>
#include <QTimer>
#include <QDebug>

TakeScreenShot::TakeScreenShot(QWidget *parent) :
    QWidget(parent)
   , m_ConfigParser(ConfigParser::getInstance())
   , m_Timer(new QTimer(this))
   , m_ImageFormat("png")
   , m_ImageQualityRatio(-1)
{
    m_ScreenShotCaptureEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_SCREENSHOT_CAPTURE);

    if(m_ScreenShotCaptureEnabled)
    {
        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_SCREENSHOT_CAPTURE);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_ImageDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_SCREENSHOT_CAPTURE);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_SCREENSHOT_CAPTURE,m_ImageDirectoryPath);

        m_ImageFormatEnum = m_ConfigParser->getScreenshotImageType();
        m_ImageFormat = m_ImageFormatEnum == IMAGE_TYPE_JPEG ? "jpeg" : "png";

        m_ImageQualityRatio = m_ConfigParser->getScreenshotImageQualityRatio();

        connect(m_Timer, SIGNAL(timeout()), this, SLOT(TakeCurrentScreenShot()));
    }
}

TakeScreenShot::~TakeScreenShot()
{
       DELETE_OBJECT(m_Timer);
}

void TakeScreenShot::StartCapturingScreenShots()
{
    if(m_ScreenShotCaptureEnabled)
    {
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

void TakeScreenShot::TakeCurrentScreenShot()
{
    //capture screenshot
    QScreen *screen = QGuiApplication::primaryScreen();
    if(const QWindow *window = windowHandle())
    {
        screen = window->screen();
    }
    if(!screen)
        return;

    QPixmap originalPixmap;
    originalPixmap = screen->grabWindow(0);
    originalPixmap.save(getCurrentFileName(), m_ImageFormat.toStdString().c_str(), m_ImageQualityRatio);
    m_Timer->start(m_CaptureDuration*60*1000);
}

QString TakeScreenShot::getCurrentFileName()
{
    //take current timestamp as filename
    QString CurrentTimeStamp = Utility::getCurrentTimeStamp();
    QString FilePath = m_LogFolder + "/" + CurrentTimeStamp + "." + m_ImageFormat;
    qDebug()<<"image path:"<<FilePath;
    return FilePath;
}
