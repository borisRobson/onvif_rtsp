#ifndef RTSP_HANDLER_H
#define RTSP_HANDLER_H
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QtCore>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>


class rtsp_handler :  public QObject
{
    Q_OBJECT
public:
    rtsp_handler();
    void start();

public slots:
    void updateVidSrcCnfg();



signals:
    void sendVidSrcCnfg(QString, QString, QString);


};

#endif // RTSP_HANDLER_H
