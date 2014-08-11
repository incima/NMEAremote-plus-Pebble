#include "pebble.h"

#define FONT_OPENSANS_EXTRA_48_NUMBERS RESOURCE_ID_FONT_OPENSANS_EXTRA_52_NUMBERS
#define FONT_OPENSANS_LIGHT_24_TEXT RESOURCE_ID_FONT_OPENSANS_LIGHT_24_TEXT
#define FONT_OPENSANS_BOLD_24_NUMBERS RESOURCE_ID_FONT_OPENSANS_BOLD_24_NUMBERS
#define FONT_OPENSANS_EXTRA_24_TEXT RESOURCE_ID_FONT_OPENSANS_EXTRA_24_TEXT

#define SMILE_DEFAULT_VALUE "--.-"
#define ANGLE_DEFAULT_VALUE "---°"

static Window *window;

static char speed[16];
static char hdg[16];
static char awa[16];

static TextLayer *top_value_layer;
static TextLayer *top_title_layer;

static Layer *right_bg_layer;
static TextLayer *right_value_layer;
static TextLayer *right_title_layer;

static Layer *left_bg_layer;
static TextLayer *left_value_layer;
static TextLayer *left_title_layer;

static Layer *circle_layer;

#define SYNC_UPDATE_TIMEOUT 10
static AppSync sync;
static uint8_t sync_buffer[48];
static GColor sync_update_color = GColorWhite;
static int sync_update_count = 0;

#define APP_TIMER_TIMEOUT 1000

enum NMEAkey {
	SPEED_KEY = 0,  
	HDG_KEY = 1,
	AWA_KEY = 2,	  	
};


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {		
	if (sync_update_count < 0)
		sync_update_count = 1;
	else 
		++sync_update_count;
  switch (key) {
    case SPEED_KEY:
			memcpy(speed, new_tuple->value->cstring, new_tuple->length);					
      break;
    case HDG_KEY:
			memcpy(hdg, new_tuple->value->cstring, new_tuple->length);	
      break;
    case AWA_KEY:
			memcpy(awa, new_tuple->value->cstring, new_tuple->length);		
      break;			
  }
}

static void circle_layer_update_proc(Layer *layer, GContext *context)
{
	GRect layer_frame = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, sync_update_color);			
	graphics_fill_circle(context, (GPoint){layer_frame.size.w/2, layer_frame.size.h/2}, 2);				
}

static void rounded_layer_update_proc(Layer* layer, GContext *context) 
{	
	GRect layer_bounds = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);	
	graphics_fill_rect(context, layer_bounds, 4, GCornersAll);
}

static void app_timer_callback(void *data) 
{  
	if (sync_update_count > 0) {
		text_layer_set_text(top_value_layer, speed);				
	  text_layer_set_text(left_value_layer, hdg);		
	 	text_layer_set_text(right_value_layer, awa);								
		sync_update_color = (sync_update_color == GColorBlack ? GColorWhite : GColorBlack);
		layer_mark_dirty(circle_layer);							
		sync_update_count = 0;
	} else {
		//only set once to prevent updates
		if (--sync_update_count == -SYNC_UPDATE_TIMEOUT) {
			text_layer_set_text(top_value_layer, SMILE_DEFAULT_VALUE);				
		  text_layer_set_text(left_value_layer, ANGLE_DEFAULT_VALUE);		
		 	text_layer_set_text(right_value_layer, ANGLE_DEFAULT_VALUE);								
		}
		if (sync_update_color != GColorBlack) {
			sync_update_color = GColorBlack;		
			layer_mark_dirty(circle_layer);										
		}		
	}				
	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

	// Top layer
  top_value_layer = text_layer_create(GRect(0, 10, 144, 54));
  text_layer_set_text_color(top_value_layer, GColorWhite);
  text_layer_set_background_color(top_value_layer, GColorClear);
  text_layer_set_font(top_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_48_NUMBERS)));
  text_layer_set_text_alignment(top_value_layer, GTextAlignmentCenter);

  top_title_layer = text_layer_create(GRect(0, 64, 144, 40));
  text_layer_set_text_color(top_title_layer, GColorWhite);
  text_layer_set_background_color(top_title_layer, GColorClear);
  text_layer_set_font(top_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_LIGHT_24_TEXT)));
  text_layer_set_text_alignment(top_title_layer, GTextAlignmentCenter);
	text_layer_set_text(top_title_layer, "SPEED");

	// Left layer
	left_bg_layer = layer_create(GRect(0, 104, 70, 64));
	layer_set_update_proc(left_bg_layer, rounded_layer_update_proc);	

  left_value_layer = text_layer_create(GRect(0, 106, 70, 24));
  text_layer_set_background_color(left_value_layer, GColorClear);
  text_layer_set_text_color(left_value_layer, GColorBlack);	
  text_layer_set_font(left_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(left_value_layer, GTextAlignmentCenter);

  left_title_layer = text_layer_create(GRect(0, 133, 70, 37));
  text_layer_set_background_color(left_title_layer, GColorClear);
	text_layer_set_text_color(left_title_layer, GColorBlack);
  text_layer_set_font(left_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(left_title_layer, GTextAlignmentCenter);
	text_layer_set_text(left_title_layer, "HDG");

	// Right layer
	right_bg_layer = layer_create(GRect(74, 104, 70, 64));
	layer_set_update_proc(right_bg_layer, rounded_layer_update_proc);

  right_value_layer = text_layer_create(GRect(74, 106, 70, 24));
  text_layer_set_background_color(right_value_layer, GColorClear);
	text_layer_set_text_color(right_value_layer, GColorBlack);
  text_layer_set_font(right_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(right_value_layer, GTextAlignmentCenter);
			
  right_title_layer = text_layer_create(GRect(74, 133, 70, 37));
  text_layer_set_background_color(right_title_layer, GColorClear);
	text_layer_set_text_color(right_title_layer, GColorBlack);
  text_layer_set_font(right_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(right_title_layer, GTextAlignmentCenter);
	text_layer_set_text(right_title_layer, "AWA");
	
	GRect window_layer_frame = layer_get_frame(window_layer);			
	circle_layer = layer_create(GRect(window_layer_frame.origin.x + window_layer_frame.size.w - 7, 
																		window_layer_frame.origin.y + 1, 
																		5, 5));																		
	layer_set_update_proc(circle_layer, circle_layer_update_proc);	
  
	layer_add_child(window_layer, left_bg_layer);
	layer_add_child(window_layer, right_bg_layer);
  layer_add_child(window_layer, text_layer_get_layer(top_value_layer));
	layer_add_child(window_layer, text_layer_get_layer(top_title_layer));	
  layer_add_child(window_layer, text_layer_get_layer(left_value_layer));
	layer_add_child(window_layer, text_layer_get_layer(right_value_layer));	
  layer_add_child(window_layer, text_layer_get_layer(left_title_layer));
	layer_add_child(window_layer, text_layer_get_layer(right_title_layer));	
	layer_add_child(window_layer, circle_layer);	
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);

  text_layer_destroy(top_value_layer);
  text_layer_destroy(top_title_layer);	
  text_layer_destroy(left_value_layer);
  text_layer_destroy(left_title_layer);
  text_layer_destroy(right_value_layer);		
  text_layer_destroy(right_title_layer);	
	layer_destroy(left_bg_layer);
	layer_destroy(right_bg_layer);	
	layer_destroy(circle_layer);		
}

static void init() {
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

static void deinit() {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}