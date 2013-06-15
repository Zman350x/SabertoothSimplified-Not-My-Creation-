/*global Backbone*/
'use strict';
window.MapView = Backbone.View.extend({

  initialize: function () {
    console.log('Initializing Map View');
  },

  render: function () {
    $(this.el).html(this.template());
    return this;
  }
});
