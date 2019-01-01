#ifndef CAPTUREPACKETPRODUCER_H
#define CAPTUREPACKETPRODUCER_H

#include "pcap.h"
#include <QThread>
#include <QSemaphore>

struct packet
{
    struct pcap_pkthdr* pkt_header;
    const u_char * pkt_data;
    int adapterId;
};

Q_DECLARE_METATYPE(struct packet)

class CapturePacketProducer: public QThread
{
    Q_OBJECT
public:
    CapturePacketProducer(QList<struct packet>&, QSemaphore&, QSemaphore&);
    ~CapturePacketProducer();
protected:
    virtual void run() /*Q_DECL_OVERRIDE*/;

private:
    QSemaphore& m_upProcessedPackets;
    QSemaphore& m_processedPackets;
    QList<struct packet>& m_PacketList;
};

#endif // CAPTUREPACKETPRODUCER_H
