#include "pebble.h"


#define FONT_OPENSANS_EXTRA_48_NUMBERS RESOURCE_ID_FONT_OPENSANS_EXTRA_52_NUMBERS
#define FONT_OPENSANS_BOLD_24_NUMBERS RESOURCE_ID_FONT_OPENSANS_BOLD_24_NUMBERS
#define FONT_OPENSANS_EXTRA_24_TEXT RESOURCE_ID_FONT_OPENSANS_EXTRA_24_TEXT

static Window *window;

static TextLayer *speed_layer;
static char speed[16];
static char hdg[16];
static char awa[16];

static Layer *circle_layer;

static Layer *right_bg_layer;
static TextLayer *right_value_layer;
static TextLayer *right_title_layer;

static Layer *left_bg_layer;
static TextLayer *left_value_layer;
static TextLayer *left_title_layer;

static AppSync sync;
static uint8_t sync_buffer[48];

#define APP_TIMER_TIMEOUT 1000

enum NMEAkey {
	SPEED_KEY = 0,  
	HDG_KEY = 1,
	AWA_KEY = 2,	  	
};

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "left_value_layer Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {	
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
  static GColor color = GColorWhite;
		
	GRect layer_frame = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, color);			
	graphics_fill_circle(context, (GPoint){layer_frame.size.w/2, layer_frame.size.h/2}, layer_frame.size.w/2);			
	color = (color == GColorWhite ? GColorBlack : GColorWhite);
}

static void rounded_layer_update_proc(Layer* layer, GContext *context) 
{	
	GRect layer_bounds = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);	
	graphics_fill_rect(context, layer_bounds, 4, GCornersAll);
}

static void app_timer_callback(void *data) 
{	
	text_layer_set_text(speed_layer, speed);		
  text_layer_set_text(left_value_layer, hdg);		
 	text_layer_set_text(right_value_layer, awa);
	
	layer_mark_dirty(circle_layer);
	
	app_timer_register(APP_TIMER_TIMEOUT, app_timer_callback, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

	// Top layer
  speed_layer = text_layer_create(GRect(0, 20, 144, 84));
  text_layer_set_text_color(speed_layer, GColorWhite);
  text_layer_set_background_color(speed_layer, GColorClear);
  text_layer_set_font(speed_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_48_NUMBERS)));
  text_layer_set_text_alignment(speed_layer, GTextAlignmentCenter);

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
	circle_layer = layer_create(GRect(window_layer_frame.origin.x + window_layer_frame.size.w - 4, 
																		window_layer_frame.origin.y, 
																		4, 4));																		
	layer_set_update_proc(circle_layer, circle_layer_update_proc);	
  
	layer_add_child(window_layer, left_bg_layer);
	layer_add_child(window_layer, right_bg_layer);
  layer_add_child(window_layer, text_layer_get_layer(speed_layer));
  layer_add_child(window_layer, text_layer_get_layer(left_value_layer));
	layer_add_child(window_layer, text_layer_get_layer(right_value_layer));	
  layer_add_child(window_layer, text_layer_get_layer(left_title_layer));
	layer_add_child(window_layer, text_layer_get_layer(right_title_layer));	
	layer_add_child(window_layer, circle_layer);	
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);

  text_layer_destroy(speed_layer);
  text_layer_destroy(left_value_layer);
  text_layer_destroy(right_value_layer);	
  text_layer_destroy(left_title_layer);
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
    TupletCString(SPEED_KEY, "--.-"),
    TupletCString(HDG_KEY, "---°"),
    TupletCString(AWA_KEY, "---°")				
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