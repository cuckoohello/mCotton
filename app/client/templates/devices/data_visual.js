/**
 * Created by chenhao on 15/4/14.
 */

// Meteor.subscribe("dataevents", Meteor.userId());

Template.dataVisual.helpers({
    dataVisualTemplate: function () {
        // console.log("dataVisualTemplate", this);

        var chart = "LINE";
        var project = Collections.Projects.findOne( {_id: this.project_id} );
        if (project) {
            chart = project.show_chart;
        }
        // console.log("dataVisualTemplate", project.show_chart);

        switch (chart) {
            case "PIE" :
                return Template.dataVisualPie;
            case "BAR" :
                return Template.dataVisualBar;
            case "MY_CITY" :
                return Template.dataVisualMyCity;
            case "LINE" :
                return Template.dataVisualLine;
            case "EGG" :
                return Template.dataVisualEgg;

            case "MPH" :
                return Template.dataVisualMapHeatmap;
            case "MPM" :
                return Template.dataVisualMapMarkers;
            case "MPP" :
                return Template.dataVisualMapPoint;
            case "MPT" :
                return Template.dataVisualMapTrace;
        }
        return Template.dataVisualLine;
    }
});

Template.dataVisual.events({
    'click .raw-data':function(e,tmpl) {
        //e.preventDefault();
        //$('html, body').animate({
        //    scrollTop: $("#item_id").offset().top
        //}, 600);
    }
});