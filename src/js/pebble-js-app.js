function formatSMile(v) {
	if(!v || v == "") return "--.--";
	return v.toFixed(2);
}

function formatKnots(v) {
	if(!v || v == "") return "--.-";
	return v.toFixed(1);
}

function formatBft(v) {
	if(!v || v == "") return "-";
	return v.toFixed(0);	
}

function formatAngle(v) {
	if(!v || v == "") return "---°";
	var x = v.toFixed(0);
	if (x < -100) 
		return '-'+ x*-1 +"°";
	if (x < -10) 
		return '-0'+ x*-1 +"°";
	if (x < 0)	
		return '-00'+ x*-1 +"°";	
	if (x < 10) 
		return '00'+ x +"°";
	else if (x < 100) 
		return '0'+ x +"°";		
	else
		return x + '°';
}

function formatTime(v) {
	if(!v || v == "") return "--:--";
	
	var d = Math.floor(v / (60 * 60 * 24));
	var div_days = v % (60 * 60 * 24);
	var h = Math.floor(div_days / (60 * 60));
	var div_min = div_days % (60 * 60);
	var m = Math.floor(div_min / 60);
	var div_sec = div_min % 60;
	var sec = Math.ceil(div_sec);
	
	var time_str = "";
	if (d > 0) {
		if (d < 10) 
			time_str += '0';
		time_str += d.toFixed(0);
		time_str += ':';
		if (h < 10)
				time_str += '0';
		time_str += h.toFixed(0);		
		return time_str;
	} else if (h > 0) {
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
			req.open('GET', e.payload.URL.trim() + "?SPEED,DEPTH,HDG,AWA,BTW,DTW,TTG,COG,XTE,SOG,TWD,TWS,BFT,TARGET_SPEED,TARGET_SPEED_PERCENT,STARTTIME_INTERVAL1970", true);
			req.onload = function(e) {
				if (req.readyState == 4) {
			  	if(req.status == 200) {
			       var response = JSON.parse(req.responseText);
						 
			       var speed = formatKnots(response.SPEED);
			       var depth = formatMeter(response.DEPTH);							 
			       var hdg = formatAngle(response.HDG);		   
			       var awa = formatAngle(response.AWA);
			       var btw = formatAngle(response.BTW);
			       var dtw = formatSMile(response.DTW);		   
			       var ttg = formatTime(response.TTG);		
						 var cog = formatAngle(response.COG);
						 var xte = formatKnots(response.XTE);
						 var sog = formatKnots(response.SOG);
						 var twd = formatAngle(response.TWD);
						 var tws = formatKnots(response.TWS);
						 var bft = formatBft(response.BFT);
						 var target_speed = formatKnots(response.TARGET_SPEED);
						 var target_speed_percent = formatPercent(response.TARGET_SPEED_PERCENT);					
						 var starttime_interval1970 = formatPercent(response.STARTTIME_INTERVAL1970);			
						 								 	 						 
			       Pebble.sendAppMessage({"SPEED":speed, 
						 											 	"DEPTH":depth,
						 												"HDG":hdg, 
																		"AWA":awa,
																		"BTW":btw,
																		"DTW":dtw,
																		"TTG":ttg,
																		"COG":cog,
																		"XTE":xte,
																		"SOG":sog,			
																		"TWD":twd,
																		"TWS":tws,
																		"BFT":bft,																																	
																		"TARGET_SPEED":target_speed,
																		"TARGET_SPEED_PERCENT":target_speed_percent,
																		"STARTTIME_INTERVAL1970": starttime_interval1970
																	});		   
			     } 
			   }
			 }
			 req.send(null);
	 	}			
	}
});