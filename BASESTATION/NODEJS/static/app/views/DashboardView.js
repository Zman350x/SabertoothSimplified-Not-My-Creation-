window.DashboardView = Backbone.View.extend({

    initialize:function () {
        console.log('Initializing Dashboard View');
        this.gauges = {};
    },

    render:function () {
      $(this.el).html(this.template());
      this.initGauges();
      return this;
    },

    initGauges: function() {
      this.gauges.gauge2 = new steelseries.Radial('gauge2', {
          gaugeType: steelseries.GaugeType.TYPE5,
          size: 201,
          //section: sections,
          //area: areas,
          titleString: 'Title',
          unitString: 'Type1',
          threshold: 50,
          lcdVisible: true
      });
      var sections = [
        steelseries.Section(20, 22, 'rgba(0, 0, 220, 0.3)'),
        steelseries.Section(22, 26, 'rgba(0, 220, 0, 0.3)'),
        steelseries.Section(26, 28, 'rgba(220, 220, 0, 0.3)')
      ];

      this.gauges.gaugeVoltage = new steelseries.RadialBargraph('gauge_voltage', {
          gaugeType: steelseries.GaugeType.TYPE2,
          size: 201,
          minValue: 20,
          maxValue: 28,
          section: sections,
          //useSectionColors: true,
          titleString: 'Voltage',
          unitString: 'V',
          lcdVisible: true
      });

      //gaugeVoltage.setMaxMeasuredValue(25);
      this.gauges.gaugeVoltage.setValueAnimated(24.5);
      return this;
    }

});
