/**
 * Created by chenhao on 15/7/20.
 */

index = 0;
img_index = 0;

//_imgs = ["/imgs/cover.png", "/imgs/mcookie.jpg", "/imgs/mic_light.png", "/imgs/music_box.png"];
_imgs = ["/imgs/cover.png"];

_data = [
    {
        "title": "<strong> Control IOT Devices </strong>",
        "description": "Push data or pull control via API of RESTful, WebSocket, MQTT, CoAP, ...",
        "buttons":[
            {label:"microduino", href:"https://www.microduino.cc/module"},
            {label:"mCookie", href:"https://www.microduino.cc/module?type=mCookie"},
            {label:"Arduino", href:"https://www.arduino.cc"}
        ]
    },
    {}, {
        "title": "<strong> Awesome Projects </strong>",
        "description": "Weather Station, Color LED, My City, Smart Egg",
        "buttons":[
            {label:"Projects ...", href:"/projects"}
        ]
    },
    {}, {
        "title": "<strong> Open Source Software </strong>",
        "description": "We built full stack open sources software for open sources hardware",
        "buttons":[
            {label:"Fork from Github", href:"https://github.com/iascchen/mCotton"}
        ]
    },
    {}, {
        "title": "<strong> Wireless Connection </strong>",
        "description": "Via BLE 4, Wifi, Zigbee"
    },
    {}, {
        "title": "<strong> Android, iOS, HTML5 </strong>",
        "description": "Control devices via mobile apps or browser. provides sample code in iOS Swift, Android, and H5"
    },
    {}, {
        "title": "<strong> In Javascript </strong>",
        "description": "All platform built in Javascript, includes clouds, gateways, even in embedded devices"
    },
    {}, {
        "title": "<strong> Open Data </strong>",
        "description": "Share your data to public, and dump it as your wish."
    },
    {}, {
        "title": "<strong> Data Visualization </strong>",
        "description": "Data visualized by SVG, D3.js, WebGL, GIS, ...",
        "buttons":[
            {label:"Devices Map", href:"/pdmap"}
        ]
    }
];

Template.home.helpers({
    ultraVisualData: function () {
        return _data;
    }
});

//Template.home.rendered = function () {
//    var s = skrollr.init();
//};

Template.scroll_item.helpers({
    background: function () {
        var img_index2 = (img_index + 1) % _imgs.length;
        return _imgs[img_index2];
    }
});