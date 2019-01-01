#include "captureurls.h"
#include "configparser.h"
#include "utility.h"
#include <QDebug>

CaptureURLs::CaptureURLs(QObject* /*parent*/)
    : m_ConfigParser(ConfigParser::getInstance())
    , m_Timer(new QTimer(this))
    , m_File(NULL)
    , m_UrlString("")
{
    m_CaptureUrlDataEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_URL_CAPTURE);

    if(m_CaptureUrlDataEnabled)
    {
        qDebug()<<"m_CaptureUrlDataEnabled";

        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_URL_CAPTURE);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_LogDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_URL_CAPTURE);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_URL_CAPTURE,m_LogDirectoryPath);

        m_FileNamePrefix = m_ConfigParser->getFeatureLogFilePrefix(FEATURE_URL_CAPTURE);

        //create log file if not exist
        if(m_File == NULL)
        m_File = Utility::CreateLogFile(m_LogFolder, m_FileNamePrefix);
        if(!(m_File == NULL))
        {
            if(m_File->open(QFile::Append/* | QFile::Truncate*/))
            {
                m_TextStream = new QTextStream(m_File);
            }
        }

        //connect(m_Clipboard, SIGNAL(dataChanged()), this, SLOT(AddClipboardData()));
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(WriteURLData()));
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

CaptureURLs::~CaptureURLs()
{
    if(m_File->isOpen())
        m_File->close();
    DELETE_OBJECT(m_TextStream)
    DELETE_OBJECT(m_File);
    DELETE_OBJECT(m_Timer);
}

bool CaptureURLs::isFeatureEnabled()
{
    bool isFeatureEnable;
    isFeatureEnable = (m_CaptureUrlDataEnabled ? true : false);
    qDebug()<<"isFeatureEnabled: "<<isFeatureEnable<<"\n";
    return isFeatureEnable;
}

void CaptureURLs::OnCaptureURLData(const char* url)
{
    if(m_CaptureUrlDataEnabled)
    {
        m_UrlString.append(url);
    }

}

void CaptureURLs::WriteURLData()
{
    if(m_CaptureUrlDataEnabled)
    {
        //qDebug()<<"Clipboard Data:"<<m_ClipboardString;
        //write timestamp to log file
        if(m_TextStream != NULL)
        {
            (*m_TextStream)  << Utility::getFormattedCurrentTimeStamp() <<","<<m_UrlString<<"\n";
            m_TextStream->flush();
        }
        m_UrlString.clear();
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}
