#-------------------------------------------------
#
# Project created by QtCreator 2016-08-15T19:54:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WindowsMonitoringApplication
TEMPLATE = app

DEFINES += HAVE_REMOTE
INCLUDEPATH += "D:\freelancerProjects\QTCreateRemoteLoginFeatureSW\WpdPack_4_1_2\WpdPack\Include"
LIBS+="-LD:\freelancerProjects\QTCreateRemoteLoginFeatureSW\WpdPack_4_1_2\WpdPack\Lib" -lwpcap -lws2_32 -lPacket


SOURCES += main.cpp\
    takescreenshot.cpp \
    myhookmouseevent.cpp \
    utility.cpp \
    configparser.cpp \
    scannetworkports.cpp \
    systemkeyboardread.cpp \
    clipboardcapture.cpp \
    captureurls.cpp \
    capturepacketproducer.cpp \
    capturepacketconsumer.cpp

HEADERS  += \
    takescreenshot.h \
    myhookmouseevent.h \
    utility.h \
    configparser.h \
    scannetworkports.h \
    systemkeyboardread.h \
    clipboardcapture.h \
    captureurls.h \
    capturepacketproducer.h \
    capturepacketconsumer.h

FORMS    += mainwindow.ui
