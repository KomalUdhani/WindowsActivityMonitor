#include "capturepacketproducer.h"
//#define WPCAP
#include <winsock2.h>
#include "stddef.h"
#include "remote-ext.h"
#include <QDebug>


const int BUFFERSIZE = 50;

CapturePacketProducer::CapturePacketProducer(QList<struct packet>& packetList, QSemaphore& upProcessedPackets, QSemaphore& processedPackets)
    : QThread()
    , m_upProcessedPackets(upProcessedPackets)
    , m_processedPackets(processedPackets)
    , m_PacketList(packetList)
{
}

CapturePacketProducer::~CapturePacketProducer()
{

}

void CapturePacketProducer::run()
{
    qDebug()<<"Producer Run\n";
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i=0, adapterNum;
    char errbuf[PCAP_ERRBUF_SIZE];
    u_int netmask;
    struct bpf_program fcode;

    /* Retrieve the device list on the local machine */
    if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    /* Print the list */
    for(d=alldevs; d; d=d->next)
    {
        printf("%d. %s\n", ++i, d->name);
        if(d->description)
            printf("(%s)\n", d->description);
        else
            printf("(No description available)\n");
    }
    adapterNum = i;
    printf("\n No of Adapters:%d\n",adapterNum);

    if(0 == i)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        //return -1;
        exit();
    }
    int j=0;
    d=alldevs;
    pcap_t** adhandle = new pcap_t*[adapterNum];
  for(d=alldevs, j=adapterNum; j>0 && d; j--,d=d->next)
  {
    printf("j=%d\n",j);
    if(d->addresses != NULL)
        /* Retrieve the mask of the first address of the interface */
        netmask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* If the interface is without an address we suppose to be in a C class network */
        netmask = 0xffffff;

    /*Open the device*/
    if((adhandle[j] = pcap_open(d->name,
                             65536,
                             PCAP_OPENFLAG_PROMISCUOUS,
                             /*timeout check if efficient for blkng call pcap_next_ex*/500,
                             NULL,
                             errbuf))==NULL)
    {
        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
                /* Free the device list */
                pcap_freealldevs(alldevs);

                //pcap_compile()
                //return -1;
                exit();
    }
    else
        fprintf(stderr,"Opened adapter sucessfully : %s\n",d->description);

    //compile the filter
    //"tcp and dst port 80"
    if(pcap_compile(adhandle[j], &fcode, "ip and tcp", 1, netmask) < 0)
    {
        fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
        /* Free the device list */
        pcap_freealldevs(alldevs);
        //return -1;
        exit();
    }

    //set filter
    if(pcap_setfilter(adhandle[j], &fcode) < 0)
    {
        fprintf(stderr,"\nError setting the filter.\n");
        /* Free the device list */
        pcap_freealldevs(alldevs);
        //return -1;
        exit();
    }

    printf("\nlistenning on %s...\n", d->description);
   }
  /* Free the device list */
  pcap_freealldevs(alldevs);

//  /*start capturing packets*/
  struct pcap_pkthdr *pkt_header;
  const u_char *pkt_data;
  int count = 0;
  while(1)
  {
      for(j=1; j<=adapterNum; j++)
      {
          //j=7;
          if(pcap_next_ex(adhandle[j],&pkt_header, &pkt_data) > 0)
          {
              //enqqueue packet in pkt struct list
              if(pkt_data)
              {
              // printf("got pkt on adapter:%d\t",j);

               m_upProcessedPackets.acquire();
               //printf("Producer: m_upProcessedPackets=%d\n",m_upProcessedPackets.available());

               if(count >= BUFFERSIZE)
               {
                   count =0;
               }
               struct packet localPacket;
               localPacket.pkt_header = pkt_header;
               localPacket.pkt_data = pkt_data;
               localPacket.adapterId= j;
               if(m_PacketList.count() < BUFFERSIZE)
               {
                  m_PacketList.append(localPacket);
               }
               else
               {
                   m_PacketList[count] = localPacket;
               }
               //printf("Producer List Count:%d\n",m_PacketList.count());

               count ++;
               m_processedPackets.release();
               //printf("Producer: m_processedPackets=%d\n",m_processedPackets.available());
              //printf("end got pkt on adapter:%d\n",j);
              }
          }
      }
  }

}

