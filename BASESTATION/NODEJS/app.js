var express = require('express')
  , http = require('http');
var app = express();
var server = app.listen(3000);
var io = require('socket.io').listen(server);
var serialport = require("serialport");
var SerialPort = serialport.SerialPort; // localize object constructor

var mavlink = require('./lib/mavlink_common_v1.0');
var mavlinkParser = new MAVLink();


var teleSerial = new SerialPort('/dev/cu.SLAB_USBtoUART', { 
    //baudrate: 115200
    baudrate: 19200
    ,parser: serialport.parsers.raw
    //,parser: serialport.parsers.readline(0xaa+0xff) 
});
app.configure(function(){
  app.set('views', __dirname + '/views');
  app.set('view engine', 'jade');
  app.use(express.favicon());
  app.use(express.logger('dev'));
  //app.use(express.logger);
  app.use(express.static(__dirname + '/static'));
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
});

app.configure('development', function(){
  app.use(express.errorHandler());
});

//app.get('/', routes.index);

console.log("Express server listening on port 3000");

teleSerial.on("data",function(chunk){
  mavlinkParser.parseBuffer(chunk); 
  //console.log(chunk);
});



io.sockets.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });

  mavlinkParser.on('message', function(message) {
    if (message.name !== "HEARTBEAT") {
      delete(message.msgbuf);
      delete(message.payload);
      delete(message.crc);
      delete(message.crc_extra);
      delete(message.format);
      socket.emit('mavlinkdata', message);
    }
    /*
    switch(message.name) {
      case "VOLTAGECURRENT":
        socket.emit('mavlinkdata', message);
      break;
    }
    */
  });
  /*
  setInterval(function(){
    var voltage = randomData(20,28,2);
    var amp_left = randomData(-10,30,1);
    socket.emit('mavlinkdata', {
      voltage: randomData(20,28,2),
      amp_combined: randomData(-10,35,2),
      amp_left: amp_left,
      amp_right: amp_left + randomData(-10,10,0),
    });
  }, 1000);
  */
});



function randomData(min, max, precise) {
  if (!precise)
    precise = 0;
  var number = (Math.random() * (max - min + 1)) + min;

  return number.toFixed(precise);
}
