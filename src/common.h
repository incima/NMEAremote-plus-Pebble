#ifndef COMMON_H_
#define COMMON_H_

#include <pebble.h>

#define FONT_OPENSANS_EXTRA_48_NUMBERS RESOURCE_ID_FONT_OPENSANS_EXTRA_52_NUMBERS
#define FONT_OPENSANS_LIGHT_24_TEXT RESOURCE_ID_FONT_OPENSANS_LIGHT_24_TEXT
#define FONT_OPENSANS_BOLD_24_NUMBERS RESOURCE_ID_FONT_OPENSANS_BOLD_24_NUMBERS
#define FONT_OPENSANS_EXTRA_24_TEXT RESOURCE_ID_FONT_OPENSANS_EXTRA_24_TEXT
#define FONT_OPENSANS_12_TEXT RESOURCE_ID_FONT_OPENSANS_12_TEXT
#define NMEA_REMOTE_SPLASH RESOURCE_ID_NMEA_REMOTE_SPLASH

#define KNOTS_DEFAULT_VALUE "--.-"
#define SMILE_DEFAULT_VALUE "--.--"
#define ANGLE_DEFAULT_VALUE "---°"
#define TIME_DEFAULT_VALUE "--:--"
#define METER_DEFAULT_VALUE "--.-"
#define BFT_DEFAULT_VALUE "-"

#define APP_TIMER_TIMEOUT 1000

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

enum NMEAkey {
	SPEED_KEY = 0,  
	DEPTH_KEY = 1,	
	HDG_KEY = 2,
	AWA_KEY = 3,	  	
	BTW_KEY = 4,	  	
	DTW_KEY = 5,	  	
	TTG_KEY = 6,
	COG_KEY = 7,
	XTE_KEY = 8,
	SOG_KEY = 9,
	TWD_KEY = 10,	
	TWS_KEY = 11,			
	BFT_KEY = 12,			
	TARGET_SPEED_KEY = 13,		
	TARGET_SPEED_PERCENT_KEY = 14,	
	STARTTIME_INTERVAL1970_KEY = 15,
	/*Config*/
	URL_KEY = 0xFF
};

typedef struct {
	char speed[8];
	char depth[8];	
	char hdg[8];	
	char awa[8];	
	char btw[8];	
	char dtw[8];	
	char ttg[8];	
	char cog[8];	
	char xte[8];	
	char sog[8];			
	char twd[8];	
	char tws[8];	
	char bft[8];				
	char target_speed[8];
	char target_speed_percent[8];		
	time_t startime;
	char url[124];
} NMEAValues;

extern NMEAValues values;

#endif /* COMMON_H_ */
