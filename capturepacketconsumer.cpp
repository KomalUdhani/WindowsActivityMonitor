#include "capturepacketconsumer.h"
#include "string.h"
#include <QDebug>

const int BUFFERSIZE = 50;

CapturePacketConsumer::CapturePacketConsumer(QList<struct packet>& packetList, QSemaphore& upProcessedPackets, QSemaphore& processedPackets)
    : QThread()
    , m_upProcessedPackets(upProcessedPackets)
    , m_processedPackets(processedPackets)
    , m_PacketList(packetList)
{
}

CapturePacketConsumer::~CapturePacketConsumer()
{

}

void CapturePacketConsumer::run()
{

    qDebug()<<"Consumer Run\n";
  int count = 0;
  while(1)
  {
      //dequeue packet in pkt struct list
      //printf("got pkt on adapter:%d\t",lo);

          m_processedPackets.acquire();
          //printf("Consumer: m_processedPackets=%d\n",m_processedPackets.available());
          if(count >= BUFFERSIZE)
          {
              count =0;
          }

          struct packet localPacket;
          //printf("Consumer pktCount:%d  local count:%d",m_PacketList.count(),count);
          if(m_PacketList.empty())
              printf("\tConsumer Empty List");
          if(!m_PacketList.isEmpty() /*&& m_PacketList.count()>count*/)
           {
              localPacket = m_PacketList[count];
              count++;
              m_upProcessedPackets.release();
               //printf("Consumer: m_upProcessedPackets=%d\n",m_upProcessedPackets.available());
          }
          else
              printf("Consumer: did not get proper pkt data\n");

          /*extract url from packet*/
          int tcp_len, url_length;
          u_int ip_len;
          uchar  *tcpPayload;
          ip_header *ih;


          /* retireve the position of the ip header */
          ih = (ip_header *) (localPacket.pkt_data +14); //length of ethernet header

          /* retireve the position of the tcp header */
          ip_len = (ih->ver_ihl & 0xf) * 4;

          /* retireve the position of the tcp payload */
          tcp_len = (((uchar*)ih)[ip_len + 12] >> 4) * 4;
          tcpPayload = (uchar*)ih + ip_len + tcp_len;

          //check if payload present
          if(strlen((char*)tcpPayload) != 0)
          {
              //printf("tcpPayload:%s\n",(char*)tcpPayload);
              //if GET request
              //then get portion of Host: till \n
              if(strstr((char*)tcpPayload,"GET"))
              {
                  //printf("Consumer: counter:%d tcppayload: %s\n",count,(char*)tcpPayload);
                  if(strstr((char*)tcpPayload,"GET"))
                  {
                      char* hostString = strstr((char*)tcpPayload,"Host:");
                      if(hostString != NULL){
                          char* urlstr = hostString + 6 ; //Host: url, 6 chars till url
                          //char* urlstr = strstr((char*)hostString," ");
                          if(strlen(urlstr) != 0)
                          {
                              char* urlend = strchr(urlstr,'\n');
                              if(strlen(urlend) != 0)
                              {
                                  url_length = urlend - urlstr;
                                  char* final_url = (char*)malloc( url_length+ 1);
                                  if(final_url)
                                  {
                                      strncpy(final_url, urlstr, url_length);

                                      final_url[url_length]='\0';
                                      printf("urlstr:%s\n",final_url);
                                      qDebug()<<"Consumer URL: "<<final_url;
                                      emit SignalURLCapture(final_url);
                                      free(final_url);
                                  }
                              }
                          }
                      }
                  }
              }
          }
          //printf("Consumer: end got pkt on adapter:%d\n",localPacket.adapterId);
  }
}
