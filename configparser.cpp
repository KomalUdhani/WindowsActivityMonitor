#include "configparser.h"
#include "utility.h"
#include "QDir"
#include "QDebug"

ConfigParser* ConfigParser::m_ConfigParser = 0;
ConfigParser::ConfigParser()
{
    QString file = QString(QDir::currentPath()+"/config.ini");
    qDebug()<<"file:"<<file<<"\n";

    if(QFile(file).exists())
    {
        qDebug()<<"File exists?"<<QFile(file).exists();
    }
    else
    {
        qDebug()<<"quiting config.ini file doesn't exist\n";
        exit(EXIT_FAILURE);
    }

    m_ConfigSettings = new QSettings(file, QSettings::IniFormat);
    m_ConfigSettings->sync();


     foreach(const QString& childKey, m_ConfigSettings->allKeys())
        {
            qDebug()<<"Key:"<<childKey<<"\n";
            qDebug()<<"Value:"<<m_ConfigSettings->value(childKey)<<"\n";
        }

    //fill keys list
    keys[KEY_TMESTAMP_FORMAT] = "GlobalSettings/TimeStampFormat";
    keys[KEY_DATE_FORMAT] = "GlobalSettings/DateFormat ";

    keys[KEY_MOUSE_MOVEMENT_CAPTURE_ENABLE] = "MouseCapture/Enable";
    keys[KEY_MOUSE_MOVEMENT_CAPTURE_DURATION] = "MouseCapture/CaptureDuration";
    keys[KEY_MOUSE_MOVEMENT_LOG_FOLDER_PATH] = "MouseCapture/LogFolderPath";
    keys[KEY_MOUSE_MOVEMENT_LOG_FILENAME_PREFIX] = "MouseCapture/LogFileNamePrefix";

    keys[KEY_SCREENSHOT_CAPTURE_ENABLE] = "ScreenCapture/Enable";
    keys[KEY_SCREENSHOT_CAPTURE_DURATION] = "ScreenCapture/CaptureDuration";
    keys[KEY_SCREENSHOT_LOG_FOLDER_PATH] = "ScreenCapture/LogFolderPath";
    keys[KEY_SCREENSHOT_IMAGE_TYPE] = "ScreenCapture/ImageType";
    keys[KEY_SCREENSHOT_IMAGE_QUALITY_RATIO] = "ScreenCapture/ImageQualityRatio";

    keys[KEY_SCAN_OPEN_NETWORK_PORTS_ENABLE] = "ScanNetworkPorts/Enable";
    keys[KEY_SCAN_OPEN_NETWORK_PORTS_DURATION] = "ScanNetworkPorts/CaptureDuration";
    keys[KEY_SCAN_OPEN_NETWORK_PORTS_FOLDER_PATH] = "ScanNetworkPorts/LogFolderPath";
    keys[KEY_SCAN_OPEN_NETWORK_PORTS_LOG_FILENAME_PREFIX] = "ScanNetworkPorts/LogFileNamePrefix";

    keys[KEY_KEY_PRESS_CAPTURE_ENABLE] = "KeyBoardCapture/Enable";
    keys[KEY_KEY_PRESS_CAPTURE_DURATION] = "KeyBoardCapture/CaptureDuration";
    keys[KEY_KEY_PRESS_CAPTURE_FOLDER_PATH] = "KeyBoardCapture/LogFolderPath";
    keys[KEY_KEY_PRESS_CAPTURE_LOG_FILENAME_PREFIX] = "KeyBoardCapture/LogFileNamePrefix";

    keys[KEY_CLIPBOARD_CAPTURE_ENABLE] = "ClipBoardCapture/Enable";
    keys[KEY_CLIPBOARD_CAPTURE_DURATION] = "ClipBoardCapture/CaptureDuration";
    keys[KEY_CLIPBOARD_CAPTURE_FOLDER_PATH] = "ClipBoardCapture/LogFolderPath";
    keys[KEY_CLIPBOARD_CAPTURE_LOG_FILENAME_PREFIX] = "ClipBoardCapture/LogFileNamePrefix";

    keys[KEY_URL_CAPTURE_ENABLE] = "URLCapture/Enable";
    keys[KEY_URL_CAPTURE_DURATION] = "URLCapture/CaptureDuration";
    keys[KEY_URL_CAPTURE_FOLDER_PATH] = "URLCapture/LogFolderPath";
    keys[KEY_URL_CAPTURE_LOG_FILENAME_PREFIX] = "URLCapture/LogFileNamePrefix";

}

ConfigParser::~ConfigParser()
{
    DELETE_OBJECT(m_ConfigSettings);
    DELETE_OBJECT(m_ConfigParser);
}

ConfigParser* ConfigParser::getInstance()
{
    if(m_ConfigParser == NULL)
    {
        m_ConfigParser = new ConfigParser();
    }
    return m_ConfigParser;
}

QString ConfigParser::getTimeStampFormat()
{
    return m_ConfigSettings->value(keys[KEY_TMESTAMP_FORMAT], "yyyy-MM-dd hh:mm:ss").toString();
}

QString ConfigParser::getDateFormat()
{
    return m_ConfigSettings->value(keys[KEY_DATE_FORMAT], "yyyy-MM-dd").toString();
}

bool ConfigParser::IsFeatureEnabled(FEATURE feature)
{
    bool Enabled;
    switch(feature)
    {
        case FEATURE_MOUSE_MOVEMENT_CAPTURE:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_MOUSE_MOVEMENT_CAPTURE_ENABLE], false).toBool();
        }break;
        case FEATURE_SCREENSHOT_CAPTURE:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_SCREENSHOT_CAPTURE_ENABLE], false).toBool();
        }break;
        case FEATURE_SCAN_OPEN_NETWORK_PORTS:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_SCAN_OPEN_NETWORK_PORTS_ENABLE], false).toBool();
        }break;
        case FEATURE_KEY_PRESS_CAPTURE:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_KEY_PRESS_CAPTURE_ENABLE], false).toBool();
        }break;
        case FEATURE_CLIPBOARD_CAPTURE:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_CLIPBOARD_CAPTURE_ENABLE], false).toBool();
        }break;
        case FEATURE_URL_CAPTURE:
        {
            Enabled = m_ConfigSettings->value(keys[KEY_URL_CAPTURE_ENABLE], false).toBool();
        }break;
        default:
        {
            Enabled = false;
        }
    }
    return Enabled;
}

int ConfigParser::getFeatureRecordDuration(FEATURE feature)
{
   int Duration;
    switch(feature)
    {
        case FEATURE_MOUSE_MOVEMENT_CAPTURE:
        {
            Duration = m_ConfigSettings->value(keys[KEY_MOUSE_MOVEMENT_CAPTURE_DURATION], 0).toInt();
        }break;
        case FEATURE_SCREENSHOT_CAPTURE:
        {
            Duration = m_ConfigSettings->value(keys[KEY_SCREENSHOT_CAPTURE_DURATION], 0).toInt();
        }break;
        case FEATURE_SCAN_OPEN_NETWORK_PORTS:
        {
            Duration = m_ConfigSettings->value(keys[KEY_SCAN_OPEN_NETWORK_PORTS_DURATION], 0).toInt();
        }break;
        case FEATURE_KEY_PRESS_CAPTURE:
        {
            Duration = m_ConfigSettings->value(keys[KEY_KEY_PRESS_CAPTURE_DURATION], 0).toInt();
        }break;
        case FEATURE_CLIPBOARD_CAPTURE:
        {
            Duration = m_ConfigSettings->value(keys[KEY_CLIPBOARD_CAPTURE_DURATION], 0).toInt();
        }break;
        case FEATURE_URL_CAPTURE:
        {
            Duration = m_ConfigSettings->value(keys[KEY_URL_CAPTURE_DURATION], 0).toInt();
        }break;
        default:
        {
            Duration = 0;
        }
    }
return Duration;
}

QString ConfigParser::getFeatureLogFolderLocation(FEATURE feature)
{
    QString PathFromConfigFile;
    QString DefaultPath = QDir::currentPath();
    switch(feature)
    {
        case FEATURE_MOUSE_MOVEMENT_CAPTURE:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_MOUSE_MOVEMENT_LOG_FOLDER_PATH], DefaultPath).toString();
        }break;
        case FEATURE_SCREENSHOT_CAPTURE:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_SCREENSHOT_LOG_FOLDER_PATH], DefaultPath).toString();
        }break;
        case FEATURE_SCAN_OPEN_NETWORK_PORTS:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_SCAN_OPEN_NETWORK_PORTS_FOLDER_PATH], DefaultPath).toString();
        }break;
        case FEATURE_KEY_PRESS_CAPTURE:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_KEY_PRESS_CAPTURE_FOLDER_PATH], DefaultPath).toString();
        }break;
        case FEATURE_CLIPBOARD_CAPTURE:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_CLIPBOARD_CAPTURE_FOLDER_PATH], DefaultPath).toString();
        }break;
        case FEATURE_URL_CAPTURE:
        {
            PathFromConfigFile = m_ConfigSettings->value(keys[KEY_URL_CAPTURE_FOLDER_PATH], DefaultPath).toString();
        }break;
        default:
        {
            PathFromConfigFile = DefaultPath;
        }
    }

    if(!QDir(PathFromConfigFile).exists() || PathFromConfigFile.isEmpty())
         PathFromConfigFile = DefaultPath;

    qDebug()<<"PathFromConfigFile: "<<PathFromConfigFile<<"\n";

return PathFromConfigFile;
}

QString ConfigParser::getFeatureLogFilePrefix(FEATURE feature)
{
    QString FileNamePrefix;
    switch(feature)
    {
        case FEATURE_MOUSE_MOVEMENT_CAPTURE:
        {
            FileNamePrefix = m_ConfigSettings->value(keys[KEY_MOUSE_MOVEMENT_LOG_FILENAME_PREFIX], "MouseLogs").toString();
        }break;
        case FEATURE_SCAN_OPEN_NETWORK_PORTS:
        {
            FileNamePrefix = m_ConfigSettings->value(keys[KEY_SCAN_OPEN_NETWORK_PORTS_LOG_FILENAME_PREFIX], "OpenPorts").toString();
        }break;
        case FEATURE_KEY_PRESS_CAPTURE:
        {
            FileNamePrefix = m_ConfigSettings->value(keys[KEY_KEY_PRESS_CAPTURE_LOG_FILENAME_PREFIX], "KeyPressedEvent").toString();
        }break;
        case FEATURE_CLIPBOARD_CAPTURE:
        {
            FileNamePrefix = m_ConfigSettings->value(keys[KEY_CLIPBOARD_CAPTURE_LOG_FILENAME_PREFIX], "KeyPressedEvent").toString();
        }break;
        case FEATURE_URL_CAPTURE:
        {
            FileNamePrefix = m_ConfigSettings->value(keys[KEY_URL_CAPTURE_LOG_FILENAME_PREFIX], "KeyPressedEvent").toString();
        }break;
        default:
        {
            FileNamePrefix = "";
        }
    }
return FileNamePrefix;
}

IMAGE_TYPE ConfigParser::getScreenshotImageType()
{
    QString imageType = m_ConfigSettings->value(keys[KEY_SCREENSHOT_IMAGE_TYPE], "JPEG").toString();
    IMAGE_TYPE screenshotImageType = (QString::compare(imageType, "JPEG", Qt::CaseInsensitive) == 0)? IMAGE_TYPE_JPEG : IMAGE_TYPE_PNG;

    return screenshotImageType;
}

int ConfigParser::getScreenshotImageQualityRatio()
{
    int imageQualityRatio = m_ConfigSettings->value(keys[KEY_SCREENSHOT_IMAGE_QUALITY_RATIO], -1).toInt();
    if(imageQualityRatio < -1 || imageQualityRatio > 100)
        imageQualityRatio = -1;
    return imageQualityRatio;
}
