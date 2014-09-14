#include "common/common.h"

#include "trl_controller.h"

TRLController* controller_get_trl_controller(Controller *controller) 
{
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
	
	layer_add_child(window_layer, view_get_root_layer(&trl_controller->bottom_switch_view->base));
	
	controller_load_update_layer(controller);		
	
	// add to root
	trl_switch_view_next(trl_controller->top_switch_view, false);			
	trl_switch_view_next(trl_controller->bottom_switch_view, false);			
		
}

void trl_controller_unload(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_unload");
	TRLController *trl_controller = controller_get_trl_controller(controller);		
	view_unload(&trl_controller->top_switch_view->base);
	view_unload(&trl_controller->bottom_switch_view->base);	
}

void trl_controller_destroy(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_destroy");	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	free(trl_controller);
}

bool trl_controller_click_handler_prev_value(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_prev_value");
	
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	switch (trl_controller->control_state) {
		case TRLControlStateTop:
			trl_switch_view_prev(trl_controller->top_switch_view, true);		
			return true;
		case TRLControlStateBottom:
			trl_switch_view_prev(trl_controller->bottom_switch_view, true);				
			return true;
		default:
			break;
	}
	return false;
}

void trl_controller_click_handler_next_top_value(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_top_value");			
	TRLController *trl_controller = controller_get_trl_controller(controller);	
	trl_switch_view_next(trl_controller->top_switch_view, true);
	trl_controller->control_state = TRLControlStateTop;	
}	

void trl_controller_click_handler_next_bottom_value(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_click_handler_next_bottom_value");			
	TRLController *trl_controller = controller_get_trl_controller(controller);
	trl_switch_view_next(trl_controller->bottom_switch_view, true);
	trl_controller->control_state = TRLControlStateBottom;	
}	

bool trl_controller_on_button_up(Controller* controller, ClickRecognizerRef recognizer)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController trl_controller_on_button_up");	
	
	TRLController *trl_controller = controller_get_trl_controller(controller);
	ButtonId button_id = click_recognizer_get_button_id(recognizer);
	switch (button_id) {
	case BUTTON_ID_BACK:		
		return trl_controller_click_handler_prev_value(controller);				
	
	case BUTTON_ID_UP:
		trl_controller->control_state = TRLControlStateTop;
		trl_controller_click_handler_next_top_value(controller);						
		return true;			
			
	case BUTTON_ID_DOWN:		
		trl_controller->control_state = TRLControlStateBottom;						
		trl_controller_click_handler_next_bottom_value(controller);			
		return true;
			
	default: 		
		trl_controller->control_state = TRLControlStateNone;		
		break;
	}	
	return false;
}

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers) 
{	
	TRLController *trl_controller = malloc(sizeof(TRLController));
	memset(trl_controller, 0, sizeof(TRLController));
	__controller_init(&trl_controller->controller, window, handlers, (ControllerVTable) {
		.load = trl_controller_load,
		.unload = trl_controller_unload,
		.destroy = trl_controller_destroy,
		.on_button_up = trl_controller_on_button_up,
		.on_button_down = NULL
	});	  		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLController %p", trl_controller);
	trl_controller->control_state = TRLControlStateNone;
	return trl_controller;
}

Controller* trl_controller_get_controller(TRLController* trl_controller) {
	return &trl_controller->controller;
}
