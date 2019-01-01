#ifndef MYHOOKMOUSEEVENT_H
#define MYHOOKMOUSEEVENT_H
#include <Windows.h>
#include <QDebug>
#include <QTimer>
#include <QObject>
#include <QFile>

class ConfigParser;
class MyHookMouseEvent : public QObject
{
    Q_OBJECT

private:
    MyHookMouseEvent(QObject* /*parent*/ = 0);
    ~MyHookMouseEvent();
    ConfigParser* m_ConfigParser;
    QTimer* m_Timer;
    bool m_MouseMovementCaptureEnabled;
    QString m_LogDirectoryPath;
    int m_CaptureDuration;
    QString m_LogFolder;
    QString m_FileNamePrefix;
    QFile* m_File;
    QTextStream* m_TextStream;

public:
    //singleton
    static MyHookMouseEvent& Instance();
    HHOOK hook; //handle to the hook
    void CaptureMouseMovement();
    void InstallHook();
    void UninstallHook();
    void resetTimer();    


    MSG msg;// struct with information about all msgs in our queue
    int Messages(); //function to deal with msgs

public slots:
    void notifyMouseNotMoved();

};

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam); // callback declaration
#endif // MYHOOKMOUSEEVENT_H
