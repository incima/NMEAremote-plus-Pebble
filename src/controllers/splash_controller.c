#include "common.h"
#include "splash_controller.h"

static void splash_circle_layer_update_proc(Layer *layer, GContext *context)
{
	GRect layer_frame = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);			
	graphics_fill_circle(context, (GPoint){layer_frame.size.w/2, layer_frame.size.h/2}, 2);				
}

SplashController* controller_get_splash_controller(Controller* controller) 
{
	void *controller_pptr = (void*)(controller);		
	return (SplashController*)controller_pptr;	
}

void splash_controller_load(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_load");		
	SplashController* splash_controller = controller_get_splash_controller(controller);
	Layer* window_layer = window_get_root_layer(splash_controller->controller.window);
	GRect window_frame = layer_get_bounds(window_layer);
	GRect window_bounds = layer_get_bounds(window_layer);
  splash_controller->splash_layer = bitmap_layer_create(window_bounds);
  layer_add_child(window_layer, bitmap_layer_get_layer(splash_controller->splash_layer));
	
	uint32_t ypos = window_frame.origin.y + window_frame.size.h - 20;
	splash_controller->circle_layer1 = layer_create(GRect(window_frame.origin.x + window_frame.size.w/2 - 12, 
																		ypos, 5, 5));																		
	layer_set_update_proc(splash_controller->circle_layer1, splash_circle_layer_update_proc);	

	splash_controller->circle_layer2 = layer_create(GRect(window_frame.origin.x + window_frame.size.w/2, 
																		ypos, 5, 5));																			
	layer_set_update_proc(splash_controller->circle_layer2, splash_circle_layer_update_proc);	
	
	splash_controller->circle_layer3 = layer_create(GRect(window_frame.origin.x + window_frame.size.w/2 +12, 
																			ypos, 5, 5));
	layer_set_update_proc(splash_controller->circle_layer3, splash_circle_layer_update_proc);			
}

void splash_controller_unload(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_unload");		
	SplashController* splash_controller = controller_get_splash_controller(controller);
	if (splash_controller->splash_bitmap)
		gbitmap_destroy(splash_controller->splash_bitmap);
	bitmap_layer_destroy(splash_controller->splash_layer);
}

void splash_controller_redraw(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_redraw");		
	SplashController* splash_controller = controller_get_splash_controller(controller);
	if (splash_controller->circle_counter == 4) {
		splash_controller->circle_counter++;
		if (splash_controller->controller.handlers.did_finish) {
			splash_controller->controller.handlers.did_finish(splash_controller_get_controller(splash_controller));
		}
		return;
	} 
	
	if (splash_controller->splash_bitmap)
		bitmap_layer_set_bitmap(splash_controller->splash_layer, splash_controller->splash_bitmap);		
	
	Layer *window_layer = window_get_root_layer(splash_controller->controller.window);
	if (splash_controller->circle_counter == 1) {
	  layer_add_child(window_layer, splash_controller->circle_layer1);		
		layer_mark_dirty(splash_controller->circle_layer1);
	}
	if (splash_controller->circle_counter == 2) {	
	  layer_add_child(window_layer, splash_controller->circle_layer2);		
		layer_mark_dirty(splash_controller->circle_layer2);		
	}
	if (splash_controller->circle_counter == 3) {		
	  layer_add_child(window_layer, splash_controller->circle_layer3);		
		layer_mark_dirty(splash_controller->circle_layer3);		
	}
	++splash_controller->circle_counter;
}

void splash_controller_cancel_redraw(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_cancel_redraw");			
}

void splash_controller_destroy(Controller* controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_destroy");			
	SplashController* splash_controller = controller_get_splash_controller(controller);
	free(splash_controller);
}

SplashController* splash_controller_create(Window* window, ControllerHandlers handlers)
{
	SplashController* splash_controller = malloc(sizeof(SplashController));
	memset(splash_controller, 0, sizeof(SplashController));
	__controller_init(&splash_controller->controller, window, handlers, (ControllerVTable) {
		.load = splash_controller_load,
		.unload = splash_controller_unload,
		.redraw = splash_controller_redraw,
		.cancel_redraw = splash_controller_cancel_redraw,
		.destroy = splash_controller_destroy
	});
	return splash_controller;
}

Controller* splash_controller_get_controller(SplashController* splash_controller)
{
	return &splash_controller->controller;
}

void splash_controller_set_bitmap_from_resource(SplashController* splash_controller, uint32_t resource_id) 
{
	if (splash_controller->splash_bitmap)
		gbitmap_destroy(splash_controller->splash_bitmap);	
  splash_controller->splash_bitmap = gbitmap_create_with_resource(resource_id);
}