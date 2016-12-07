#include <QtGui/QApplication>
#include "udp_handler.h"
#include "tcp_handler.h"
#include "xml_handler.h"
#include "rtsp_handler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Task* task = new Task();

    QObject::connect(task, SIGNAL(finished()),&app, SLOT(quit()));

    QTimer::singleShot(1, task, SLOT(run()));

    if(!gst_is_initialized())
        gst_init(NULL, NULL);

    return app.exec();
}

void Task::run()
{
    rtsp_handler *rtsphandler = new rtsp_handler(this);
    xml_handler xmlhandler;
    tcp_handler *tcpHandler = new tcp_handler(this);
    udp_handler *udpHandler = new udp_handler(this);

    qDebug() << QThread::idealThreadCount();

    QObject::connect(udpHandler, SIGNAL(probe_received()), &xmlhandler, SLOT(create_getSysDTRes()));
    QObject::connect(rtsphandler, SIGNAL(sendVidSrcCnfg(QString, QString, QString)),&xmlhandler, SLOT(storeVidSrcCnfg(QString, QString, QString)));

    QObject::connect(this, SIGNAL(beginRtsp()), rtsphandler, SLOT(start()));
    QObject::connect(this, SIGNAL(beginUdp()), udpHandler, SLOT(start()));
    QObject::connect(this, SIGNAL(beginTcp()), tcpHandler, SLOT(start()));

    //emit beginTcp();
    emit beginUdp();
    emit beginRtsp();

    return;
}
