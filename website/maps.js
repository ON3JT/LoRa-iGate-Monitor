var mymap; 
var overlays;
var jsonDataCall;
var $table = $('#mainTable');
var MarkerGroup;
var PolylinesGroup;


	var IconBlip = L.icon({
    iconUrl: 'images/dotColor0.png',
    iconSize: [16, 16],
    //iconAnchor: [8, 16]
	});
	
	var myIcon = L.icon({
    iconUrl: 'images/antenna-green.png',
    iconSize: [26, 34],
    iconAnchor: [13, 34]
	});

	$('#datepicker').datepicker({
		
		format: 'yyyy-mm-dd',
		weekStart: 1,
		autoclose : true,
		todayBtn : 'linked',
		todayHighlight : true,
		 
		})
		.on('changeDate', function(e) {
		LoadData(e.format('yyyy-mm-dd'));
		
		});
	

function initMap() 
{
	mymap = L.map('mapid').setView([51.171414, 4.466801], 11);
	L.tileLayer('https://{s}.basemaps.cartocdn.com/rastertiles/voyager/{z}/{x}/{y}{r}.png', 
	{
		attribution:'&copy;  <a href=\"https://qrz.com/db/ON3JT\">Joost Luyten ON3JT</a> & <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>',
		zoomSnap: 0.25,
	}).addTo(mymap);
	
	// iGate Marker
	L.marker([51.171414, 4.466801], {title : "iGate location",icon: myIcon}).addTo(mymap);
	
	L.control.scale().addTo(mymap);
	
	// Create layer for markers and add it to the map
	MarkerGroup = L.layerGroup().addTo(mymap);
	PolylinesGroup = L.layerGroup().addTo(mymap);

    LoadData((new Date()).toISOString().split('T')[0]);
}



function LoadData(iGateDate)
{
	// remove all the markers in one go
	MarkerGroup.clearLayers();
	PolylinesGroup.clearLayers();
	
	$.getJSON('json.php?date='+iGateDate,function(result) {
		jsonDataCall = result;
		
		$.each(jsonDataCall, function(key,data) 
		{
			
			marker = new L.marker([data.latitude,data.longitude],{title : data.call+' '+data.time, icon : IconBlip})
				.bindPopup(	'<b>'+ data.call + '</b><br/>' +
							'<br/><b>Time (UTC): </b>' + data.time +
							'<br/><b>RSSI:</b> ' + data.rssi + ' dBm' +
							'<br/><b>Signal:</b> ' + data.signal + ' <br/>' +
							'<br/><b>TX distance:</b> ' + data.distance + ' km'
		      ,{autoPan:true,maxHeight:250})
			  .addTo(MarkerGroup);
			var latlngs = [
				[51.171414, 4.466801],
				[data.latitude, data.longitude],
				];
			 L.polyline(latlngs, {color: 'red', opacity: 0.1}).addTo(PolylinesGroup);
		});
		
		 
		$table.bootstrapTable({
				data: jsonDataCall});
				
		$table.bootstrapTable('load', jsonDataCall)		;
	});


	$('#datepicker').datepicker('update', '');

	mymap.invalidateSize();
}



function returnQth(lat, lng) 
{
		var qth = '';
		lat += 90; 
		lng += 180;
		
		lat = lat / 10 + 0.0000001;
		lng = lng / 20 + 0.0000001;

		qth += chr(65 + lng) + chr(65 + lat);

		lat = 10 * (lat - floor(lat));
		lng = 10 * (lng - floor(lng));

		qth += chr(48 + lng) + chr(48 + lat);

		lat = 24 * (lat - floor(lat));
		lng = 24 * (lng - floor(lng));

		qth += chr(65 + lng) + chr(65 + lat);

		lat = 10 * (lat - floor(lat));
		lng = 10 * (lng - floor(lng));

		qth += chr(48 + lng) + chr(48 + lat);

		lat = 24 * (lat - floor(lat));
		lng = 24 * (lng - floor(lng));

		qth += chr(65 + lng) + chr(65 + lat);
		
		return qth;
}

function chr(x)
{
return String.fromCharCode(x);
}

function floor(x)
{
	return Math.floor(x);
}