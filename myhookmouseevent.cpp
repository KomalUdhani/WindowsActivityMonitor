#include "myhookmouseevent.h"
#include "utility.h"
#include "configparser.h"
#include <QDir>


MyHookMouseEvent::MyHookMouseEvent(QObject* /*parent*/)
    : m_ConfigParser(ConfigParser::getInstance())
    , m_Timer(new QTimer(this))
    , m_File(NULL)
{
    m_MouseMovementCaptureEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_MOUSE_MOVEMENT_CAPTURE);
    if(m_MouseMovementCaptureEnabled)
    {        
        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_MOUSE_MOVEMENT_CAPTURE);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_LogDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_MOUSE_MOVEMENT_CAPTURE);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_MOUSE_MOVEMENT_CAPTURE,m_LogDirectoryPath);

        m_FileNamePrefix = m_ConfigParser->getFeatureLogFilePrefix(FEATURE_MOUSE_MOVEMENT_CAPTURE);
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(notifyMouseNotMoved()));
    }
}

MyHookMouseEvent::~MyHookMouseEvent()
{
    if(m_File->isOpen())
        m_File->close();
    DELETE_OBJECT(m_TextStream)
    DELETE_OBJECT(m_File);
    DELETE_OBJECT(m_Timer);
}

void MyHookMouseEvent::CaptureMouseMovement()
{
    if(m_MouseMovementCaptureEnabled)
    {
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
        InstallHook();
        m_Timer->start(m_CaptureDuration*60*1000);
    }

}

void MyHookMouseEvent::notifyMouseNotMoved()
{
    qDebug()<<"mouse not moved in last two minutes timestamp:"<<Utility::getFormattedCurrentTimeStamp() <<"\n";
    //write timestamp to log file
    if(m_TextStream != NULL)
    {
        (*m_TextStream) << "MouseNotMovedTimeStamp" << "," << Utility::getFormattedCurrentTimeStamp() <<"\n";
        m_TextStream->flush();
    }
}

MyHookMouseEvent& MyHookMouseEvent::Instance()
{
    static MyHookMouseEvent myHook;
    return myHook;
}

int MyHookMouseEvent::Messages()
{
    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(1);
    }
    UninstallHook();
    return (int)msg.wParam;
}

void MyHookMouseEvent::InstallHook()
{    
    if(!(hook = SetWindowsHookEx(WH_MOUSE_LL, MyMouseCallback, NULL, 0)))
    {        
        qDebug()<<"Error:"<<GetLastError();
    }
}

void MyHookMouseEvent::UninstallHook()
{
    UnhookWindowsHookEx(hook);
}

void MyHookMouseEvent::resetTimer()
{
    m_Timer->start(m_CaptureDuration*60*1000);
}

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{   
    MSLLHOOKSTRUCT *pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    if(nCode == 0)
    {
        //we have information in wParam/lParam?
        if(pMouseStruct != NULL)
        {            
            qDebug()<<"Mouse Coordinates: x="<< pMouseStruct->pt.x << "y=" << pMouseStruct->pt.y;
            MyHookMouseEvent::Instance().resetTimer();
        }
    }
    return CallNextHookEx(MyHookMouseEvent::Instance().hook, nCode, wParam, lParam);
}

