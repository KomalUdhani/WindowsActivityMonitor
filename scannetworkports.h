#ifndef SCANNETWORKPORTS_H
#define SCANNETWORKPORTS_H

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QProcess>

class ConfigParser;
class ScanNetworkPorts : public QObject
{    
    Q_OBJECT

public:
    ScanNetworkPorts(QObject* /*parent*/ = 0);
    ~ScanNetworkPorts();

public slots:
    void CaptureOpenNetworkPorts();

private:
    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_CaptureNetworkPortsEnabled;
    QString m_LogDirectoryPath;
    QString m_LogFolder;
    int m_CaptureDuration;
    QString m_FileNamePrefix;
    QFile* m_File;
    QTextStream* m_TextStream;
    QProcess *m_ExecuteNetstatProcess;
};

#endif // SCANNETWORKPORTS_H
