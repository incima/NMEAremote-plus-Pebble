Pebble.addEventListener("ready",
  function(e) {
		var timerID = setInterval(reload,1000);		
		function reload() {
			var req = new XMLHttpRequest();
			req.open('GET', 'http://192.168.1.15:8080/json', true);
			req.onload = function(e) {
				console.log("Hello " + req.readyState + ":" + req.status);
				if (req.readyState == 4) {
			  	if(req.status == 200) {
			       var response = JSON.parse(req.responseText);
			       var speed = response.SOG;
			       var hdg = response.HDG;		   
			       var awa = response.AWA;
						 console.log("SPEED: " + speed + " HDG: " + hdg + " AWA: " + awa);
			       Pebble.sendAppMessage({ "SPEED":"SOG "+speed, "HDG":"HDG "+hdg, "AWA":"AWA "+awa });		   
			     } else { 
						 console.log("Error"); 
			       Pebble.sendAppMessage({ "SPEED":"SOG --", "HDG":"HDG --", "AWA":"AWA --" });		   						 
				 	 }
			   }
			 }
			 req.send(null);
	 	}
  }
);