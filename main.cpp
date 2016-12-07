#include <QtGui/QApplication>
#include "udp_handler.h"
//#include "tcp_handler.h"
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

    udp_handler *udpHandler = new udp_handler(this);

    rtsp_handler *rtsphandler  = new rtsp_handler();
    xml_handler *xmlhandler = new xml_handler();


    QObject::connect(udpHandler, SIGNAL(probe_received()), xmlhandler, SLOT(create_getSysDTRes()));
    QObject::connect(rtsphandler, SIGNAL(sendVidSrcCnfg(QString, QString, QString)), xmlhandler, SLOT(storeVidSrcCnfg(QString, QString, QString)));

    //rtsphandler->start(QThread::HighestPriority);

    return;
}
