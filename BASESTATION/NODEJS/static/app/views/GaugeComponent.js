window.GaugeComponent = Backbone.View.extend({

  initialize: function () {
    console.log(this.options);
    console.log('Initializing Gauge View');
    this.gauges = {};
  },

  render: function () {
    $(this.el).html(this.template());
    this.initGauges();
    return this;
  }
});
