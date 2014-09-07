#ifndef COMMON_H_
#define COMMON_H_

#include <pebble.h>

//#undef APP_LOG
//#define APP_LOG(...)

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
	time_t speed_ts;
	char depth[8];	
	time_t depth_ts;	
	char hdg[8];	
	time_t hdg_ts;
	char awa[8];	
	time_t awa_ts;
	char btw[8];	
	time_t btw_ts;
	char dtw[8];	
	time_t dtw_ts;
	char ttg[8];	
	time_t ttg_ts;
	char cog[8];	
	time_t cog_ts;
	char xte[8];	
	time_t xte_ts;
	char sog[8];	
	time_t sog_ts;		
	char twd[8];	
	time_t twd_ts;
	char tws[8];	
	time_t tws_ts;
	char bft[8];	
	time_t bft_ts;			
	char target_speed[8];
	time_t target_speed_ts;
	char target_speed_percent[8];		
	time_t target_speed_percent_ts;
	time_t starttime;
	char starttime_str[8];
	time_t starttime_ts;
	char current_time[8];
	char current_date[16];	
	time_t current_time_ts;
	char url[124];
	time_t url_ts;
} NMEAValues;

extern NMEAValues values;

#endif /* COMMON_H_ */
