const xml2js = require('xml2js')
  , numberRE = /^-?([1-9]\d*|0)(\.\d*)?$/
  , dateRE = /^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(.\d+)?Z$/
  , prefixMatch = /(?!xmlns)^.*:/
  ;

/*setup method names*/
const  getSysDT = "getSystemDateAndTime";
const  getDNS = "getDNS";
const  getScopes = "getScopes";
const  getDevInfo = "getDeviceInformation";
const  getNtwkInts = "getNetworkInterfaces";
const  getCaps = "getCapabilities";
const  getServices = "getServices";
const  getVidSrc = "getVideoSources";
const getVidSrcConfig = "getVideoSourceConfiguration"
const  getProfiles = "getProfiles";
const  getProfile = "getProfile";
const  getSnapshot = "getSnapshotUri";
const  getStream = "getStreamUri";



var parseBody = function(data){

    var reqAction = "";
    var printData = function( err, result, xml){
      var req = JSON.stringify(result[0])
      var j = JSON.parse(req);
      var method = "";
      for (key in j) {
        if (j.hasOwnProperty(key)) {
            method = key.toString();                 
        }
      } 

      if (method == (getSysDT)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getSysDateTimeRes.xml");
      }
      else if(method == (getDNS)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getDNSRes.xml");
      }
      else if(method == (getScopes)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getScopesRes.xml");
      }
      else if(method == (getDevInfo)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getDevInfoRes.xml");
      }
      else if(method == (getNtwkInts)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getNetworkInterfacesRes.xml");
      }
      else if(method == (getCaps)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getCapabilitiesRes.xml");
      }
      else if(method == (getScopes)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getScopesRes.xml");
      }
      else if(method == (getCaps)){
        return("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getCapabilitiesRes.xml");
      }
      else if(method == (getVidSrc)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getVideoSourcesRes.xml")
      }
      else if(method == (getServices)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getServicesRes.xml")
      }
      else if(method == (getProfiles)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getProfilesRes.xml")
      }
      else if(method == (getProfile)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getProfileRes.xml")
      }
      else if(method == (getSnapshot)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getSnapshotRes.xml")
      }
      else if(method == (getStream)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getStreamUriRes.xml")
      }
      else if(method == (getVidSrcConfig)){
        return ("/home/standby/doorentry/apps/qtapps/onvif-rtsp/xmlFiles/getVideoSourceConfigurationRes.xml")
      }
      
      else{
        return("Unkown message: " + method);
      }
    };


  	const parseSOAPString = function(xml, callback) {
    /* Filter out xml name spaces */
    xml = xml.replace(/xmlns(.*?)=(".*?")/g,'');

    xml2js.parseString(
      xml
      , {
        tagNameProcessors: [function(str) {
          str = str.replace(prefixMatch, '');
          var secondLetter = str.charAt(1);
          if (secondLetter && secondLetter.toUpperCase() !== secondLetter) {
            return str.charAt(0).toLowerCase() + str.slice(1);
          } else {
            return str;
          }
        }]
      }
      , function(err, result) {
        if (!result || !result['envelope'] || !result['envelope']['body']) {
          callback(new Error('Wrong ONVIF SOAP response'), null, xml);
        } else {
          if (!err && result['envelope']['body'][0]['fault']) {
            err = new Error(
              'ONVIF SOAP Fault: ' +
              (
                result.envelope.body[0].fault[0].reason[0].text[0]._
                ||
                JSON.stringify(linerase(result.envelope.body[0].fault[0].code[0]))
              ) +
              (
                result.envelope.body[0].fault[0].detail
                ? ': ' + result.envelope.body[0].fault[0].detail[0].text[0]
                : ''
              )
            );
          }
          
          reqAction = callback(err, result['envelope']['body'], xml);          

        }
      });
  };

  parseSOAPString(data, printData);  
  return reqAction;
}

module.exports = {
  parseBody: parseBody
};
