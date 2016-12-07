#ifndef XML_CREATOR_H
#define XML_CREATOR_H

#include <QObject>
#include <QtXml>
#include <QUuid>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QString>
#include <QDateTime>

class xml_creator
{

private:
    QString ipv4addr;
    QString ipv6addr;
    QString hwAddr;
    QString analyticsService;
    QString deviceService;
    QString eventService;
    QString imagingService;
    QString mediaService;
    QString deviceIOService;
    QString telecomService;
    QString analyticsWsdl;
    QString deviceWsdl;
    QString eventWsdl;
    QString imagingWsdl;
    QString mediaWsdl;
    QString deviceIOWsdl;


public:
    xml_creator();
    QDomDocument createGetSysDTRes(QDomDocument doc);
    QDomDocument createGetDevInfoRes(QDomDocument doc);
    QDomDocument createGetDNSRes(QDomDocument doc);
    QDomDocument createGetNetworkInterfacesRes(QDomDocument doc);
    QDomDocument createGetCapabilitiesRes(QDomDocument doc);
    QDomDocument createGetServicesRes(QDomDocument doc);
    QDomDocument createGetStreamUriRes(QDomDocument doc);
    QDomDocument createGetVideoSourcesRes(QDomDocument doc,QString width, QString height, QString framerate );
};

#endif // XML_CREATOR_H
