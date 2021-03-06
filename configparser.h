#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QSettings>

enum FEATURE
{
    FEATURE_MOUSE_MOVEMENT_CAPTURE,
    FEATURE_SCREENSHOT_CAPTURE,
    FEATURE_SCAN_OPEN_NETWORK_PORTS,
    FEATURE_KEY_PRESS_CAPTURE,
    FEATURE_CLIPBOARD_CAPTURE,
    FEATURE_URL_CAPTURE,
    FEATURE_COUNT
};

enum KEY
{
    KEY_TMESTAMP_FORMAT,
    KEY_DATE_FORMAT,

    KEY_MOUSE_MOVEMENT_CAPTURE_ENABLE,
    KEY_MOUSE_MOVEMENT_CAPTURE_DURATION,
    KEY_MOUSE_MOVEMENT_LOG_FOLDER_PATH,
    KEY_MOUSE_MOVEMENT_LOG_FILENAME_PREFIX,

    KEY_SCREENSHOT_CAPTURE_ENABLE,    
    KEY_SCREENSHOT_CAPTURE_DURATION,
    KEY_SCREENSHOT_LOG_FOLDER_PATH,
    KEY_SCREENSHOT_IMAGE_TYPE,
    KEY_SCREENSHOT_IMAGE_QUALITY_RATIO,

    KEY_SCAN_OPEN_NETWORK_PORTS_ENABLE,
    KEY_SCAN_OPEN_NETWORK_PORTS_DURATION,
    KEY_SCAN_OPEN_NETWORK_PORTS_FOLDER_PATH,
    KEY_SCAN_OPEN_NETWORK_PORTS_LOG_FILENAME_PREFIX,

    KEY_KEY_PRESS_CAPTURE_ENABLE,
    KEY_KEY_PRESS_CAPTURE_DURATION,//this will record time stamp with this duration
    KEY_KEY_PRESS_CAPTURE_FOLDER_PATH,
    KEY_KEY_PRESS_CAPTURE_LOG_FILENAME_PREFIX,

    KEY_CLIPBOARD_CAPTURE_ENABLE,
    KEY_CLIPBOARD_CAPTURE_DURATION,
    KEY_CLIPBOARD_CAPTURE_FOLDER_PATH,
    KEY_CLIPBOARD_CAPTURE_LOG_FILENAME_PREFIX,

    KEY_URL_CAPTURE_ENABLE,
    KEY_URL_CAPTURE_DURATION,
    KEY_URL_CAPTURE_FOLDER_PATH,
    KEY_URL_CAPTURE_LOG_FILENAME_PREFIX,

    KEY_COUNT
};

enum IMAGE_TYPE
{
    IMAGE_TYPE_PNG,
    IMAGE_TYPE_JPEG
};
class ConfigParser
{
public:
    static ConfigParser* getInstance();
    QString getTimeStampFormat();
    QString getDateFormat();

    bool IsFeatureEnabled(FEATURE feature);
    int getFeatureRecordDuration(FEATURE feature);
    QString getFeatureLogFolderLocation(FEATURE feature);
    QString getFeatureLogFilePrefix(FEATURE feature);

    IMAGE_TYPE getScreenshotImageType();
    int getScreenshotImageQualityRatio();

private:
    ConfigParser();
    ~ConfigParser();
    static ConfigParser* m_ConfigParser;
    QSettings *m_ConfigSettings;
    QString keys[KEY_COUNT];
};

#endif // CONFIGPARSER_H
