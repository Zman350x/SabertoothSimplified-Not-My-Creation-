window.Router = Backbone.Router.extend({
  routes: {
    '': 'home',
    'map': 'map'
  },

  initialize: function () {
    console.log("BB INIT");
    this.socket = io.connect('http://localhost');
    this.socket.on('news', function (data) {
      console.log(data);
      this.socket.emit('my other event', { my: 'data' });
    });

    this.socket.on('mavlinkdata', this.onMavlinkData);
    this.headerBar = new HeaderBar();
  },

  onMavlinkData: function(data) {
    console.log('onMavlinkData',data);
    switch(data.name) {
      case 'VOLTAGECURRENT': 
        app.dashboardView.gauges.gaugeVoltage.setValueAnimated(data.voltage/100);
        app.dashboardView.gauges.gaugeAmpLeft.setValueAnimated(data.cur_left/100);
        app.dashboardView.gauges.gaugeAmpRight.setValueAnimated(data.cur_right/100);
        app.dashboardView.gauges.gaugeAmpCombined.setValueAnimated((data.cur_left/100)+(data.cur_right/100));
      break;
      case 'DHT11_DATA':
        app.dashboardView.gauges.gaugeTemp.setValueAnimated(data.temperature_celsius);
        app.dashboardView.gauges.gaugeHum.setValueAnimated(data.humidity_percent);
      break;
    }

  },

  home: function() {
    console.log("home");
    this.headerBar.setActive('menu-dashboard');
    if (!this.dashboardView) {
      this.dashboardView = new DashboardView();
    }
    $('#content').html(this.dashboardView.el);
      this.dashboardView.render();

  },

  map: function() {
    console.log("map");
    this.headerBar.setActive('menu-map');
    if (!this.mapView) {
      this.mapView = new MapView();
      this.mapView.render();
    }
    $('#content').html(this.mapView.el);
  }
});

/*
$(function() {
  app = new Router();
  Backbone.history.start();
});
*/

var templates = [
  'MapView',
  'DashboardView'
];
$(function() {
  templateLoader.load(templates, function () {
    app = new Router();
    Backbone.history.start();
  });
});
