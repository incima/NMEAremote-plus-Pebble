#include <pebble.h>
#include "common.h"
#include "list.h"
#include "classes/fonts.h"
#include "classes/trl_controller.h"
#include "classes/splash_controller.h"

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[1024];
static GColor sync_update_color = GColorWhite;
static unsigned long last_sync_update_count = 0;
static unsigned long sync_update_count = 0;
static Window *splash_window = NULL;
static SplashController *splash_controller = NULL;
typedef enum {
	TRL_CONTROLLER=0,
} ControllerID;
struct ControllerEntry {
    struct list_head list;		
		ControllerID controllerID;				
		Window* window;
		Controller* controller;
};
struct list_head controller_list;
	
NMEAValues values;

static void connect_to_url();

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
			struct ControllerEntry *entry = list_entry(ptr, struct ControllerEntry, list);	
			window_stack_push(entry->window, true);			
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
		case STARTTIME_INTERVAL1970_KEY:		
			values.startime = (time_t)new_tuple->value->uint32;
			values.startime_ts = time(NULL);
			success = values.startime > 0;			
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
		struct list_head *ptr;	
		struct ControllerEntry *entry;		
	  list_for_each(ptr, &controller_list) {
	     entry = list_entry(ptr, struct ControllerEntry, list);
	     if (entry->window == top_window) {
				 controller_redraw(entry->controller);												 
				 controller_redraw_update_layer(entry->controller, sync_update_color);												 			 				 
	     }			 
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

static void click_handler_prev_controller(ClickRecognizerRef recognizer, Window *window) {
	if(splash_window)
		return;
		
	struct list_head *ptr;	
	struct ControllerEntry *entry;		
  list_for_each(ptr, &controller_list) {
     entry = list_entry(ptr, struct ControllerEntry, list);
     if (entry->window == window) {
			 if (ptr->prev != &controller_list) {
				 window_stack_pop(entry->window);			 
				 return;
			 }    
		 }
	 }
}

static void click_handler_next_controller(ClickRecognizerRef recognizer, Window *window) {
	if(splash_window)
		return;
	
	struct list_head *ptr;	
	struct ControllerEntry *entry;		
  list_for_each(ptr, &controller_list) {
     entry = list_entry(ptr, struct ControllerEntry, list);
     if (entry->window == window) {
			 	if (ptr->next != &controller_list) {
				 	struct ControllerEntry *next_entry = list_entry(ptr->next, struct ControllerEntry, list);			 
				 	window_stack_push(next_entry->window, true);	
					return;
				} 
		 }
	}
}
/*
static void window_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	if(splash_window)
		return;

	Window *window = (Window *)context;
	connect_to_url();
}
*/
static void window_click_config_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)click_handler_prev_controller);	
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)click_handler_next_controller);
}

/**
	TRL Controllers
 */

static void trl_window_load(Window* window) 
{	 
	struct list_head *ptr;	
	struct ControllerEntry *entry;	
  list_for_each(ptr, &controller_list) {
     entry = list_entry(ptr, struct ControllerEntry, list);
     if (entry->window == window) {
			 	if (!entry->controller) {
					TRLController *trl_controller = trl_controller_create(window, (ControllerHandlers) {
			 			.did_load = controller_did_load,
			 			.did_unload = controller_did_unload
			 		});	
			 		entry->controller = trl_controller_get_controller(trl_controller);
			 	}
			 	controller_load(entry->controller);
			 	controller_load_update_layer(entry->controller);
     }
  }	
}

static void trl_window_unload(Window* window) 
{
	struct list_head *ptr;	
	struct ControllerEntry *entry;		
  list_for_each(ptr, &controller_list) {
     entry = list_entry(ptr, struct ControllerEntry, list);
     if (entry->window == window) {
				 if (entry->controller)
					 controller_destroy(entry->controller), entry->controller = NULL;
     }
  }		
}

static void load_trl_window_for_controller_id(ControllerID controllerID)
{
	struct ControllerEntry *entry = malloc(sizeof(struct ControllerEntry));
	memset(entry, 0, sizeof(struct ControllerEntry));
	entry->controllerID = controllerID;		
  entry->window = window_create();
	entry->controller = NULL;
	list_add_tail(&entry->list, &controller_list);		
  window_set_background_color(entry->window, GColorBlack);	
  window_set_fullscreen(entry->window, true);
  window_set_click_config_provider(entry->window, (ClickConfigProvider)window_click_config_provider);		
  window_set_window_handlers(entry->window, (WindowHandlers) {
    .load = trl_window_load,
    .unload = trl_window_unload
  });
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
	
	load_trl_window_for_controller_id(TRL_CONTROLLER);
								
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
		TupletCString(URL_KEY, "")				
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void deinit() 
{	
	struct list_head *ptr, *tmp;	
	struct ControllerEntry *entry;		
  list_for_each_safe(ptr, tmp, &controller_list) {
     entry = list_entry(ptr, struct ControllerEntry, list);
		 if (entry->controller)
			 controller_destroy(entry->controller), entry->controller = NULL;
		 if (entry->window)
			 window_destroy(entry->window), entry->window = NULL;		
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