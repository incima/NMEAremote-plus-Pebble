#include <pebble.h>
#include "common.h"
#include "controllers/trl_controller.h"
#include "controllers/splash_controller.h"

static Window *speed_window;
static Window *btw_window;
static Window *splash_window;

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[128];
static GColor sync_update_color = GColorWhite;
static int sync_update_count = 0;
static TRLController *speed_controller;
static TRLController *btw_controller;
static SplashController *splash_controller;
static struct {
	char speed[8];
	char hdg[8];	
	char awa[8];	
	char btw[8];	
	char dtw[8];	
	char ttg[8];	
} values;

void load_speed_window();
void load_btw_window();

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) 
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) 
{		
	if (sync_update_count < 0)
		sync_update_count = 1;
	else 
		++sync_update_count;	
  switch (key) {
    case SPEED_KEY:
			memcpy(values.speed, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.speed)));					
			break;
    case HDG_KEY:
			memcpy(values.hdg, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.hdg)));	
			break;
    case AWA_KEY:
			memcpy(values.awa, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.awa)));		
			break;
    case BTW_KEY:
			memcpy(values.btw, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.btw)));	
			break;
    case DTW_KEY:
			memcpy(values.dtw, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.dtw)));		
			break;
    case TTG_KEY:
			memcpy(values.ttg, new_tuple->value->cstring, MIN(new_tuple->length, sizeof(values.ttg)));	
			break;			
  }	
}

static void app_timer_callback(void *data) 
{  
	Window* top_window = window_stack_get_top_window();
	if (sync_update_count > 0) {
		sync_update_color = (sync_update_color == GColorBlack ? GColorWhite : GColorBlack);				
		if (top_window == splash_window)
			controller_redraw_if_needed(splash_controller_get_controller(splash_controller));									
		else if (top_window == speed_window)	{
			speed_controller->top_value = values.speed;
			speed_controller->left_value = values.hdg;
			speed_controller->right_value = values.awa;							
			controller_redraw_if_needed(trl_controller_get_controller(speed_controller));
			controller_redraw_update_layer(trl_controller_get_controller(speed_controller), sync_update_color);			
		}
		else if (top_window == btw_window){
			btw_controller->top_value = values.btw;
			btw_controller->left_value = values.dtw;
			btw_controller->right_value = values.ttg;							
			controller_redraw_if_needed(trl_controller_get_controller(btw_controller));
			controller_redraw_update_layer(trl_controller_get_controller(btw_controller), sync_update_color);			
		}		
		sync_update_count = 0;
	} else {
		//only set once to prevent updates
		if (--sync_update_count == -SYNC_UPDATE_TIMEOUT) {
			if (top_window == splash_window)
				controller_redraw_if_needed(splash_controller_get_controller(splash_controller));							
			else if (top_window == speed_window) {
				controller_cancel_redraw(trl_controller_get_controller(speed_controller));			
				controller_redraw_update_layer(trl_controller_get_controller(speed_controller), GColorBlack);						
			}
			else if (top_window == btw_window) {
				controller_cancel_redraw(trl_controller_get_controller(btw_controller));			
				controller_redraw_update_layer(trl_controller_get_controller(btw_controller), GColorBlack);			
			}			
		}
	}				
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

static void window_up_click_handler(ClickRecognizerRef recognizer, void *context) {
		Window *window = (Window *)context;
		if (window == btw_window) {
			window_stack_pop(btw_window);
		}
}

static void window_down_click_handler(ClickRecognizerRef recognizer, void *context) {
		Window *window = (Window *)context;
		if (window == speed_window) {			
			window_stack_push(btw_window, true);
		}
}

static void window_select_click_handler(ClickRecognizerRef recognizer, void *context) {
		Window *window = (Window *)context;
}

/**
	Speed HDG AWA Window
 */

static void speed_window_load(Window* window) 
{	 
	if (!speed_controller) {
		speed_controller = trl_controller_create(window, (ControllerHandlers) {
			.did_load = controller_did_load,
			.did_unload = controller_did_unload
		});	
	}
	speed_controller->top_title = "SPEED";
	speed_controller->left_title = "HDG";
	speed_controller->right_title = "AWA";
	speed_controller->top_default_value = SMILE_DEFAULT_VALUE;
	speed_controller->left_default_value = ANGLE_DEFAULT_VALUE;
	speed_controller->right_default_value = ANGLE_DEFAULT_VALUE;				
	controller_load(trl_controller_get_controller(speed_controller));
	controller_load_update_layer(trl_controller_get_controller(speed_controller));
}

static void speed_window_unload(Window* window) 
{
	controller_unload(trl_controller_get_controller(speed_controller));		
}

static void speed_window_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, window_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, window_down_click_handler);		
  window_single_click_subscribe(BUTTON_ID_SELECT, window_select_click_handler);
}

void load_speed_window()
{
  speed_window = window_create();
  window_set_background_color(speed_window, GColorBlack);	
  window_set_fullscreen(speed_window, true);
	window_set_click_config_provider(speed_window, speed_window_click_config_provider);			
  window_set_window_handlers(speed_window, (WindowHandlers) {
    .load = speed_window_load,
    .unload = speed_window_unload
  });			
}

/**
	Speed HDG AWA Window
 */

static void btw_window_load(Window* window) 
{	 
	if (!btw_controller) {
		btw_controller = trl_controller_create(window, (ControllerHandlers) {
			.did_load = controller_did_load,
			.did_unload = controller_did_unload
		});	
	}
	btw_controller->top_title = "BTW";
	btw_controller->left_title = "DTW";
	btw_controller->right_title = "TTG";
	btw_controller->top_default_value = ANGLE_DEFAULT_VALUE;
	btw_controller->left_default_value = ANGLE_DEFAULT_VALUE;
	btw_controller->right_default_value = SMILE_DEFAULT_VALUE;				
	controller_load(trl_controller_get_controller(btw_controller));
	controller_load_update_layer(trl_controller_get_controller(btw_controller));
}

static void btw_window_unload(Window* window) 
{
	controller_unload(trl_controller_get_controller(btw_controller));		
}

static void btw_window_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, window_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, window_down_click_handler);		
  window_single_click_subscribe(BUTTON_ID_SELECT, window_select_click_handler);
}

void load_btw_window()
{
  btw_window = window_create();
  window_set_background_color(btw_window, GColorBlack);	
  window_set_fullscreen(btw_window, true);
  window_set_click_config_provider(btw_window, btw_window_click_config_provider);		
  window_set_window_handlers(btw_window, (WindowHandlers) {
    .load = btw_window_load,
    .unload = btw_window_unload
  });			
}

/**
	Splash Window
*/

void splash_controller_did_finish(Controller *controller)
{
	window_stack_push(speed_window, true);			
	window_stack_remove(splash_window, false);	
}

static void splash_window_load(Window *window)
{
	splash_controller = splash_controller_create(window, (ControllerHandlers) {
		.did_load = controller_did_load,
		.did_unload = controller_did_unload,
		.did_finish = splash_controller_did_finish
	});
	splash_controller_set_bitmap_from_resource(splash_controller, NMEA_REMOTE_SPLASH);
	controller_load(splash_controller_get_controller(splash_controller));		
}

static void splash_window_unload(Window *window)
{
	controller_unload(splash_controller_get_controller(splash_controller));
	window_destroy(splash_window), splash_window = NULL;			
}

static void load_splash_window()
{
  splash_window = window_create();
  window_set_background_color(splash_window, GColorBlack);	
  window_set_fullscreen(splash_window, true);
  window_set_window_handlers(splash_window, (WindowHandlers) {
    .load = splash_window_load,
    .unload = splash_window_unload
  });			
}

static void init() 
{
	load_splash_window();	
	window_stack_push(splash_window, true);
		
	load_speed_window();
	load_btw_window();		
			
  const int inbound_size = 128;
  const int outbound_size = 128;
  app_message_open(inbound_size, outbound_size);
		
  Tuplet initial_values[] = {
    TupletCString(SPEED_KEY, SMILE_DEFAULT_VALUE),
    TupletCString(HDG_KEY, ANGLE_DEFAULT_VALUE),
    TupletCString(AWA_KEY, ANGLE_DEFAULT_VALUE),			
	  TupletCString(BTW_KEY, ANGLE_DEFAULT_VALUE),
	  TupletCString(DTW_KEY, ANGLE_DEFAULT_VALUE),			
		TupletCString(TTG_KEY, ANGLE_DEFAULT_VALUE)
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void deinit() 
{
	controller_destroy(splash_controller_get_controller(splash_controller)), splash_controller = NULL;			
	controller_destroy(trl_controller_get_controller(btw_controller)), btw_controller = NULL;		
	controller_destroy(trl_controller_get_controller(speed_controller)), speed_controller = NULL;			
  app_sync_deinit(&sync);				
	if (speed_window)
		window_destroy(speed_window);
	if (btw_window)
		window_destroy(btw_window);
	if (splash_window)
		window_destroy(splash_window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}