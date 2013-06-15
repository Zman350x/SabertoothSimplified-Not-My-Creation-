/*global MAVLink*/
'use strict';
var express = require('express');
var app = express();
var server = app.listen(3000);
var io = require('socket.io').listen(server);
var serialport = require('serialport');
var SerialPort = serialport.SerialPort; // localize object constructor

require('./lib/mavlink_common_v1.0');
var mavlinkParser = new MAVLink();


var teleSerial = new SerialPort('/dev/cu.SLAB_USBtoUART', {
  baudrate: 19200,
  parser: serialport.parsers.raw
});

app.configure(function () {
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

app.configure('development', function () {
  app.use(express.errorHandler());
});

//app.get('/', routes.index);

console.log('Express server listening on port 3000');

teleSerial.on('data', function (chunk) {
  mavlinkParser.parseBuffer(chunk);
});



io.sockets.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });

  mavlinkParser.on('message', function (message) {
    if (message.name !== 'HEARTBEAT') {
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
});
