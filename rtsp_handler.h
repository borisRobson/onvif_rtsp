#ifndef RTSP_HANDLER_H
#define RTSP_HANDLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QtCore>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

class rtsp_server : public QThread
{
    Q_OBJECT
public:
    rtsp_server(int ID = 0, QObject *parent = 0);
    void run();

public slots:
    void beginServe();

signals:
    void serve();
};

class rtsp_handler :  public QObject
{
    Q_OBJECT
public:
    rtsp_handler(QObject *parent = 0);
    //void start();
    void setupServer();

private:
    rtsp_server *rtspServer;

public slots:
    void updateVidSrcCnfg();
    void start();

signals:
    void sendVidSrcCnfg(QString, QString, QString);

};



#endif // RTSP_HANDLER_H
