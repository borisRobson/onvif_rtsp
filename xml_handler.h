#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include <QObject>
#include <QtXml>
#include <QUuid>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QString>
#include <QDateTime>
#include <QProcess>

#include "xml_creator.h"

class xml_handler : public QObject
{
    Q_OBJECT
public:
    xml_handler();

    QDomDocument loadFile(QString filename);
    void writeFile(QDomDocument doc, QString filename);

    void create_getDevInfoRes();
    void create_getScopes();
    void create_getVidSourcesRes();
    void create_getDNSRes();
    void create_getNtwkInterfacesRes();
    void create_getCapabilitiesRes();
    void create_getServicesRes();
    void create_getProfilesRes();
    void create_getSnapshotRes();
    void create_getProfileRes();
    void create_getStreamUriRes();
    void create_getVideoSourceConfigRes();

    bool createDir();

signals:
    void updateVidSrcConfig();


private:
    QString filepath;
    xml_creator xmlcreator;
    QString width;
    QString height;
    QString framerate;




public slots:
    void create_getSysDTRes();
    void storeVidSrcCnfg(QString width, QString height, QString framerate);

};

#endif // XML_HANDLER_H
