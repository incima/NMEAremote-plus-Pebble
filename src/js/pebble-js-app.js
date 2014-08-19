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
	var h = Math.floor(v / (60 * 60));
	var div_min = v % (60 * 60);
	var m = Math.floor(div_min / 60);
	var div_sec = div_min % 60;
	var sec = Math.ceil(div_sec);
	
	var time_str = "";
	if (h > 0) {
		if (h < 10) 
			time_str += '0';
		time_str += h.toFixed(0);
		time_str += ':';
		if (m < 10)
				time_str += '0';
		time_str += m.toFixed(0);		
		return time_str;
	} else {
		if (m < 10)
				time_str += '0';
		time_str += m.toFixed(0);				
		time_str += ':';
		if (s < 10)
				time_str += '0';
		time_str += s.toFixed(0);		
		return time_str;
	}	
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
																		"SOG":sog,																		
																		"TARGET_SPEED":target_speed,
																		"TARGET_SPEED_PERCENT":target_speed_percent
																	});		   
			     } 
			   }
			 }
			 req.send(null);
	 	}			
	}
});