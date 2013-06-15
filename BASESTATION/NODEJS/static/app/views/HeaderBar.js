/*global Backbone*/
'use strict';

var HeaderBar = Backbone.View.extend({
  el: 'ul',
  className: 'nav',

  initialize: function () {
    this.setActive('menu-dashboard');
  },

  setActive: function (ident) {
    $('#mainmenu li').removeClass('active');
    $('#mainmenu li.' + ident).addClass('active');
  }

});
