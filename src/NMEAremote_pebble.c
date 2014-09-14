#include <pebble.h>
#include <math.h>

#include "common/common.h"
#include "ui/ui.h"

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[1024];
static GColor sync_update_color = GColorWhite;
static unsigned long last_sync_update_count = 0;
static unsigned long sync_update_count = 0;
static Window *splash_window = NULL;
static SplashController *splash_controller = NULL;
typedef struct {
    struct list_head list;		
		Controller* controller;		
} ControllerEntry;
struct list_head controller_list;
	
NMEAValues values;

static void connect_to_url();
static Controller* window_get_controller(Window *window);

static void connect_failed_timer_callback(void *data) 
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "connect_failed_timer_callback %lu", sync_update_count);					
	if (splash_controller) {
		splash_controller_set_info_text(splash_controller, "FAILURE");			
		splash_controller_set_updating(splash_controller, false);	
	}	
}

static void connect_success_timer_callback(void *data)
{	
  APP_LOG(APP_LOG_LEVEL_DEBUG, "connect_success_timer_callback %lu", sync_update_count);				
	if (splash_controller) {				
		if (!list_empty(&controller_list)) {
			struct list_head *ptr = controller_list.next;	
			ControllerEntry *entry = list_entry(ptr, ControllerEntry, list);	
			window_stack_push(controller_get_window(entry->controller), true);			
		}
		window_stack_remove(splash_window, false);						
	}
	// Save URL
	if (strlen(values.url)) 
		persist_write_string(URL_KEY, values.url);			
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) 
{		
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "sync_tuple_changed_callback: %u", (unsigned int)key);		
	
	bool success = false;	
	switch (key) {
		case URL_KEY:
			if (old_tuple == NULL || 0 != strcmp(new_tuple->value->cstring, old_tuple->value->cstring)) {
				memcpy(values.url, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.url)));							
				if (strlen(values.url)) {
					connect_to_url();
				}
			}
			break;
		case SPEED_KEY:
			memcpy(values.speed, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.speed)));					
			values.speed_ts = time(NULL);
			success = new_tuple->length > 1;
			break;
		  case DEPTH_KEY:
			memcpy(values.depth, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.depth)));					
			values.depth_ts = time(NULL);
			success = new_tuple->length > 1;			
			break;
		case HDG_KEY:
			memcpy(values.hdg, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.hdg)));	
			values.hdg_ts = time(NULL);
			success = new_tuple->length > 1;
			break;
		case AWA_KEY:
			memcpy(values.awa, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.awa)));		
			values.awa_ts = time(NULL);
			success = new_tuple->length > 1;
			break;
		case BTW_KEY:
			memcpy(values.btw, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.btw)));	
			values.btw_ts = time(NULL);
			success = new_tuple->length > 1;
			break;
		case DTW_KEY:
			memcpy(values.dtw, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.dtw)));		
			values.dtw_ts = time(NULL);
			success = new_tuple->length > 1;
			break;
		case TTG_KEY:
			memcpy(values.ttg, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.ttg)));	
			values.ttg_ts = time(NULL);
			success = new_tuple->length > 1;
			break;		
		case COG_KEY:
			memcpy(values.cog, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.cog)));	
			values.cog_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case XTE_KEY:
			memcpy(values.xte, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.xte)));	
			values.xte_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case SOG_KEY:
			memcpy(values.sog, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.sog)));	
			values.sog_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case TWD_KEY:
			memcpy(values.twd, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.twd)));	
			values.twd_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case TWS_KEY:		
			memcpy(values.tws, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.tws)));	
			values.tws_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case BFT_KEY:
			memcpy(values.bft, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.bft)));	
			values.bft_ts = time(NULL);
			success = new_tuple->length > 1;
			break;										
		case TARGET_SPEED_KEY:			
			memcpy(values.target_speed, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.target_speed)));	
			values.target_speed_ts = time(NULL);
			success = new_tuple->length > 1;
			break;				
		case TARGET_SPEED_PERCENT_KEY:
			memcpy(values.target_speed_percent, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.target_speed_percent)));	
			values.target_speed_percent_ts = time(NULL);
			success = new_tuple->length > 1;			
			break;				
		case STARTTIME_INTERVAL1970_KEY: {			
			char temp[16];
			memcpy(temp, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(temp)));                  
			values.starttime = (time_t)atol(temp);			
			values.starttime_ts = time(NULL);
			success = values.starttime > 0;			
			break;		
		}
		case TIMEZONE_OFFSET_KEY: 
			values.timezone_offset = (time_t)new_tuple->value->int32;
			values.timezone_offset_ts = time(NULL);
			success = true;
			break;
		default:
			success = false;
			break;	
  }	
	
	if (success) {
		++sync_update_count;	
		if (splash_controller && splash_controller->updating) {		
			splash_controller_set_info_text(splash_controller, "CONNECTED");					
			app_timer_register(1250, connect_success_timer_callback, NULL);			
		}	
	}
}

static void app_timer_callback(void *data) 
{  	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "app_timer_callback: %lu", sync_update_count);			
	
	if (sync_update_count > last_sync_update_count) {
		sync_update_color = (sync_update_color == GColorBlack ? GColorWhite : GColorBlack);							
	} 
	else {
		sync_update_color = GColorBlack;
	}
	
	time_t now = time(NULL);
	
	// update current_time
	values.current_time_ts = now;
	struct tm *tm = localtime(&values.current_time_ts);
	strftime(values.current_time, sizeof(values.current_time), "%R", tm);
	strftime(values.current_date, sizeof(values.current_date), "%e %b %Y", tm);		

	// update starttime if available
	if (values.starttime > 0) {						
		values.seconds_to_gun = values.starttime - (values.current_time_ts + values.timezone_offset);				
  		format_seconds(values.seconds_to_gun, values.starttime_str, sizeof(values.starttime_str));				
	}
	else {
		char temp[] = "--:--";
		values.seconds_to_gun = 0;
		strncpy(values.starttime_str, temp, MIN(sizeof(temp)+1, sizeof(values.starttime_str)));
	}
		
	// check values for actuality
	if (now - values.speed_ts > 10)
		memcpy(values.speed, KNOTS_DEFAULT_VALUE, MIN(strlen(KNOTS_DEFAULT_VALUE)+1, sizeof(values.speed)));	
	if (now - values.depth_ts > 10)		
		memcpy(values.depth, METER_DEFAULT_VALUE, MIN(strlen(METER_DEFAULT_VALUE)+1, sizeof(values.depth)));			
	if (now - values.hdg_ts > 10)		
		memcpy(values.hdg, ANGLE_DEFAULT_VALUE, MIN(strlen(ANGLE_DEFAULT_VALUE)+1, sizeof(values.hdg)));		
	if (now - values.awa_ts > 10)		
		memcpy(values.awa, ANGLE_DEFAULT_VALUE, MIN(strlen(ANGLE_DEFAULT_VALUE)+1, sizeof(values.awa)));			
	if (now - values.btw_ts > 10)		
		memcpy(values.btw, ANGLE_DEFAULT_VALUE, MIN(strlen(ANGLE_DEFAULT_VALUE)+1, sizeof(values.btw)));	
	if (now - values.dtw_ts > 10)		
		memcpy(values.dtw, SMILE_DEFAULT_VALUE, MIN(strlen(SMILE_DEFAULT_VALUE)+1, sizeof(values.dtw)));		
	if (now - values.ttg_ts > 10)		
		memcpy(values.ttg, TIME_DEFAULT_VALUE, MIN(strlen(TIME_DEFAULT_VALUE)+1, sizeof(values.ttg)));			
	if (now - values.cog_ts > 10)		
		memcpy(values.cog, ANGLE_DEFAULT_VALUE, MIN(strlen(ANGLE_DEFAULT_VALUE)+1, sizeof(values.cog)));			
	if (now - values.xte_ts > 10)		
		memcpy(values.xte, SMILE_DEFAULT_VALUE, MIN(strlen(SMILE_DEFAULT_VALUE)+1, sizeof(values.xte)));			
	if (now - values.sog_ts > 10)		
		memcpy(values.sog, KNOTS_DEFAULT_VALUE, MIN(strlen(KNOTS_DEFAULT_VALUE)+1, sizeof(values.sog)));									
	if (now - values.twd_ts > 10)		
		memcpy(values.twd, ANGLE_DEFAULT_VALUE, MIN(strlen(ANGLE_DEFAULT_VALUE)+1, sizeof(values.cog)));	
	if (now - values.tws_ts > 10)				
		memcpy(values.tws, KNOTS_DEFAULT_VALUE, MIN(strlen(KNOTS_DEFAULT_VALUE)+1, sizeof(values.xte)));			
	if (now - values.bft_ts > 10)		
		memcpy(values.bft, BFT_DEFAULT_VALUE, MIN(strlen(BFT_DEFAULT_VALUE)+1, sizeof(values.sog)));												
	if (now - values.target_speed_ts > 10)		
		memcpy(values.target_speed, SMILE_DEFAULT_VALUE, MIN(strlen(SMILE_DEFAULT_VALUE)+1, sizeof(values.target_speed)));			
	if (now - values.target_speed_percent_ts > 10)
		memcpy(values.target_speed_percent, KNOTS_DEFAULT_VALUE, MIN(strlen(KNOTS_DEFAULT_VALUE)+1, sizeof(values.target_speed_percent)));									
	
	Window* top_window = window_stack_get_top_window();	
	if (top_window == splash_window)
		controller_redraw(splash_controller_get_controller(splash_controller));									
	else {															
		Controller *controller = window_get_controller(top_window);
		if (controller) {
			controller_redraw(controller);												 
			controller_redraw_update_layer(controller, sync_update_color);			
		}
	}
	last_sync_update_count = sync_update_count;
	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void controller_did_load(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Controller did load");
}

static void controller_did_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Controller did unload");
}

/**
 Button events
 */

static void window_prev_controller(Window *window) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "window_prev_controller");
			
	struct list_head *ptr;	
	ControllerEntry *entry;		
	list_for_each(ptr, &controller_list) {
		entry = list_entry(ptr, ControllerEntry, list);
		if (controller_get_window(entry->controller) == window) {
			 if (ptr->prev != &controller_list) {
				 window_stack_pop(controller_get_window(entry->controller));			 
				 return;
			 }    
		 }
	 }
}

static void window_next_controller(Window *window) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "window_next_controller");
		
	struct list_head *ptr;	
	ControllerEntry *entry;		
	list_for_each(ptr, &controller_list) {
		entry = list_entry(ptr, ControllerEntry, list);
		if (controller_get_window(entry->controller) == window) {
			if (ptr->next != &controller_list) {
				ControllerEntry *next_entry = list_entry(ptr->next, ControllerEntry, list);		
				Window *window = controller_get_window(next_entry->controller);
				if (window)
					window_stack_push(window, true);	
				return;
			} 
		 }	
	}
}


static Controller* window_get_controller(Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "window_get_controller %p", window);
	
	struct list_head *ptr, *tmp;	
	ControllerEntry *entry;		
	list_for_each_safe(ptr, tmp, &controller_list) {
	    entry = list_entry(ptr, ControllerEntry, list);	    
		if (entry->controller && controller_get_window(entry->controller) == window) {			
			ControllerEntry *entry = list_entry(ptr, ControllerEntry, list);
			return entry->controller;				
		} 
	}
	return NULL;
}

static bool handle_click = false;

static void window_generic_click_up_handler(ClickRecognizerRef recognizer, Window *window) 
{		
	Controller *controller = window_get_controller(window);
	ButtonId button_id = click_recognizer_get_button_id(recognizer);	
	switch (button_id) {
	case BUTTON_ID_BACK:
		if (handle_click) {
			window_prev_controller(window);
			return;
		} else if (controller) {
			handle_click = !controller_on_button_up(controller, recognizer);
		}
		break;
	case BUTTON_ID_SELECT:
		window_next_controller(window);
		handle_click = true;	
		break;
	default:		 		
		handle_click = !controller_on_button_up(controller, recognizer);		
		break;
	}	
}

static void window_click_config_provider(Window *window) 
{	
	window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)window_generic_click_up_handler);
	window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)window_generic_click_up_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)window_generic_click_up_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)window_generic_click_up_handler);  
}

/**
	Controllers
 */

static void window_load(Window* window) 
{	 
	APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load %p", window);		
	
	Controller *controller = window_get_controller(window);
	if (controller) {
		controller_load(controller);
	}
}

static void window_unload(Window* window) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "window_unload %p", window);
	
	Controller *controller = window_get_controller(window);
	if (controller) {
		controller_unload(controller);
	}
}

static void controller_entry_create(Controller *controller)
{
	ControllerEntry *entry = malloc(sizeof(ControllerEntry));
	memset(entry, 0, sizeof(*entry));
	entry->controller = controller;
	Window *window = controller_get_window(entry->controller);
	window_set_background_color(window, GColorBlack);	
	window_set_fullscreen(window, true);		
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});
	list_add_tail(&entry->list, &controller_list);	
	window_set_click_config_provider(window, (ClickConfigProvider)window_click_config_provider);	
}

/**
	Splash Window
*/

static void splash_window_load(Window *window)
{
	splash_controller = splash_controller_create(window, (ControllerHandlers) {
		.did_load = controller_did_load,
		.did_unload = controller_did_unload
	});	
	splash_controller_set_bitmap_from_resource(splash_controller, NMEA_REMOTE_SPLASH);
	controller_load(splash_controller_get_controller(splash_controller));		
}

static void splash_window_unload(Window *window)
{
	if (splash_controller)
		controller_destroy(splash_controller_get_controller(splash_controller)), splash_controller = NULL;
}

static void load_splash_window()
{
	if (!splash_window)
		splash_window = window_create();
	window_set_background_color(splash_window, GColorBlack);	
	window_set_fullscreen(splash_window, true);
	window_set_window_handlers(splash_window, (WindowHandlers) {
		.load = splash_window_load,
		.unload = splash_window_unload
	});			
}

static void connect_to_url() 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "URL: %s", values.url);						
	if (!splash_window) {
		load_splash_window();	
		window_stack_pop_all(false);
		window_stack_push(splash_window, false);	
	}
	if (splash_controller) {
		splash_controller_set_updating(splash_controller, true);
		splash_controller_set_info_text(splash_controller, "CONNECTING");						
		app_timer_register(15000, connect_failed_timer_callback, NULL);
	}
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet cfg_val = TupletCString(URL_KEY, values.url);
	dict_write_tuplet(iter, &cfg_val);
	app_message_outbox_send();
}

static void init() 
{	
	fonts_load();
	
	INIT_LIST_HEAD(&controller_list);	
		
	load_splash_window();	
	window_stack_push(splash_window, true);	
	
	controller_entry_create(trl_controller_get_controller(trl_controller_create(window_create(), (ControllerHandlers) {
		.did_load = controller_did_load,
		.did_unload = controller_did_unload
	})));
	controller_entry_create(timer_controller_get_controller(timer_controller_create(window_create(), (ControllerHandlers) {
		.did_load = controller_did_load,
		.did_unload = controller_did_unload
	})));
									
  const int inbound_size = app_message_inbox_size_maximum();
  const int outbound_size = app_message_outbox_size_maximum();
  app_message_open(inbound_size, outbound_size);
	
	// Try read URL
	persist_read_string(URL_KEY, values.url, sizeof(values.url));	
	if (strlen(values.url))
		connect_to_url();
		
  Tuplet initial_values[] = {
		TupletCString(SPEED_KEY, ""),
		TupletCString(DEPTH_KEY, ""),
		TupletCString(HDG_KEY, ""),
		TupletCString(AWA_KEY, ""),						
		TupletCString(BTW_KEY, ""),	
		TupletCString(DTW_KEY, ""),	
		TupletCString(TTG_KEY, ""),	
		TupletCString(COG_KEY, ""),	
		TupletCString(XTE_KEY, ""),	
		TupletCString(SOG_KEY, ""),	
		TupletCString(TWD_KEY, ""),	
		TupletCString(TWS_KEY, ""),	
		TupletCString(BFT_KEY, ""),			
		TupletCString(TARGET_SPEED_KEY, ""),	
		TupletCString(TARGET_SPEED_PERCENT_KEY, ""),	
		TupletInteger(STARTTIME_INTERVAL1970_KEY, (time_t)0),
		TupletInteger(TIMEZONE_OFFSET_KEY, (time_t)0),		
		TupletCString(URL_KEY, "http://192.168.1.13:8080/json")				
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void deinit() 
{	
	struct list_head *ptr, *tmp;	
	ControllerEntry *entry;		
	list_for_each_safe(ptr, tmp, &controller_list) {
     entry = list_entry(ptr, ControllerEntry, list);
		 Window *window = controller_get_window(entry->controller);
		 if (entry->controller)
			 controller_destroy(entry->controller), entry->controller = NULL;
		 if (window)
			 window_destroy(window);
		 list_del(ptr);
		 free(entry);		 
	}		
	if (splash_controller)
		controller_destroy(splash_controller_get_controller(splash_controller)), splash_controller = NULL;			
	if (splash_window)
		window_destroy(splash_window), splash_window = NULL;
	
  app_sync_deinit(&sync);	
	
	fonts_unload();
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}