#include <pebble.h>
#include "controller.h"
#include "common.h"

static Window *window;
static Layer *circle_layer;

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[48];
static GColor sync_update_color = GColorWhite;
static int sync_update_count = 0;
static TRLController *trl_controller;
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

static void circle_layer_update_proc(Layer *layer, GContext *context)
{
	GRect layer_frame = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, sync_update_color);			
	graphics_fill_circle(context, (GPoint){layer_frame.size.w/2, layer_frame.size.h/2}, 2);				
}

static void app_timer_callback(void *data) 
{  
	if (sync_update_count > 0) {
		trl_controller->top_value = values.speed;
		trl_controller->left_value = values.hdg;
		trl_controller->right_value = values.awa;				
		controller_redraw_if_needed(trl_controller_get_controller(trl_controller));
		sync_update_color = (sync_update_color == GColorBlack ? GColorWhite : GColorBlack);
		layer_mark_dirty(circle_layer);							
		sync_update_count = 0;
	} else {
		//only set once to prevent updates
		if (--sync_update_count == -SYNC_UPDATE_TIMEOUT) {
			controller_cancel_redraw(trl_controller_get_controller(trl_controller));			
		}
		if (sync_update_color != GColorBlack) {
			sync_update_color = GColorBlack;		
			layer_mark_dirty(circle_layer);										
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

static void window_load(Window* window) 
{
	Layer *window_layer = window_get_root_layer(window);	
	 
	trl_controller = trl_controller_create(window, (ControllerHandlers) {
		.on_did_load = controller_did_load,
		.on_did_unload = controller_did_unload
	});	
	trl_controller->top_title = "SPEED";
	trl_controller->left_title = "HDG";
	trl_controller->right_title = "AWA";
	trl_controller->top_default_value = SMILE_DEFAULT_VALUE;
	trl_controller->left_default_value = ANGLE_DEFAULT_VALUE;
	trl_controller->right_default_value = ANGLE_DEFAULT_VALUE;				
	controller_load(trl_controller_get_controller(trl_controller));
	
	GRect window_layer_frame = layer_get_frame(window_layer);			
	circle_layer = layer_create(GRect(window_layer_frame.origin.x + window_layer_frame.size.w - 7, 
																		window_layer_frame.origin.y + 1, 
																		5, 5));																		
	layer_set_update_proc(circle_layer, circle_layer_update_proc);	
  layer_add_child(window_layer, circle_layer);		
}

static void window_unload(Window* window) 
{
	controller_unload(trl_controller_get_controller(trl_controller));	
	
	layer_destroy(circle_layer);	
}

static void init() 
{
  window = window_create();
  window_set_background_color(window, GColorBlack);	
  window_set_fullscreen(window, true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });	
		
  const int inbound_size = 64;
  const int outbound_size = 16;
  app_message_open(inbound_size, outbound_size);

  const bool animated = true;
	window_stack_push(window, animated);
		
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
  window_destroy(window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}