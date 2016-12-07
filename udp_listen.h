#ifndef UDP_LISTEN_H
#define UDP_LISTEN_H

#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>

#include <iostream>
#include <string>

class udp_listen : public QThread
{
    Q_OBJECT
public:
    udp_listen(int ID = 0, QObject *parent = 0);
    void run();

signals:
    void dataRecieved(QByteArray msg, char* src_addr, int port);
    void beginListen();

public slots:
    void msgListen();

private:
    int fd;
    struct sockaddr_in addr;
};

#endif // UDP_LISTEN_H
