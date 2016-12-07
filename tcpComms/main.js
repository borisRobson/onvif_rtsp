var http = require('http');
var parser = require('./parser');
var fs = require('fs');
//var xml2js = require('xml2js');
//var xmlParse = new xml2js.Parser();
const xml2js = require('xml2js')
  , numberRE = /^-?([1-9]\d*|0)(\.\d*)?$/
  , dateRE = /^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(.\d+)?Z$/
  , prefixMatch = /(?!xmlns)^.*:/
  ;

function parseReq(body){
	var method = parser.parseBody(body);
  return method;
}

Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

const linerase = function(xml) {
  if (Array.isArray(xml)) {
    if (xml.length > 1) {
      return xml.map(linerase);
    } else {
      xml = xml[0];
    }
  }
  if (typeof xml === 'object') {
    var obj = {};
    Object.keys(xml).forEach(function(key) {
      if (key === '$') {
        obj.$ = linerase(xml.$);
      } else {
        obj[key] = linerase(xml[key]);
      }
    });
    return obj;
  } else {
    if (xml === 'true') { return true; }
    if (xml === 'false') { return false; }
    if (numberRE.test(xml)) { return parseFloat(xml); }
    if (dateRE.test(xml)) { return new Date(xml); }
    return xml;
  }
};

http.createServer(function(request, response) {
  var headers = request.headers;
  var method = request.method;
  var url = request.url;
  var body = [];
  request.on('error', function(err) {
    console.error(err);
  }).on('data', function(chunk) {
    body.push(chunk);
  }).on('end', function() {
    body = Buffer.concat(body).toString();
    // BEGINNING OF NEW STUFF
    var filePath = "";
    filePath = parseReq(body);
    console.log(filePath);

    body = [];
    response.on('error', function(err) {
      console.error(err);
    });

 
    try {
        var fs = require('fs');
        var xml2js = require('xml2js');
        var json;
        var fileData = fs.readFileSync(filePath, 'utf-8');        
        body = linerase(fileData);
    }catch (ex) {console.log(ex)}


    // Note: the 2 lines above could be replaced with this next one:
    // response.writeHead(200, {'Content-Type': 'application/json'})
    
    var data = JSON.stringify(body);    
    data = data.replace(/"</g,"<");
    data = data.replace(/>"/g,">");    
    data = data.replace(/(\\)/g,"");
    //console.log((data));

    
    var len = Object.size(data);
    //console.log(len);
    
    response.statusCode = 200;    
    response.setHeader('Content-Type', 'application/soap+xml;charset=utf-8');    
    response.setHeader('Content-Length', len);
    response.setHeader('Connection','close');
    
  

    response.write(data);
    response.end();
    // Note: the 2 lines above could be replaced with this next one:
    // response.end(JSON.stringify(responseBody))

    // END OF NEW STUFF
  });
}).listen(80);
