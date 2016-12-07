#ifndef UDP_LISTEN_H
#define UDP_LISTEN_H

#include <QRunnable>
#include <QObject>
#include <QDebug>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <iostream>
#include <string>

class udp_listen : public QObject,public QRunnable
{
    Q_OBJECT
public:
    udp_listen();

protected:
    void run();

signals:
    void dataRecieved(QByteArray msg, char* src_addr, int port);

private:
    int fd;
    struct sockaddr_in addr;
};

#endif // UDP_LISTEN_H
