#include <pebble.h>
#include "common.h"
#include "controllers/trl_controller.h"
#include "controllers/splash_controller.h"

static Window *trl_window;
static Window *splash_window;
static Layer *circle_layer;

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[48];
static GColor sync_update_color = GColorWhite;
static int sync_update_count = 0;
static TRLController *trl_controller;
static SplashController *splash_controller;
static struct {
	char speed[8];
	char hdg[8];	
	char awa[8];	
} values;

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
  }	
}

static void app_timer_callback(void *data) 
{  
	if (sync_update_count > 0) {
		sync_update_color = (sync_update_color == GColorBlack ? GColorWhite : GColorBlack);				
		if (splash_controller)
			controller_redraw_if_needed(splash_controller_get_controller(splash_controller));									
		if (trl_controller)	{
			trl_controller->top_value = values.speed;
			trl_controller->left_value = values.hdg;
			trl_controller->right_value = values.awa;							
			controller_redraw_if_needed(trl_controller_get_controller(trl_controller));
			controller_redraw_update_layer(trl_controller_get_controller(trl_controller), sync_update_color);			
		}
		sync_update_count = 0;
	} else {
		//only set once to prevent updates
		if (--sync_update_count == -SYNC_UPDATE_TIMEOUT) {
			if (splash_controller)
				controller_cancel_redraw(splash_controller_get_controller(splash_controller));							
			if (trl_controller) {
				controller_cancel_redraw(trl_controller_get_controller(trl_controller));			
				controller_redraw_update_layer(trl_controller_get_controller(trl_controller), GColorBlack);			
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
	TRL Window
 */

static void trl_window_load(Window* window) 
{
	Layer *window_layer = window_get_root_layer(window);	
	 
	trl_controller = trl_controller_create(trl_window, (ControllerHandlers) {
		.did_load = controller_did_load,
		.did_unload = controller_did_unload
	});	
	trl_controller->top_title = "SPEED";
	trl_controller->left_title = "HDG";
	trl_controller->right_title = "AWA";
	trl_controller->top_default_value = SMILE_DEFAULT_VALUE;
	trl_controller->left_default_value = ANGLE_DEFAULT_VALUE;
	trl_controller->right_default_value = ANGLE_DEFAULT_VALUE;				
	controller_load(trl_controller_get_controller(trl_controller));
	controller_load_update_layer(trl_controller_get_controller(trl_controller));
}

static void trl_window_unload(Window* window) 
{
	layer_destroy(circle_layer);	
	controller_unload(trl_controller_get_controller(trl_controller));		
}

static void load_trl_window()
{
  trl_window = window_create();
  window_set_background_color(trl_window, GColorBlack);	
  window_set_fullscreen(trl_window, true);
  window_set_window_handlers(trl_window, (WindowHandlers) {
    .load = trl_window_load,
    .unload = trl_window_unload
  });			
}

static void push_trl_window()
{
	load_trl_window();
  const bool animated = true;	
	window_stack_push(trl_window, animated);	
}

/**
	Splash Window
*/

void splash_controller_did_finish(Controller *controller)
{
	push_trl_window();
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
	//controller_destory(splash_controller_get_controller(splash_controller)), splash_controller = NULL;
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
  const bool animated = true;	
	window_stack_push(splash_window, animated);
			
  const int inbound_size = 64;
  const int outbound_size = 16;
  app_message_open(inbound_size, outbound_size);
		
  Tuplet initial_values[] = {
    TupletCString(SPEED_KEY, SMILE_DEFAULT_VALUE),
    TupletCString(HDG_KEY, ANGLE_DEFAULT_VALUE),
    TupletCString(AWA_KEY, ANGLE_DEFAULT_VALUE)				
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void deinit() 
{
  app_sync_deinit(&sync);				
	if (trl_window)
		window_destroy(trl_window);
	if (splash_window)
		window_destroy(splash_window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}