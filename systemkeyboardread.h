#ifndef SYSTEMKEYBOARDREAD_H
#define SYSTEMKEYBOARDREAD_H

#include "configparser.h"
#include <Windows.h>
#include <QObject>
#include <QFile>
#include <QTimer>
#include <QTextStream>

class SystemKeyboardRead : public QObject
{
    Q_OBJECT
public:    
    ~SystemKeyboardRead();

    static SystemKeyboardRead* getInstance();
    // Returns whether the keyboard hook is connected
    bool connected();
    bool setConnected(bool state);
    void mapVKeyCode(KBDLLHOOKSTRUCT *pKeyBoard);
    void log(const char *s1);
    void CaptureKeyEvents();

signals:
    void keyPressed(byte *keysDepressed, DWORD keyPressed);

public slots:
    void recordTimeStamp();

private:
    explicit SystemKeyboardRead(QObject *parent = 0);
    static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

    static const int numberKeys = 256;
    static SystemKeyboardRead *Instance;
    HHOOK keyboardHook;

    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_KeyEventCaptureEnabled;
    QString m_LogDirectoryPath;
    int m_CaptureDuration;
    QString m_LogFolder;
    QString m_FileNamePrefix;
    QFile* m_File;
    QTextStream* m_TextStream;
};

#endif // SYSTEMKEYBOARDREAD_H
