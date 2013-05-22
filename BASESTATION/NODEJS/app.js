var express = require('express')
  , http = require('http');
var app = express();
var server = app.listen(3000);
var io = require('socket.io').listen(server);

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

io.sockets.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('my other event', function (data) {
    console.log(data);
  });

  setInterval(function(){
    var voltage = randomData(20,28);
    socket.emit('voltage', { value: voltage });
  }, 3000);

});



function randomData(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}
