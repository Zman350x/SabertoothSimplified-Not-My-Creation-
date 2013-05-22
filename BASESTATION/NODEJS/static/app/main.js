window.Router = Backbone.Router.extend({
  routes: {
    '': 'home',
    'map': 'map'
  },

  initialize: function () {
    console.log("BB INIT");

    this.headerBar = new HeaderBar();
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

      socket.on('voltage', function(data){
        console.log(data);
        console.log(app.dashboardView.gauges.gaugeVoltage.setValueAnimated(data.value));
      })
  });
});
