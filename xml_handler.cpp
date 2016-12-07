#include "xml_handler.h"

xml_handler::xml_handler()
{
    filepath =  "./tcpComms/xmltemplates/";    

    //load defalut values
    width = "640";
    height = "480";
    framerate = "15";
    emit updateVidSrcConfig();

    create_getDevInfoRes();
    create_getScopes();
    create_getVidSourcesRes();
    create_getDNSRes();
    create_getNtwkInterfacesRes();
    create_getCapabilitiesRes();
    create_getServicesRes();
    create_getProfilesRes();
    create_getSnapshotRes();
    create_getProfileRes();
    create_getStreamUriRes();
    create_getVideoSourceConfigRes();
}

void xml_handler::create_getVideoSourceConfigRes()
{
    QDomDocument doc = loadFile(QString("getVideoSourceConfigurationRes.xml"));

    writeFile(doc,"getVideoSourceConfigurationRes.xml" );
}

void xml_handler::create_getStreamUriRes()
{
    QDomDocument doc = loadFile(QString("getStreamUriRes.xml"));

    QDomDocument getUriRes = xmlcreator.createGetStreamUriRes(doc);

    writeFile(getUriRes,"getStreamUriRes.xml" );
}

void xml_handler::create_getProfileRes()
{
    QDomDocument doc = loadFile(QString("getProfileRes.xml"));

    writeFile(doc,"getProfileRes.xml" );
}

void xml_handler::create_getSnapshotRes()
{
    QDomDocument doc = loadFile(QString("getSnapshotRes.xml"));

    writeFile(doc,"getSnapshotRes.xml" );
}

void xml_handler::create_getProfilesRes()
{
    QDomDocument doc = loadFile(QString("getProfilesRes.xml"));

    writeFile(doc,"getProfilesRes.xml" );
}

void xml_handler::create_getServicesRes()
{
    QDomDocument doc = loadFile(QString("getServicesRes.xml"));

    QDomDocument servicesDoc = xmlcreator.createGetServicesRes(doc);

    writeFile(servicesDoc,"getServicesRes.xml" );
}

void xml_handler::create_getCapabilitiesRes()
{
    QDomDocument doc = loadFile(QString("getCapabilitiesRes.xml"));

    QDomDocument ntwkIntDoc = xmlcreator.createGetCapabilitiesRes(doc);

    writeFile(ntwkIntDoc,"getCapabilitiesRes.xml" );
}

void xml_handler::create_getNtwkInterfacesRes()
{
    QDomDocument doc = loadFile(QString("getNetworkInterfacesRes.xml"));

    QDomDocument ntwkIntDoc = xmlcreator.createGetNetworkInterfacesRes(doc);

    writeFile(ntwkIntDoc,"getNetworkInterfacesRes.xml" );
}

void xml_handler::create_getScopes()
{
    QDomDocument doc = loadFile(QString("getScopesRes.xml"));

    writeFile(doc,"getScopesRes.xml" );
}

void xml_handler::create_getSysDTRes()
{
    QDomDocument doc = loadFile(QString("getSysDateTimeRes.xml"));

    QDomDocument sysDTDoc = xmlcreator.createGetSysDTRes(doc);

    writeFile(sysDTDoc,"getSysDateTimeRes.xml" );
}

void xml_handler::create_getDevInfoRes()
{
    QDomDocument doc = loadFile(QString("getDevInfoRes.xml"));

    QDomDocument devInfoDoc = xmlcreator.createGetDevInfoRes(doc);

    writeFile(devInfoDoc,"getDevInfoRes.xml" );
}

void xml_handler::create_getVidSourcesRes()
{
    QDomDocument doc = loadFile(QString("getVideoSourcesRes.xml"));

    QDomDocument vidSrcConfigDoc = xmlcreator.createGetVideoSourcesRes(doc, width, height, framerate);
    writeFile(vidSrcConfigDoc,"getVideoSourcesRes.xml");

}

void xml_handler::create_getDNSRes()
{
    QDomDocument doc = loadFile(QString("getDNSRes.xml"));

    QDomDocument devInfoDoc = xmlcreator.createGetDNSRes(doc);

    writeFile(devInfoDoc,"getDNSRes.xml" );
}


QDomDocument xml_handler::loadFile(QString filename)
{
    qDebug() << __FUNCTION__;
    QString file = filepath + (QString(filename));
    QFile tempfile(file);

    QDomDocument doc;
    if(!tempfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "could not open template";
    }else{
        doc.setContent(&tempfile);
        tempfile.close();
    }
    return doc;
}



void xml_handler::writeFile(QDomDocument doc, QString filename)
{
    QString prefix;
#ifdef IMX6
    prefix = "/nvdata/tftpboot/xmlFiles/";
#else
    prefix = "./xmlFiles/";
#endif
    QString file = prefix + filename;
    QFile xmlfile(file);
    if(!xmlfile.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
    {
        qDebug() << "Failed to write file";
    }

    QTextStream stream(&xmlfile);
    stream.setCodec("UTF-8");
    QString fstring(doc.toString());
    fstring = fstring.simplified();
    fstring.replace("> ",">");
    fstring.replace("\n", "");
    stream << fstring;
    xmlfile.close();
}

void xml_handler::storeVidSrcCnfg( QString configWidth, QString configHeight, QString configFramerate)
{
    width = configWidth;
    height = configHeight;
    framerate = configFramerate;
    create_getVidSourcesRes();
}
