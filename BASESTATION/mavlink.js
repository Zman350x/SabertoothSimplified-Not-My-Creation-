var serialport = require("serialport");
var SerialPort = serialport.SerialPort; // localize object constructor

var mavlink = require('mavlink_common_v1.0');
var mavlinkParser = new MAVLink();


var teleSerial = new SerialPort('/dev/cu.SLAB_USBtoUART', { 
    //baudrate: 115200
    baudrate: 19200
    ,parser: serialport.parsers.raw
    //,parser: serialport.parsers.readline(0xaa+0xff) 
});

var step = 0;

var test1;
var test2;

var xor = 0;

teleSerial.on("data",function(chunk){
  mavlinkParser.parseBuffer(chunk); 
  //console.log(chunk);
});

mavlinkParser.on('message', function(message) {
  console.log('Got a message of any type!');
  console.log(message);
});
