#include "clipboardcapture.h"
#include "configparser.h"
#include "utility.h"
#include <QApplication>
#include <QDebug>

ClipboardCapture::ClipboardCapture(QObject* /*parent*/)
    : m_ConfigParser(ConfigParser::getInstance())
    , m_Timer(new QTimer(this))
    , m_File(NULL)
{
    m_CaptureClipboardDataEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_CLIPBOARD_CAPTURE);

    if(m_CaptureClipboardDataEnabled)
    {
        qDebug()<<"m_CaptureClipboardDataEnabled\n";
        m_Clipboard = QApplication::clipboard();
        m_ClipboardString = m_Clipboard->text();

        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_CLIPBOARD_CAPTURE);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_LogDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_CLIPBOARD_CAPTURE);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_CLIPBOARD_CAPTURE,m_LogDirectoryPath);

        m_FileNamePrefix = m_ConfigParser->getFeatureLogFilePrefix(FEATURE_CLIPBOARD_CAPTURE);

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

        connect(m_Clipboard, SIGNAL(dataChanged()), this, SLOT(AddClipboardData()));
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(CaptureClipboardData()));
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

ClipboardCapture::~ClipboardCapture()
{
    if(m_File->isOpen())
        m_File->close();
    DELETE_OBJECT(m_TextStream)
    DELETE_OBJECT(m_File);
    DELETE_OBJECT(m_Timer);
}


void ClipboardCapture::AddClipboardData()
{
    if(m_CaptureClipboardDataEnabled)
    {
    //qDebug()<<"clipboard data changed\n";
    m_ClipboardString.append("\n");
    m_ClipboardString.append(m_Clipboard->text());
    }
}

void ClipboardCapture::CaptureClipboardData()
{
    if(m_CaptureClipboardDataEnabled)
    {
        //qDebug()<<"Clipboard Data:"<<m_ClipboardString;
        //write timestamp to log file
        if(m_TextStream != NULL)
        {
            (*m_TextStream)  << Utility::getFormattedCurrentTimeStamp() <<","<<m_ClipboardString<<"\n";
            m_TextStream->flush();
        }
        m_ClipboardString.clear();
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}
