#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

#include <QObject>
#include <string>
#include <QtXml>
#include <QDebug>
#include <QThreadPool>

#include "udp_listen.h"
#include "udp_send.h"


class udp_handler : public QObject
{
    Q_OBJECT
public:
    explicit udp_handler(QObject *parent = 0);
    void parse(QByteArray msg, char* src_addr, int src_port);
    QString parseHeader(QDomElement hdr);
    bool parseBody(QDomElement bdy);
    //void start();

signals:
    void send_udp(QString , char* , int );
    void probe_received();


public slots:
    void parse_msg(QByteArray msg, char* src_addr, int port);
    void start();

private:
    udp_listen *listener;
    udp_send *sender;

};

class Task: public QObject{
    Q_OBJECT
public:
    Task(QObject* parent=0) : QObject(parent){}
private:

public slots:
    void run();
signals:
    void finished();
    void beginRtsp();
    void beginUdp();
    void beginTcp();
};

#endif // UDP_HANDLER_H
