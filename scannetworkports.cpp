#include "scannetworkports.h"
#include "configparser.h"
#include "utility.h"

ScanNetworkPorts::ScanNetworkPorts(QObject* /*parent*/)
    : m_ConfigParser(ConfigParser::getInstance())
    , m_Timer(new QTimer(this))
    , m_File(NULL)
{
    m_CaptureNetworkPortsEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_SCAN_OPEN_NETWORK_PORTS);

    if(m_CaptureNetworkPortsEnabled)
    {
        m_ExecuteNetstatProcess = new QProcess();
        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_SCAN_OPEN_NETWORK_PORTS);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_LogDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_SCAN_OPEN_NETWORK_PORTS);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_SCAN_OPEN_NETWORK_PORTS,m_LogDirectoryPath);

        m_FileNamePrefix = m_ConfigParser->getFeatureLogFilePrefix(FEATURE_SCAN_OPEN_NETWORK_PORTS);

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

        connect(m_Timer, SIGNAL(timeout()), this, SLOT(CaptureOpenNetworkPorts()));
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

ScanNetworkPorts::~ScanNetworkPorts()
{
    if(m_File->isOpen())
        m_File->close();
    DELETE_OBJECT(m_TextStream)
    DELETE_OBJECT(m_File);
    DELETE_OBJECT(m_Timer);
}

void ScanNetworkPorts::CaptureOpenNetworkPorts()
{
    if(m_CaptureNetworkPortsEnabled)
    {
        //capture open network ports & write to file
        m_ExecuteNetstatProcess->start("netstat", QStringList()<< "-an");
        m_ExecuteNetstatProcess->waitForFinished();
        //qDebug()<<"netstat o/p:\n"<<m_ExecuteNetstatProcess->readAll();

        //write timestamp to log file
        if(m_TextStream != NULL)
        {
            (*m_TextStream)  << Utility::getFormattedCurrentTimeStamp() <<","<<m_ExecuteNetstatProcess->readAll()<<"\n";
            m_TextStream->flush();
        }

        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

