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
	trl_controller->top_switch_view = trl_switch_view_create(trl_controller->controller.window, GRect(0, 0, 144, 104));
		
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create("SPEED", values.speed)->base);
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create("DEPTH", values.depth)->base);
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create("BTW", values.btw)->base);
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create("DTW", values.dtw)->base);	
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create("TWD", values.twd)->base);
	trl_switch_view_add_view(trl_controller->top_switch_view, 
													&trl_top_view_create(values.current_date, values.current_time)->base);

	trl_switch_view_next(trl_controller->top_switch_view, false);
	
  layer_add_child(window_layer, view_get_root_layer(&trl_controller->top_switch_view->base));
					
	// create bottom views
	trl_controller->bottom_switch_view = trl_switch_view_create(trl_controller->controller.window, GRect(0, 104, 144, 64));
	
	trl_switch_view_add_view(trl_controller->bottom_switch_view, 
													&trl_bottom_view_create("HDG", values.hdg, "AWA", values.awa)->base);	
	trl_switch_view_add_view(trl_controller->bottom_switch_view, 
													&trl_bottom_view_create("DTW", values.dtw, "TTG", values.ttg)->base);											
	trl_switch_view_add_view(trl_controller->bottom_switch_view, 
													&trl_bottom_view_create("COG", values.cog, "XTE", values.xte)->base);	
	trl_switch_view_add_view(trl_controller->bottom_switch_view, 
													&trl_bottom_view_create("SOG", values.sog, "TTG", values.ttg)->base);	
	trl_switch_view_add_view(trl_controller->bottom_switch_view, 
													&trl_bottom_view_create("TWS", values.tws, "BFT", values.bft)->base);	

	trl_switch_view_next(trl_controller->bottom_switch_view, false);	
	
  layer_add_child(window_layer, view_get_root_layer(&trl_controller->bottom_switch_view->base));	
	
	controller_load_update_layer(controller);		
}

void trl_controller_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_unload");
	TRLController *trl_controller = controller_get_trl_controller(controller);		
	view_unload(&trl_controller->top_switch_view->base);
	view_unload(&trl_controller->bottom_switch_view->base);	
}

static TRLController* trl_controller;

void trl_controller_click_handler_prev_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_prev_value");
	switch (trl_controller->control_state) {
		case TRLControlStateTop:
			trl_switch_view_prev(trl_controller->top_switch_view, true);		
			break;
		case TRLControlStateBottom:
			trl_switch_view_prev(trl_controller->bottom_switch_view, true);				
			break;
		default:
			break;
	}
}

void trl_controller_click_handler_next_top_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_top_value");			
	trl_switch_view_next(trl_controller->top_switch_view, true);
	trl_controller->control_state = TRLControlStateTop;	
}	

void trl_controller_click_handler_next_bottom_value(ClickRecognizerRef recognizer, Window *window)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_bottom_value");			
	trl_switch_view_next(trl_controller->bottom_switch_view, true);
	trl_controller->control_state = TRLControlStateBottom;	
}	

void trl_controller_click_config_provider(Window *window) 
{
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)trl_controller_click_handler_prev_value);	
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)trl_controller_click_handler_next_top_value);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)trl_controller_click_handler_next_bottom_value);	
}

void trl_controller_destroy(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_destroy");	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	free(trl_controller);
}

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers) 
{	
	trl_controller = malloc(sizeof(TRLController));
	memset(trl_controller, 0, sizeof(TRLController));
	__controller_init(&trl_controller->controller, window, handlers, (ControllerVTable) {
		.load = trl_controller_load,
		.unload = trl_controller_unload,
		.destroy = trl_controller_destroy
	});	
  //window_set_click_config_provider(window, (ClickConfigProvider)trl_controller_click_config_provider);		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController %p", trl_controller);		
	return trl_controller;
}

Controller* trl_controller_get_controller(TRLController* trl_controller) {
	return &trl_controller->controller;
}
