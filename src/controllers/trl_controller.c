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

TRLTopView* trl_controller_top_view_create(char *title, char *value)
{
	TRLTopView *top_view = malloc(sizeof(TRLTopView));
	memset(top_view, 0, sizeof(TRLTopView));

	// Top layer	
	top_view->root_layer = layer_create(GRect(0, 0, 144, 104));	
	
  top_view->top_value_layer = text_layer_create(GRect(0, 10, 144, 54));
  text_layer_set_text_color(top_view->top_value_layer, GColorWhite);
  text_layer_set_background_color(top_view->top_value_layer, GColorClear);
  text_layer_set_font(top_view->top_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_48_NUMBERS)));
  text_layer_set_text_alignment(top_view->top_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(top_view->top_value_layer, GTextOverflowModeWordWrap);
	text_layer_set_text(top_view->top_value_layer, value);

  top_view->top_title_layer = text_layer_create(GRect(0, 64, 144, 40));
  text_layer_set_text_color(top_view->top_title_layer, GColorWhite);
  text_layer_set_background_color(top_view->top_title_layer, GColorClear);
  text_layer_set_font(top_view->top_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_LIGHT_24_TEXT)));
  text_layer_set_text_alignment(top_view->top_title_layer, GTextAlignmentCenter);
	text_layer_set_text(top_view->top_title_layer, title);
	
	layer_add_child(top_view->root_layer, text_layer_get_layer(top_view->top_value_layer));
	layer_add_child(top_view->root_layer, text_layer_get_layer(top_view->top_title_layer));	
	
	return top_view;
}

void trl_controller_top_view_destroy(TRLTopView *top_view)
{
  text_layer_destroy(top_view->top_value_layer), top_view->top_value_layer = NULL;
  text_layer_destroy(top_view->top_title_layer), top_view->top_title_layer = NULL;	
	layer_destroy(top_view->root_layer), top_view->root_layer = NULL;		
	free(top_view);
}

TRLBottomView* trl_controller_bottom_view_create(char *left_title, char *left_value, char *right_title, char *right_value)
{
	TRLBottomView *bottom_view = malloc(sizeof(TRLBottomView));
	memset(bottom_view, 0, sizeof(TRLBottomView));
		
	// bottom layer
	bottom_view->root_layer = layer_create(GRect(0, 104, 144, 64));
	
	// Left bottom layer
	bottom_view->left_bg_layer = layer_create(GRect(0, 0, 70, 64));
	layer_set_update_proc(bottom_view->left_bg_layer, rounded_layer_update_proc);	

  bottom_view->left_value_layer = text_layer_create(GRect(0, 0, 70, 24));
  text_layer_set_background_color(bottom_view->left_value_layer, GColorClear);
  text_layer_set_text_color(bottom_view->left_value_layer, GColorBlack);	
  text_layer_set_font(bottom_view->left_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(bottom_view->left_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->left_value_layer, GTextOverflowModeWordWrap);	
	text_layer_set_text(bottom_view->left_value_layer, left_value);
  
	bottom_view->left_title_layer = text_layer_create(GRect(0, 27, 70, 37));
  text_layer_set_background_color(bottom_view->left_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->left_title_layer, GColorBlack);
  text_layer_set_font(bottom_view->left_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(bottom_view->left_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->left_title_layer, left_title);
	
	// Right bottom layer
	bottom_view->right_bg_layer = layer_create(GRect(74, 0, 70, 64));
	layer_set_update_proc(bottom_view->right_bg_layer, rounded_layer_update_proc);

  bottom_view->right_value_layer = text_layer_create(GRect(74, 0, 70, 24));
  text_layer_set_background_color(bottom_view->right_value_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_value_layer, GColorBlack);
  text_layer_set_font(bottom_view->right_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(bottom_view->right_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->right_value_layer, GTextOverflowModeWordWrap);		
	text_layer_set_text(bottom_view->right_value_layer, right_value);
				
  bottom_view->right_title_layer = text_layer_create(GRect(74, 27, 70, 37));
  text_layer_set_background_color(bottom_view->right_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_title_layer, GColorBlack);
  text_layer_set_font(bottom_view->right_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(bottom_view->right_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->right_title_layer, right_title);

	// add to root layer
	layer_add_child(bottom_view->root_layer, bottom_view->left_bg_layer);	
	layer_add_child(bottom_view->root_layer, text_layer_get_layer(bottom_view->left_value_layer));
	layer_add_child(bottom_view->root_layer, text_layer_get_layer(bottom_view->left_title_layer));	

	layer_add_child(bottom_view->root_layer, bottom_view->right_bg_layer);	
	layer_add_child(bottom_view->root_layer, text_layer_get_layer(bottom_view->right_value_layer));
	layer_add_child(bottom_view->root_layer, text_layer_get_layer(bottom_view->right_title_layer));		
	
	return bottom_view;
}

void trl_controller_bottom_view_destroy(TRLBottomView *bottom_view)
{
  text_layer_destroy(bottom_view->left_value_layer), bottom_view->left_value_layer = NULL;
  text_layer_destroy(bottom_view->left_title_layer), bottom_view->left_title_layer = NULL;
  text_layer_destroy(bottom_view->right_value_layer), bottom_view->right_value_layer = NULL;		
  text_layer_destroy(bottom_view->right_title_layer), bottom_view->right_title_layer = NULL;	
	layer_destroy(bottom_view->left_bg_layer), bottom_view->left_bg_layer = NULL;
	layer_destroy(bottom_view->right_bg_layer), bottom_view->right_bg_layer = NULL;	
  layer_destroy(bottom_view->root_layer), bottom_view->root_layer = NULL;			
	free(bottom_view);
}

void trl_controller_load(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_load");
	TRLController *trl_controller = controller_get_trl_controller(controller);
  Layer *window_layer = window_get_root_layer(trl_controller->controller.window);
	
	// create top views
	list_add_tail(&trl_controller_top_view_create("SPEED", values.speed)->list, &trl_controller->top_view_list);
	list_add_tail(&trl_controller_top_view_create("DEPTH", values.depth)->list, &trl_controller->top_view_list);
	list_add_tail(&trl_controller_top_view_create("DTW", values.dtw)->list, &trl_controller->top_view_list);		
	list_add_tail(&trl_controller_top_view_create("TWD", values.twd)->list, &trl_controller->top_view_list);		
		
	// create bottom views
	list_add_tail(&trl_controller_bottom_view_create("HDG", values.hdg, "AWA", values.awa)->list, &trl_controller->bottom_view_list);
	
	// add 1. top view
	trl_controller->current_top_view = list_entry(trl_controller->top_view_list.next, TRLTopView, list);
  layer_add_child(window_layer, trl_controller->current_top_view->root_layer);
	// add 1. bottom view
	trl_controller->current_bottom_view = list_entry(trl_controller->bottom_view_list.next, TRLBottomView, list);	
  layer_add_child(window_layer, trl_controller->current_bottom_view->root_layer);		
}

void trl_controller_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_unload");
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	
	// destroy top views
	struct list_head *ptr, *tmp;	
	TRLTopView *top_view;		
  list_for_each_safe(ptr, tmp, &trl_controller->top_view_list) {
     top_view = list_entry(ptr, TRLTopView, list);
		 list_del(ptr);	 
		 trl_controller_top_view_destroy(top_view);		 
	}		
	trl_controller->current_top_view = NULL;
	
	// destroy bottom views
	TRLBottomView *bottom_view;		
  list_for_each_safe(ptr, tmp, &trl_controller->bottom_view_list) {
     bottom_view = list_entry(ptr, TRLBottomView, list);
		 list_del(ptr);	 
		 trl_controller_bottom_view_destroy(bottom_view);		 
	}			
	trl_controller->current_bottom_view = NULL;
}

void trl_controller_redaw(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_redraw");		
	TRLController *trl_controller = controller_get_trl_controller(controller);		
	/*
	text_layer_set_text(trl_controller->top_title_layer, "SPEED");	
	text_layer_set_text(trl_controller->top_value_layer, values.speed);
	text_layer_set_text(trl_controller->left_title_layer, "HDG");	
	text_layer_set_text(trl_controller->left_value_layer, values.hdg);	
	text_layer_set_text(trl_controller->right_title_layer, "AWA");			
	text_layer_set_text(trl_controller->right_value_layer, values.awa);			
	*/
}

void trl_controller_destroy(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_destroy");	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	free(trl_controller);
}


static PropertyAnimation *current_prop_animation = NULL;
static PropertyAnimation *next_prop_animation = NULL;
static TRLController* trl_controller;
	
static void animation_started(Animation *animation, void *data) {
	TRLTopView *top_view = (TRLTopView*)data;
	Layer *window_root_layer = window_get_root_layer(trl_controller->controller.window);
	layer_add_child(window_root_layer, top_view->root_layer);
}

static void animation_stopped(Animation *animation, bool finished, void *data) {
	trl_controller->current_top_view = (TRLTopView*)data;
}

static void destroy_property_animation(PropertyAnimation **prop_animation) {
  if (*prop_animation == NULL) {
    return;
  }

  if (animation_is_scheduled((Animation*) *prop_animation)) {
    animation_unschedule((Animation*) *prop_animation);
  }

  property_animation_destroy(*prop_animation);
  *prop_animation = NULL;
}

void trl_controller_click_handler_prev_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_prev_value");
	destroy_property_animation(&current_prop_animation);	
	destroy_property_animation(&next_prop_animation);	
		
	TRLTopView *current_top_view = trl_controller->current_top_view;
	Layer *current_top_layer = current_top_view->root_layer;
	
	GRect current_from_frame = layer_get_frame(current_top_layer);
	GRect current_to_frame = current_from_frame;
	current_to_frame.origin.x = 144;
	current_prop_animation = property_animation_create_layer_frame(current_top_layer, &current_from_frame, &current_to_frame);
	animation_set_duration((Animation*)current_prop_animation, 400);
	animation_set_curve((Animation*)current_prop_animation, AnimationCurveEaseIn);	

	TRLTopView *next_top_view;
	if (current_top_view->list.prev != &trl_controller->top_view_list)
		next_top_view = list_entry(current_top_view->list.prev, TRLTopView, list);
	else
		next_top_view = list_entry(trl_controller->top_view_list.prev, TRLTopView, list);
	Layer *next_top_layer = next_top_view->root_layer;

	GRect next_from_frame = current_from_frame;
	next_from_frame.origin.x = -144; 		
	GRect next_to_frame = current_from_frame; 
	
	next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
	animation_set_duration((Animation*)next_prop_animation, 400);
	animation_set_curve((Animation*)next_prop_animation, AnimationCurveEaseIn);	  	
	animation_set_handlers((Animation*)next_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)animation_started,		
    .stopped = (AnimationStoppedHandler)animation_stopped,
  }, next_top_view);
		
	animation_schedule((Animation*)current_prop_animation);				
	animation_schedule((Animation*)next_prop_animation);					
}

void trl_controller_click_handler_next_top_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_top_value");			
	destroy_property_animation(&current_prop_animation);	
	
	TRLTopView *current_top_view = trl_controller->current_top_view;
	Layer *current_top_layer = current_top_view->root_layer;
	
	GRect current_from_frame = layer_get_frame(current_top_layer);
	GRect current_to_frame = current_from_frame;
	current_to_frame.origin.x = -144;
	current_prop_animation = property_animation_create_layer_frame(current_top_layer, &current_from_frame, &current_to_frame);
	animation_set_duration((Animation*)current_prop_animation, 400);
	animation_set_curve((Animation*)current_prop_animation, AnimationCurveEaseIn);
	
	TRLTopView *next_top_view;
	if (current_top_view->list.next != &trl_controller->top_view_list)
		next_top_view = list_entry(current_top_view->list.next, TRLTopView, list);
	else
		next_top_view = list_entry(trl_controller->top_view_list.next, TRLTopView, list);
	Layer *next_top_layer = next_top_view->root_layer;
	
	GRect next_from_frame = layer_get_frame(next_top_layer);
	next_from_frame.origin.x = 144;
	GRect next_to_frame = next_from_frame; 
	next_to_frame.origin.x = 0; 	
	
	next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
	animation_set_duration((Animation*)next_prop_animation, 400);
	animation_set_curve((Animation*)next_prop_animation, AnimationCurveEaseIn);	  			
  animation_set_handlers((Animation*)next_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)animation_started,
    .stopped = (AnimationStoppedHandler)animation_stopped,
  }, next_top_view);
	
	animation_schedule((Animation*)current_prop_animation);
	animation_schedule((Animation*)next_prop_animation);						
}	

void trl_controller_click_config_provider(Window *window) 
{
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)trl_controller_click_handler_prev_value);	
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)trl_controller_click_handler_next_top_value);
}

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers) 
{	
	trl_controller = malloc(sizeof(TRLController));
	memset(trl_controller, 0, sizeof(TRLController));
	INIT_LIST_HEAD(&trl_controller->top_view_list);
	INIT_LIST_HEAD(&trl_controller->bottom_view_list);			
	__controller_init(&trl_controller->controller, window, handlers, (ControllerVTable) {
		.load = trl_controller_load,
		.unload = trl_controller_unload,
		.redraw = trl_controller_redaw,
		.destroy = trl_controller_destroy
	});	
  window_set_click_config_provider(window, (ClickConfigProvider)trl_controller_click_config_provider);		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController %p", trl_controller);		
	return trl_controller;
}

Controller* trl_controller_get_controller(TRLController* trl_controller) {
	return &trl_controller->controller;
}
