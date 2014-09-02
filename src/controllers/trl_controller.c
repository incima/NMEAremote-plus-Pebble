#include "common.h"
#include "trl_controller.h"

TRLController* controller_get_trl_controller(Controller *controller) {
	void *controller_pptr = (void*)(controller);		
	return (TRLController*)controller_pptr;
}
		
void trl_controller_load(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_load");
	TRLController *trl_controller = controller_get_trl_controller(controller);
  Layer *window_layer = window_get_root_layer(trl_controller->controller.window);
	
	// create top views
	list_add_tail(&view_list_entry_create(&trl_top_view_create("SPEED", values.speed)->base)->list, &trl_controller->top_view_list);
	list_add_tail(&view_list_entry_create(&trl_top_view_create("DEPTH", values.depth)->base)->list, &trl_controller->top_view_list);
	list_add_tail(&view_list_entry_create(&trl_top_view_create("DTW", values.dtw)->base)->list, &trl_controller->top_view_list);		
	list_add_tail(&view_list_entry_create(&trl_top_view_create("TWD", values.twd)->base)->list, &trl_controller->top_view_list);		
		
	// create bottom views
	list_add_tail(&view_list_entry_create(&trl_bottom_view_create("HDG", values.hdg, "AWA", values.awa)->base)->list, &trl_controller->bottom_view_list);
	
	// add 1. top view
	trl_controller->curr_top_view =  list_entry(trl_controller->top_view_list.next, ViewListEntry, list);
  layer_add_child(window_layer, view_get_root_layer(trl_controller->curr_top_view->view));

	// add 1. bottom view
	trl_controller->curr_bottom_view = list_entry(trl_controller->bottom_view_list.next, ViewListEntry, list);	
  layer_add_child(window_layer, view_get_root_layer(trl_controller->curr_bottom_view->view));		
}

void trl_controller_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_unload");
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	
	// destroy top views
	struct list_head *ptr, *tmp;	
	ViewListEntry *top_entry;
	TRLTopView *top_view;		
  list_for_each_safe(ptr, tmp, &trl_controller->top_view_list) {
		top_entry = list_entry(ptr, ViewListEntry, list);
    top_view = trl_top_view_from_view(top_entry->view);
		list_del(ptr);	 
		trl_top_view_destroy(top_view);		 
		free(top_entry);
	}		
	trl_controller->curr_top_view = NULL;
	
	// destroy bottom views
	ViewListEntry *bottom_entry;
	TRLBottomView	*bottom_view;
  list_for_each_safe(ptr, tmp, &trl_controller->bottom_view_list) {
		bottom_entry = list_entry(ptr, ViewListEntry, list);
    bottom_view = trl_bottom_view_from_view(bottom_entry->view);
		list_del(ptr);	 
		trl_bottom_view_destroy(bottom_view);		 
		free(bottom_entry);		 
	}			
	trl_controller->curr_bottom_view = NULL;
}

void trl_controller_redaw(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_redraw");		
	TRLController *trl_controller = controller_get_trl_controller(controller);		
}

void trl_controller_destroy(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_destroy");	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	free(trl_controller);
}

static PropertyAnimation *curr_prop_animation = NULL;
static PropertyAnimation *next_prop_animation = NULL;
static TRLController* trl_controller;

static void destroy_property_animation(PropertyAnimation **prop_animation) {
  if (prop_animation == NULL || *prop_animation == NULL) {
    return;
  }
  if (animation_is_scheduled((Animation*)*prop_animation)) {
    animation_unschedule((Animation*)*prop_animation);
  }
  property_animation_destroy(*prop_animation);
  *prop_animation = NULL;
}

static void curr_animation_started(Animation *animation, ViewListEntry *prev_entry) {
	
}

static void curr_animation_stopped(Animation *animation, bool finished, ViewListEntry *prev_entry) {
	layer_remove_from_parent(view_get_root_layer(prev_entry->view));
}
	
static void next_animation_started(Animation *animation, ViewListEntry *next_entry) {
	Layer *window_root_layer = window_get_root_layer(trl_controller->controller.window);
	layer_add_child(window_root_layer, view_get_root_layer(next_entry->view));
}

static void next_animation_stopped(Animation *animation, bool finished, ViewListEntry *next_entry) {
	trl_controller->curr_top_view = next_entry;
}

void trl_controller_click_handler_prev_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_prev_value");
	destroy_property_animation(&curr_prop_animation);	
	destroy_property_animation(&next_prop_animation);	
		
	ViewListEntry *curr_entry = trl_controller->curr_top_view;
	Layer *curr_top_layer = view_get_root_layer(curr_entry->view);
		
	GRect curr_from_frame = layer_get_frame(curr_top_layer);
	curr_from_frame.origin.x = 0;	
	GRect curr_to_frame = curr_from_frame;
	curr_to_frame.origin.x = 144;
	curr_prop_animation = property_animation_create_layer_frame(curr_top_layer, &curr_from_frame, &curr_to_frame);
	animation_set_duration((Animation*)curr_prop_animation, 400);
	animation_set_curve((Animation*)curr_prop_animation, AnimationCurveEaseIn);	
	animation_set_handlers((Animation*)curr_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)curr_animation_started,		
    .stopped = (AnimationStoppedHandler)curr_animation_stopped,
  }, curr_entry);
	
	ViewListEntry *next_entry;
	if (trl_controller->curr_top_view->list.prev != &trl_controller->top_view_list)
		next_entry = list_entry(trl_controller->curr_top_view->list.prev, ViewListEntry, list);
	else
		next_entry = list_entry(trl_controller->top_view_list.prev, ViewListEntry, list);
	Layer *next_top_layer = view_get_root_layer(next_entry->view);

	GRect next_from_frame = layer_get_frame(next_top_layer);
	next_from_frame.origin.x = -144; 		
	GRect next_to_frame = next_from_frame; 
	next_to_frame.origin.x = 0; 	
		
	next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
	animation_set_duration((Animation*)next_prop_animation, 400);
	animation_set_curve((Animation*)next_prop_animation, AnimationCurveEaseIn);	  	
	animation_set_handlers((Animation*)next_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)next_animation_started,		
    .stopped = (AnimationStoppedHandler)next_animation_stopped,
  }, next_entry);
	
	animation_schedule((Animation*)curr_prop_animation);				
	animation_schedule((Animation*)next_prop_animation);					
}

void trl_controller_click_handler_next_top_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_top_value");			
	destroy_property_animation(&curr_prop_animation);	
	destroy_property_animation(&next_prop_animation);	
		
	ViewListEntry *curr_entry = trl_controller->curr_top_view;
	Layer *curr_top_layer = view_get_root_layer(curr_entry->view);
	
	GRect curr_from_frame = layer_get_frame(curr_top_layer);
	curr_from_frame.origin.x = 0;
	GRect curr_to_frame = curr_from_frame;
	curr_to_frame.origin.x = -144;
	curr_prop_animation = property_animation_create_layer_frame(curr_top_layer, &curr_from_frame, &curr_to_frame);
	animation_set_duration((Animation*)curr_prop_animation, 400);
	animation_set_curve((Animation*)curr_prop_animation, AnimationCurveEaseIn);
	animation_set_handlers((Animation*)curr_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)curr_animation_started,		
    .stopped = (AnimationStoppedHandler)curr_animation_stopped,
  }, curr_entry);
	
	ViewListEntry *next_entry;
	if (trl_controller->curr_top_view->list.next != &trl_controller->top_view_list)
		next_entry = list_entry(trl_controller->curr_top_view->list.next, ViewListEntry, list);
	else
		next_entry = list_entry(trl_controller->top_view_list.next, ViewListEntry, list);
	Layer *next_top_layer = view_get_root_layer(next_entry->view);
	
	GRect next_from_frame = layer_get_frame(next_top_layer);
	next_from_frame.origin.x = 144;
	GRect next_to_frame = next_from_frame; 
	next_to_frame.origin.x = 0; 	
	
	next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
	animation_set_duration((Animation*)next_prop_animation, 400);
	animation_set_curve((Animation*)next_prop_animation, AnimationCurveEaseIn);	  			
  animation_set_handlers((Animation*)next_prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler)next_animation_started,
    .stopped = (AnimationStoppedHandler)next_animation_stopped,
  }, next_entry);
	
	animation_schedule((Animation*)curr_prop_animation);
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
