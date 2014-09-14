#include <math.h>

#include "format.h"

size_t format_seconds(time_t v, char *time_str, size_t len) 
{
	int dd = (int)floor(v / (60 * 60 * 24));
	int div_days = v % (60 * 60 * 24);
	int hh = (int)floor(div_days / (60 * 60));
	int div_min = div_days % (60 * 60);
	int mm = (int)floor(div_min / 60);
	int div_sec = div_min % 60;
	int ss = (int)div_sec;
		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "format_seconds: %i:%i:%i:%i", dd, hh, mm, ss);
	
	unsigned int d = abs(dd);
	unsigned int h = abs(hh);
	unsigned int m = abs(mm);
	unsigned int s = abs(ss);
		
	size_t c = 0;	
	if (d > 0) {			
		c = snprintf(&time_str[c], len-c, "%02u:%02u", d, h);				
	} 
	else if (h > 0) {		
		c = snprintf(&time_str[c], len-c, "%02u:%02u", h, m);		
	} 
	else {	
		c = snprintf(time_str + c, len-c, "%02u:%02u", m, s);				
	}
	return c;
}
