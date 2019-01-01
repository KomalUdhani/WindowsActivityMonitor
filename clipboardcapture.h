#ifndef CLIPBOARDCAPTURE_H
#define CLIPBOARDCAPTURE_H

#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QClipboard>

class ConfigParser;
class ClipboardCapture : public QObject
{
    Q_OBJECT

public:
    ClipboardCapture(QObject* /*parent*/ = 0);
    ~ClipboardCapture();

public slots:
    void CaptureClipboardData();
    void AddClipboardData();

private:
    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_CaptureClipboardDataEnabled;
    QString m_LogDirectoryPath;
    QString m_LogFolder;
    int m_CaptureDuration;
    QString m_FileNamePrefix;
    QFile* m_File;
    QTextStream* m_TextStream;
    QClipboard* m_Clipboard;
    QString m_ClipboardString;
};


#endif // CLIPBOARDCAPTURE_H
