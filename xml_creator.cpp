#include "xml_creator.h"

#define ANALYTICS 0
#define DEVICE 1
#define EVENTS 2
#define IMAGING 3
#define MEDIA 4
#define DEVICEIO 5


xml_creator::xml_creator()
{
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if(interface.flags() && QNetworkInterface::CanBroadcast)
            hwAddr = interface.hardwareAddress();
    }

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             ipv4addr = address.toString();
        else if(address.protocol() == QAbstractSocket::IPv6Protocol && address != QHostAddress(QHostAddress::LocalHostIPv6)){
               QString tempAddr = address.toString();
               if(tempAddr.indexOf(QRegExp("[FE80]"),0) == 0)
                    ipv6addr = tempAddr;
        }
    }

    analyticsService = QString("http://") + ipv4addr + QString("/onvif/analytics_service");
    deviceService = QString("http://") + ipv4addr + QString("/onvif/device_service");
    eventService = QString("http://") + ipv4addr + QString("/onvif/event_service");
    imagingService = QString("http://") + ipv4addr + QString("/onvif/imaging_service");
    mediaService = QString("http://") + ipv4addr + QString("/onvif/media_service");
    deviceIOService = QString("http://") + ipv4addr + QString("/onvif/deviceIO_service");
    telecomService = QString("http://") + ipv4addr + QString("/onvif/telecom_service");

    analyticsWsdl = "http://www.onvif.org/ver20/analytics/wsdl";
    deviceWsdl = "http://www.onvif.org/ver10/device/wsdl";
    eventWsdl = "http://www.onvif.org/ver10/events/wsdl";
    imagingWsdl = "http://www.onvif.org/ver20/imaging/wsdl";
    mediaWsdl = "http://www.onvif.org/ver10/media/wsdl";
    deviceIOWsdl = "http://10.10.40.17/onvif/deviceIO_service";

}

QDomDocument xml_creator::createGetSysDTRes(QDomDocument doc)
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode sysdtnode = ele.firstChildElement().firstChildElement();
            QDomNodeList dtnodes = sysdtnode.childNodes();
            for(int i =0; i < dtnodes.count(); i++)
            {
                QDomNode dtNode = dtnodes.at(i).toElement();
                if(dtNode.nodeName().contains("DateTimeType", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("Manual");
                    dtNode.appendChild(type);
                }
                else if(dtNode.nodeName().contains("DaylightSavings", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("false");
                    dtNode.appendChild(type);
                }
                else if(dtNode.nodeName().contains("TimeZone", Qt::CaseInsensitive))
                {
                    QDomElement tz = dtNode.firstChild().toElement();
                    QDomText type = doc.createTextNode("GMT+00:00");
                    tz.appendChild(type);
                }
                else if(dtNode.nodeName().contains("DateTime", Qt::CaseInsensitive))
                {
                    QDomNodeList dtList = dtNode.childNodes();
                    for(int i = 0; i < dtList.count(); i++)
                    {
                        QDomElement dt = dtList.at(i).toElement();
                        if(dt.nodeName().contains("Time", Qt::CaseInsensitive))
                        {
                            QDomNodeList timeList = dt.childNodes();
                            for(int i = 0; i < timeList.count(); i++)
                            {
                                QDomElement timenode = timeList.at(i).toElement();
                                if(timenode.nodeName().contains("Hour", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(time.hour()));
                                    timenode.appendChild(timeText);
                                }
                                else if(timenode.nodeName().contains("Minute", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(time.minute()));
                                    timenode.appendChild(timeText);
                                }
                                else if(timenode.nodeName().contains("Second", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(time.second()));
                                    timenode.appendChild(timeText);
                                }
                            }
                        }
                        else if(dt.nodeName().contains("Date", Qt::CaseInsensitive))
                        {
                            QDomNodeList dateList = dt.childNodes();
                            for(int i = 0; i < dateList.count(); i++)
                            {
                                QDomElement datenode = dateList.at(i).toElement();
                                if(datenode.nodeName().contains("Year", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(date.year()));
                                    datenode.appendChild(timeText);
                                }
                                else if(datenode.nodeName().contains("Month", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(date.month()));
                                    datenode.appendChild(timeText);
                                }
                                else if(datenode.nodeName().contains("Day", Qt::CaseInsensitive))
                                {
                                    QDomText timeText = doc.createTextNode(QString::number(date.day()));
                                    datenode.appendChild(timeText);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return doc;
}

QDomDocument xml_creator::createGetDevInfoRes(QDomDocument doc)
{
    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode devInfoNode = ele.firstChildElement();
            QDomNodeList infoNodes = devInfoNode.childNodes();
            for(int i =0; i < infoNodes.count(); i++)
            {
                QDomNode infoNode = infoNodes.at(i).toElement();
                if(infoNode.nodeName().contains("Manufacturer", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("Paxton");
                    infoNode.appendChild(type);
                }
                else if(infoNode.nodeName().contains("Model", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("Net2 Entry Panel");
                    infoNode.appendChild(type);
                }
                else if(infoNode.nodeName().contains("FirmwareVersion", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("1.0");
                    infoNode.appendChild(type);
                }
                else if(infoNode.nodeName().contains("SerialNumber", Qt::CaseInsensitive))
                {
                    //TODO get from prop bag
                    QDomText type = doc.createTextNode("4085241");
                    infoNode.appendChild(type);
                }
                else if(infoNode.nodeName().contains("HardwareId", Qt::CaseInsensitive))
                {
                    //TODO get from prop bag
                    QDomText type = doc.createTextNode("1.0");
                    infoNode.appendChild(type);
                }
            }
        }
    }
    return doc;
}

QDomDocument xml_creator::createGetVideoSourcesRes(QDomDocument doc, QString width, QString height, QString framerate)
{
    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode vidCnfgNode = ele.firstChildElement();
            QDomNodeList vidCnfgNodes = vidCnfgNode.firstChildElement().childNodes();
            for(int i =0; i < vidCnfgNodes.count(); i++)
            {
                QDomNode configNode = vidCnfgNodes.at(i).toElement();                
                if(configNode.nodeName().contains("Framerate", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode(framerate);
                    configNode.appendChild(type);
                }
                else if(configNode.nodeName().contains("Resolution", Qt::CaseInsensitive))
                {
                    QDomNodeList resList = configNode.childNodes();

                    for(int i = 0; i < resList.count(); i++)
                    {
                        QDomNode resNode = resList.at(i).toElement();
                        if(resNode.nodeName().contains("Height", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(height);
                            resNode.appendChild(type);
                        }
                        else if(resNode.nodeName().contains("Width", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(width);
                            resNode.appendChild(type);
                        }
                    }
                }
            }
        }
    }
    return doc;
}

QDomDocument xml_creator::createGetDNSRes(QDomDocument doc)
{
    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode dnsInfoNode = ele.firstChildElement().firstChildElement();
            QDomNodeList dnsInfoNodes = dnsInfoNode.childNodes();
            for(int i =0; i < dnsInfoNodes.count(); i++)
            {
                QDomNode dnsNode = dnsInfoNodes.at(i).toElement();
                if(dnsNode.nodeName().contains("FromDHCP", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("true");
                    dnsNode.appendChild(type);
                }
                else if(dnsNode.nodeName().contains("SearchDomain", Qt::CaseInsensitive))
                {
                    QDomText type = doc.createTextNode("Paxton");
                    dnsNode.appendChild(type);
                }
                else if(dnsNode.nodeName().contains("DNSManual", Qt::CaseInsensitive))
                {
                    QDomNodeList dnsList = dnsNode.childNodes();

                    for(int i = 0; i < dnsList.count(); i++)
                    {
                        QDomNode dnsNode = dnsList.at(i).toElement();
                        if(dnsNode.nodeName().contains("Type", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode("IPv4");
                            dnsNode.appendChild(type);
                        }
                        else if(dnsNode.nodeName().contains("IPv4Address", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(ipv4addr);
                            dnsNode.appendChild(type);
                        }
                    }
                }
            }
        }
    }
    return doc;
}

QDomDocument xml_creator::createGetNetworkInterfacesRes(QDomDocument doc)
{
   QDomNode root = doc.firstChildElement();
   QDomNodeList rootchildren = root.childNodes();
   for(int i = 0; i < rootchildren.count(); i++)
   {
       QDomElement ele = rootchildren.at(i).toElement();

       if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
       {
           QDomNode interfaceNode = ele.firstChildElement().firstChildElement();
           QDomNodeList interfaceNodes = interfaceNode.childNodes();
           for(int i =0; i < interfaceNodes.count(); i++)
           {
               QDomNode interfaceNode = interfaceNodes.at(i).toElement();
               if(interfaceNode.nodeName().contains("Info", Qt::CaseInsensitive))
               {
                   QDomNodeList infoNodes = interfaceNode.childNodes();
                   for(int i = 0; i < infoNodes.count(); i++)
                   {
                       QDomNode hwNode = infoNodes.at(i).toElement();
                       if(hwNode.nodeName().contains("HwAddress", Qt::CaseInsensitive))
                       {
                           QDomText type = doc.createTextNode(hwAddr);
                           hwNode.appendChild(type);
                       }
                   }

               }
               else if(interfaceNode.nodeName().contains("IPv4", Qt::CaseInsensitive))
               {
                   QDomNodeList ipList = interfaceNode.childNodes();

                   for(int i = 0; i < ipList.count(); i++)
                   {
                       QDomNode ipNode = ipList.at(i).toElement();
                       if(ipNode.nodeName().contains("Config", Qt::CaseInsensitive))
                       {
                           QDomNodeList manualList = ipNode.firstChildElement().childNodes();
                           for(int i = 0; i < manualList.count(); i++)
                           {
                             QDomNode ipAddrNode = manualList.at(i).toElement();
                             if(ipAddrNode.nodeName().contains("Address", Qt::CaseInsensitive))
                             {
                                 QDomText type = doc.createTextNode(ipv4addr);
                                 ipAddrNode.appendChild(type);
                             }
                           }
                       }
                   }
               }
           }
       }
   }
   return doc;
}

QDomDocument xml_creator::createGetCapabilitiesRes(QDomDocument doc)
{
    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode capNode = ele.firstChildElement();
            QDomNodeList capNodes = capNode.firstChildElement().childNodes();
            for(int i =0; i < capNodes.count(); i++)
            {
                QDomNode capabilityNode = capNodes.at(i).toElement();
                if(capabilityNode.nodeName().contains("Analytics", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(analyticsService);
                            node.appendChild(type);
                        }
                    }

                }
                else if(capabilityNode.nodeName().contains("Device", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(deviceService);
                            node.appendChild(type);
                        }
                    }

                }
                else if(capabilityNode.nodeName().contains("Events", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(eventService);
                            node.appendChild(type);
                        }
                    }

                }
                else if(capabilityNode.nodeName().contains("Imaging", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(imagingService);
                            node.appendChild(type);
                        }
                    }

                }
                else if(capabilityNode.nodeName().contains("Media", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                        {
                            QDomText type = doc.createTextNode(mediaService);
                            node.appendChild(type);
                        }
                    }

                }
                else if(capabilityNode.nodeName().contains("Extension", Qt::CaseInsensitive))
                {
                    QDomNodeList capList = capabilityNode.childNodes();
                    for(int i = 0; i < capList.count(); i++)
                    {
                        QDomNode node = capList.at(i).toElement();
                        if(node.nodeName().contains("DeviceIO", Qt::CaseInsensitive))
                        {
                            QDomNodeList ioList = node.childNodes();
                            for(int i = 0; i < ioList.count(); i++)
                            {
                                QDomNode ioNode = ioList.at(i).toElement();
                                if(ioNode.nodeName().contains("XAddr", Qt::CaseInsensitive))
                                {
                                    QDomText type = doc.createTextNode(deviceIOService);
                                    ioNode.appendChild(type);

                                }
                            }
                        }
                        else if(node.nodeName().contains("Extensions", Qt::CaseInsensitive))
                        {
                            QDomNodeList ioList = node.childNodes();
                            for(int i = 0; i < ioList.count(); i++)
                            {
                                QDomNode ioNode = ioList.at(i).toElement();
                                if(ioNode.nodeName().contains("XAddr", Qt::CaseInsensitive))
                                {
                                    QDomText type = doc.createTextNode(telecomService);
                                    ioNode.appendChild(type);

                                }
                            }
                        }
                    }

                }
            }
        }
    }
    return doc;
}


QDomDocument xml_creator::createGetServicesRes(QDomDocument doc)
{
    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode serviceNode = ele.firstChildElement();
            QDomNodeList serviceNodes = serviceNode.childNodes();
            for(int i =0; i < serviceNodes.count(); i++)
            {
                QDomNode sNode = serviceNodes.at(i).toElement();
                QDomNodeList sList = sNode.childNodes();
                QString wsdl;
                QString service;
                if(i == DEVICE)
                {
                    service = deviceService;
                    wsdl = deviceWsdl;
                }
                else if(i == ANALYTICS)
                {
                    service = analyticsService;
                    wsdl = analyticsWsdl;
                }
                else if(i == EVENTS)
                {
                    service = eventService;
                    wsdl = eventWsdl;
                }
                else if(i == IMAGING)
                {
                    service = imagingService;
                    wsdl = imagingWsdl;
                }
                else if(i == MEDIA)
                {
                    service = mediaService;
                    wsdl = mediaWsdl;
                }
                else if(i == DEVICEIO)
                {
                    service = deviceIOService;
                    wsdl = deviceIOWsdl;
                }
                for(int i = 0; i < sList.count(); i++)
                {
                    QDomNode node = sList.at(i).toElement();
                    if(node.nodeName().contains("Namespace", Qt::CaseInsensitive))
                    {
                        QDomText type = doc.createTextNode(wsdl);
                        node.appendChild(type);
                    }
                    else if(node.nodeName().contains("XAddr", Qt::CaseInsensitive))
                    {
                        QDomText type = doc.createTextNode(service);
                        node.appendChild(type);
                    }
                }
            }
        }
    }

    return doc;
}

QDomDocument xml_creator::createGetStreamUriRes(QDomDocument doc)
{

    QDomNode root = doc.firstChildElement();
    QDomNodeList rootchildren = root.childNodes();
    for(int i = 0; i < rootchildren.count(); i++)
    {
        QDomElement ele = rootchildren.at(i).toElement();

        if(ele.nodeName().contains("Body", Qt::CaseInsensitive))
        {
            QDomNode mediaNode = ele.firstChildElement().firstChildElement();
            QDomNodeList mediaNodes = mediaNode.childNodes();
            for(int i =0; i < mediaNodes.count(); i++)
            {
                QDomNode uriNode = mediaNodes.at(i).toElement();
                if(uriNode.nodeName().contains("Uri", Qt::CaseInsensitive))
                {
                    QString rtsp = "rtsp://" + ipv4addr + ":8554/test";
                    QDomText type = doc.createTextNode(rtsp);
                    uriNode.appendChild(type);
                }
            }
        }
    }
    return doc;
}
