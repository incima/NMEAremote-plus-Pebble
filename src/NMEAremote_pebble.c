#include "pebble.h"

static Window *window;

static TextLayer *speed_layer;
static char speed[16];

static TextLayer *hdg_layer;
static char hdg[16];

static TextLayer *awa_layer;
static char awa[16];

static AppSync sync;
static uint8_t sync_buffer[48];

enum NMEAkey {
	SPEED_KEY = 0,  
	HDG_KEY = 1,
	AWA_KEY = 2,	  	
};

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case SPEED_KEY:
      text_layer_set_text(speed_layer, new_tuple->value->cstring);
      break;
    case HDG_KEY:
      text_layer_set_text(hdg_layer, new_tuple->value->cstring);
      break;
    case AWA_KEY:
      text_layer_set_text(awa_layer, new_tuple->value->cstring);
      break;			
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

	int y=10;
  speed_layer = text_layer_create(GRect(0, y, 144, 40));
  text_layer_set_text_color(speed_layer, GColorWhite);
  text_layer_set_background_color(speed_layer, GColorClear);
  text_layer_set_font(speed_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(speed_layer, GTextAlignmentCenter);

	y += 40+10;
  hdg_layer = text_layer_create(GRect(0, y, 144, 40));
  text_layer_set_text_color(hdg_layer, GColorWhite);
  text_layer_set_background_color(hdg_layer, GColorClear);
  text_layer_set_font(hdg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(hdg_layer, GTextAlignmentCenter);

	y += 40+10;
  awa_layer = text_layer_create(GRect(0, y, 144, 40));
  text_layer_set_text_color(awa_layer, GColorWhite);
  text_layer_set_background_color(awa_layer, GColorClear);
  text_layer_set_font(awa_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(awa_layer, GTextAlignmentCenter);

  Tuplet initial_values[] = {
    TupletCString(SPEED_KEY, "--.-"),
    TupletCString(HDG_KEY, "---°"),
    TupletCString(AWA_KEY, "---°")				
  };
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  layer_add_child(window_layer, text_layer_get_layer(speed_layer));
  layer_add_child(window_layer, text_layer_get_layer(hdg_layer));
	layer_add_child(window_layer, text_layer_get_layer(awa_layer));	
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);

  text_layer_destroy(speed_layer);
  text_layer_destroy(hdg_layer);
  text_layer_destroy(awa_layer);	
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
}

static void deinit() {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}