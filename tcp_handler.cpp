#include "tcp_handler.h"
#include <QDebug>

tcp_listener *listener;
tcp_handler::tcp_handler(QObject* parent)
{
#ifdef IMX6
    process = "node ./tcpComms/imxMain.js";
#else
    process = "sudo node ./tcpComms/main.js";
#endif
    listener = new tcp_listener(2, this);
}

void tcp_handler::start()
{
    listener->start(QThread::LowestPriority);
}

tcp_listener::tcp_listener(int ID, QObject *parent)
{
    qDebug() << "TcpThread ID: " << ID  << " this thread: "<< this<< " Parent: " << parent;
}

void tcp_listener::run()
{
    tcpProcess = new QProcess();
#ifdef IMX6
    tcpProcess->start("node ./tcpComms/imxMain.js");
#else
    tcpProcess->start("sudo node ./tcpComms/main.js");
#endif

    if(!tcpProcess->waitForStarted())
        return;

    exec();
}
