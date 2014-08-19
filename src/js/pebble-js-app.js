function formatSMile(v) {
	if(!v || v == "") return "--.-";
	return v.toFixed(2);
}

function formatKnots(v) {
	if(!v || v == "") return "--.-";
	return v.toFixed(1);
}

function formatAngle(v) {
	if(!v || v == "") return "---°";
	var x = v.toFixed(0);
	if (x < 10) 
		return '00'+ x +"°";
	else if (x < 100) 
		return '0'+ x +"°";		
	else
		return x + '°';
}

function formatTime(v) {
	if(!v || v == "") return "--:--";
	return v.toFixed(1);
}

function formatMeter(v) {
	if(!v || v == "") return "--.-";
	return v.toFixed(1);
}

function formatPercent(v) {
	if(!v || v == "") return "---%";
	return v.toFixed(1);	
}

Pebble.addEventListener("appmessage",
function(e) {		
  console.log("Received message: " + JSON.stringify(e.payload));
	if (e.payload.URL) {
		console.log("URL: " + e.payload.URL);
		var timerID = setInterval(reload,1000);		
		function reload() {
			var req = new XMLHttpRequest();
			req.open('GET', e.payload.URL, true);
			req.onload = function(e) {
				if (req.readyState == 4) {
			  	if(req.status == 200) {
			       var response = JSON.parse(req.responseText);
			       var speed = formatKnots(response.Speed);
			       var depth = formatMeter(response.Depth);							 
			       var hdg = formatAngle(response.HDG);		   
			       var awa = formatAngle(response.AWA);
			       var btw = formatAngle(response.BTW);
			       var dtw = formatSMile(response.DTW);		   
			       var ttg = formatTime(response.TTG);		
						 var cog = formatAngle(response.COG);
						 var xte = formatKnots(response.XTE);
						 var sog = formatKnots(response.SOG);
						 var target_speed = formatKnots(response.TARGET_SPEED);
						 var target_speed_percent = formatPercent(response.TARGET_SPEED_PERCENT);						 						 
			       Pebble.sendAppMessage({"Speed":speed, 
						 											 	"Depth":depth,
						 												"HDG":hdg, 
																		"AWA":awa,
																		"BTW":btw,
																		"DTW":dtw,
																		"TTG":ttg,
																		"COG":cog,
																		"XTE":xte,
																		"TARGET_SPEED":target_speed,
																		"TARGET_SPEED_PERCENT":target_speed_percent,
																		"SOG":sog
																	});		   
			     } 
			   }
			 }
			 req.send(null);
	 	}			
	}
});