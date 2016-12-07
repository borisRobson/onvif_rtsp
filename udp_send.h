#ifndef UDP_SEND_H
#define UDP_SEND_H

#include <QObject>
#include <QtXml>
#include <QUuid>
#include <QNetworkInterface>
#include <QUdpSocket>

class udp_send : public QObject
{
    Q_OBJECT
public:
    explicit udp_send();

signals:


public slots:
    void send_udp(QString ID, char* addr, int port);
    QByteArray create_probe_res(QString ID);
    QString create_uuid();

private:
    QUdpSocket *sendsocket;

};

#endif // UDP_SEND_H
