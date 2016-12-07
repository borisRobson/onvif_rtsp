#include "udp_send.h"
using namespace std;

udp_send::udp_send()
{

}

void udp_send::send_udp(QString ID, char *addr, int port)
{
    QByteArray res =  create_probe_res(ID);
    QUdpSocket *socket = new QUdpSocket(this);
    QHostAddress send_addr = QHostAddress(addr);


    qint64 err =  socket->writeDatagram(res, send_addr, port);
    qDebug() << err;
}

QByteArray udp_send::create_probe_res(QString ID)
{
    QDomDocument doc;
    QFile probetemplate("./tcpComms/xmltemplates/probereq.xml");
    if(!probetemplate.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "could not open template";
    }else{
        doc.setContent(&probetemplate);
        probetemplate.close();
    }

    QString msg_uid = create_uuid();
    QString endpoint = create_uuid();
    //TODO load hardware id from prop bag
    QString scopes = QString("onvif://www.onvif.org/location/country/England onvif://www.onvif.org/name/Paxton onvif://www.onvif.org/hardware/Net2_Entry onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/type/Network_Video_Transmitter onvif://www.onvif.org/extension/unique_identifier");

    QString localip;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             localip =  QString(address.toString());
    }
    localip.prepend("http://");
    localip.append("/onvif/device_service");
    qDebug() << localip;

    //get the root element
    QDomNode root = doc.firstChildElement();

    QDomNodeList children = root.childNodes();
    for(int i = 0; i < children.count(); i++)
    {
        QDomElement ele = children.at(i).toElement();

        if(ele.nodeName().contains("Header", Qt::CaseInsensitive))
        {
            QDomNodeList hdlist = ele.childNodes();
            for(int i = 0; i < hdlist.count(); i++)
            {
                QDomElement hdr = hdlist.at(i).toElement();

                if(hdr.nodeName().contains("MessageID", Qt::CaseInsensitive)){
                    QDomText txt = doc.createTextNode(msg_uid);
                    hdr.appendChild(txt);
                }
                else if(hdr.nodeName().contains("RelatesTo", Qt::CaseInsensitive)){
                    QDomText txt = doc.createTextNode(ID);
                    hdr.appendChild(txt);
                }
            }
        }
        else if(ele.nodeName().contains("Body", Qt::CaseInsensitive)){
            QDomElement pmatches = ele.firstChild().toElement();
            QDomElement pmatch = pmatches.firstChild().toElement();
            QDomNodeList matchlist = pmatch.childNodes();
            for(int i = 0; i < matchlist.count(); i++){
                QDomElement bdyelem = matchlist.at(i).toElement();
                if(bdyelem.nodeName().contains("Endpoint", Qt::CaseInsensitive)){
                    QDomElement addr = bdyelem.firstChild().toElement();
                    QDomText txt = doc.createTextNode(endpoint);
                    addr.appendChild(txt);
                }
                else if(bdyelem.nodeName().contains("Scopes", Qt::CaseInsensitive)){
                    QDomText txt = doc.createTextNode(scopes);
                    bdyelem.appendChild(txt);
                }
                else if(bdyelem.nodeName().contains("Xaddr", Qt::CaseInsensitive)){
                    QDomText txt = doc.createTextNode(localip);
                    bdyelem.appendChild(txt);
                }

            }

        }
    }

    return doc.toByteArray();
}

QString udp_send::create_uuid()
{
    //create uuid
    QUuid uid = QUuid::createUuid();

    QString uuidprfx = "uuid:";
    QString uuid = QString(uid.toString());

    //format uuid
    uuid.chop(1);
    uuid.remove(0,1);
    uuid.prepend(uuidprfx);

    return uuid;
}
