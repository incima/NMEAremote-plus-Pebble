#include "common.h"
#include "trl_controller.h"

TRLController* controller_get_trl_controller(Controller *controller) {
	void *controller_pptr = (void*)(controller);		
	return (TRLController*)controller_pptr;
}

static void rounded_layer_update_proc(Layer* layer, GContext *context) 
{	
	GRect layer_bounds = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);	
	graphics_fill_rect(context, layer_bounds, 4, GCornersAll);
}

void trl_controller_load(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_load");
	TRLController *trl_controller = controller_get_trl_controller(controller);

  Layer *window_layer = window_get_root_layer(trl_controller->controller.window);

	// Top layer
  trl_controller->top_value_layer = text_layer_create(GRect(0, 10, 144, 54));
  text_layer_set_text_color(trl_controller->top_value_layer, GColorWhite);
  text_layer_set_background_color(trl_controller->top_value_layer, GColorClear);
  text_layer_set_font(trl_controller->top_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_48_NUMBERS)));
  text_layer_set_text_alignment(trl_controller->top_value_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->top_value_layer, "");

  trl_controller->top_title_layer = text_layer_create(GRect(0, 64, 144, 40));
  text_layer_set_text_color(trl_controller->top_title_layer, GColorWhite);
  text_layer_set_background_color(trl_controller->top_title_layer, GColorClear);
  text_layer_set_font(trl_controller->top_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_LIGHT_24_TEXT)));
  text_layer_set_text_alignment(trl_controller->top_title_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->top_title_layer, "");
		
	// Left layer
	trl_controller->left_bg_layer = layer_create(GRect(0, 104, 70, 64));
	layer_set_update_proc(trl_controller->left_bg_layer, rounded_layer_update_proc);	

  trl_controller->left_value_layer = text_layer_create(GRect(0, 106, 70, 24));
  text_layer_set_background_color(trl_controller->left_value_layer, GColorClear);
  text_layer_set_text_color(trl_controller->left_value_layer, GColorBlack);	
  text_layer_set_font(trl_controller->left_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(trl_controller->left_value_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->left_value_layer, "");
  
	trl_controller->left_title_layer = text_layer_create(GRect(0, 133, 70, 37));
  text_layer_set_background_color(trl_controller->left_title_layer, GColorClear);
	text_layer_set_text_color(trl_controller->left_title_layer, GColorBlack);
  text_layer_set_font(trl_controller->left_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(trl_controller->left_title_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->left_title_layer, "");
	
	// Right layer
	trl_controller->right_bg_layer = layer_create(GRect(74, 104, 70, 64));
	layer_set_update_proc(trl_controller->right_bg_layer, rounded_layer_update_proc);

  trl_controller->right_value_layer = text_layer_create(GRect(74, 106, 70, 24));
  text_layer_set_background_color(trl_controller->right_value_layer, GColorClear);
	text_layer_set_text_color(trl_controller->right_value_layer, GColorBlack);
  text_layer_set_font(trl_controller->right_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(trl_controller->right_value_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->right_value_layer, "");
				
  trl_controller->right_title_layer = text_layer_create(GRect(74, 133, 70, 37));
  text_layer_set_background_color(trl_controller->right_title_layer, GColorClear);
	text_layer_set_text_color(trl_controller->right_title_layer, GColorBlack);
  text_layer_set_font(trl_controller->right_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(trl_controller->right_title_layer, GTextAlignmentCenter);
	text_layer_set_text(trl_controller->right_title_layer, "");
		
	layer_add_child(window_layer, trl_controller->left_bg_layer);
	layer_add_child(window_layer, trl_controller->right_bg_layer);
  layer_add_child(window_layer, text_layer_get_layer(trl_controller->top_value_layer));
	layer_add_child(window_layer, text_layer_get_layer(trl_controller->top_title_layer));	
  layer_add_child(window_layer, text_layer_get_layer(trl_controller->left_value_layer));
	layer_add_child(window_layer, text_layer_get_layer(trl_controller->right_value_layer));	
  layer_add_child(window_layer, text_layer_get_layer(trl_controller->left_title_layer));
	layer_add_child(window_layer, text_layer_get_layer(trl_controller->right_title_layer));		

}

void trl_controller_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_unload");
	TRLController *trl_controller = controller_get_trl_controller(controller);	
  text_layer_destroy(trl_controller->top_value_layer);
  text_layer_destroy(trl_controller->top_title_layer);	
  text_layer_destroy(trl_controller->left_value_layer);
  text_layer_destroy(trl_controller->left_title_layer);
  text_layer_destroy(trl_controller->right_value_layer);		
  text_layer_destroy(trl_controller->right_title_layer);	
	layer_destroy(trl_controller->left_bg_layer);
	layer_destroy(trl_controller->right_bg_layer);	
}

void trl_controller_redaw(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_redraw");		
	TRLController *trl_controller = controller_get_trl_controller(controller);		
	text_layer_set_text(trl_controller->top_title_layer, trl_controller->top_title);	
	text_layer_set_text(trl_controller->top_value_layer, trl_controller->top_value);
	text_layer_set_text(trl_controller->left_title_layer, trl_controller->left_title);	
	text_layer_set_text(trl_controller->left_value_layer, trl_controller->left_value);	
	text_layer_set_text(trl_controller->right_title_layer, trl_controller->right_title);			
	text_layer_set_text(trl_controller->right_value_layer, trl_controller->right_value);			
}

void trl_controller_cancel_redaw(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_cancel_redraw");	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	text_layer_set_text(trl_controller->top_title_layer, trl_controller->top_title);
	text_layer_set_text(trl_controller->top_value_layer, trl_controller->top_default_value);	
	text_layer_set_text(trl_controller->left_title_layer, trl_controller->left_title);	
	text_layer_set_text(trl_controller->left_value_layer, trl_controller->left_default_value);	
	text_layer_set_text(trl_controller->right_title_layer, trl_controller->right_title);			
	text_layer_set_text(trl_controller->right_value_layer, trl_controller->right_default_value);			
}

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers) 
{
	TRLController* trl_controller = malloc(sizeof(TRLController));
	memset(trl_controller, 0, sizeof(TRLController));
	__controller_init(&trl_controller->controller, window, handlers, (ControllerVTable) {
		.load = trl_controller_load,
		.unload = trl_controller_unload,
		.redraw = trl_controller_redaw,
		.cancel_redraw = trl_controller_cancel_redaw
	});	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController %p", trl_controller);		
	return trl_controller;
}

Controller* trl_controller_get_controller(TRLController* trl_controller) {
	return &trl_controller->controller;
}