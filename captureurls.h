#ifndef CAPTUREURLS_H
#define CAPTUREURLS_H

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QObject>

class ConfigParser;
class CaptureURLs : public QObject
{
    Q_OBJECT
public:
    explicit CaptureURLs(QObject *parent= 0);
    ~CaptureURLs();
    bool isFeatureEnabled();

signals:

public slots:
    void OnCaptureURLData(const char *url);
    void WriteURLData();

private:
    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_CaptureUrlDataEnabled;
    QString m_LogDirectoryPath;
    QString m_LogFolder;
    int m_CaptureDuration;
    QString m_FileNamePrefix;
    QFile* m_File;
    QTextStream* m_TextStream;
    QString m_UrlString;
};

#endif // CAPTUREURLS_H
