function formatSMile(v) {
	if(!v) return "-.--";
	return v.toFixed(1);
}

function formatAngle(v) {
	if(!v) return "---°";
	var x = v.toFixed(0);
	if (x < 100) 
		return '0'+ x +"°";
	else
		return x + '°';
}

Pebble.addEventListener("ready",
  function(e) {
		var timerID = setInterval(reload,1000);		
		function reload() {
			var req = new XMLHttpRequest();
			req.open('GET', 'http://192.168.1.15:8080/json', true);
			req.onload = function(e) {
				if (req.readyState == 4) {
			  	if(req.status == 200) {
			       var response = JSON.parse(req.responseText);
			       var speed = formatSMile(response.Speed);
			       var hdg = formatAngle(response.HDG);		   
			       var awa = formatAngle(response.AWA);
			       Pebble.sendAppMessage({"Speed":speed, 
						 												"HDG":hdg, 
																		"AWA":awa 
																	});		   
			     } else { 
						 console.log("Error"); 
			       Pebble.sendAppMessage({ "Speed":"--", 
						 												 "HDG":"--", 
																		 "AWA":"--" 
																	 });		   						 
				 	 }
			   }
			 }
			 req.send(null);
	 	}
  }
);