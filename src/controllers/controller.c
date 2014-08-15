#include "common.h"
#include "controller.h"

ControllerVTable* controller_get_vtable(Controller *controller) {
	return (ControllerVTable*)controller->vtable;
}

void __controller_init(Controller* controller, Window* window, ControllerHandlers handlers, ControllerVTable vtable) 
{
	memset(controller, 0, sizeof(Controller));
	ControllerVTable *vtable_ptr = malloc(sizeof(ControllerVTable));
	memset(vtable_ptr, 0, sizeof(ControllerVTable));
	memcpy(vtable_ptr, &vtable, sizeof(ControllerVTable));
	controller->vtable = vtable_ptr;
	controller->handlers = handlers;
	controller->window = window;
}

void controller_load(Controller* controller) 
{
	ControllerVTable *vtable = controller_get_vtable(controller);
	if (vtable->load)
		vtable->load(controller);	
	if (controller->handlers.did_load)
		controller->handlers.did_load(controller);
}

void controller_unload(Controller* controller) 
{
	ControllerVTable *vtable = controller_get_vtable(controller);	
	if (vtable->unload)
		vtable->unload(controller);
	if (controller->update_layer) 
		layer_destroy(controller->update_layer), controller->update_layer = NULL;
	if (controller->handlers.did_unload)
		controller->handlers.did_unload(controller);	
}

void controller_destroy(Controller* controller)
{
	ControllerVTable *vtable = controller_get_vtable(controller);	
	if (vtable->destroy)
		vtable->destroy(controller);
}

void controller_redraw_if_needed(Controller* controller) 
{
	ControllerVTable *vtable = controller_get_vtable(controller);	
	if (vtable->redraw)
		vtable->redraw(controller);			
}

void controller_cancel_redraw(Controller* controller) 
{
	ControllerVTable *vtable = controller_get_vtable(controller);	
	if (vtable->cancel_redraw)
		vtable->cancel_redraw(controller);				
}

static GColor controller_update_color;
void controller_update_layer_update_proc(Layer *layer, GContext *context)
{
	GRect layer_frame = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, controller_update_color);			
	graphics_fill_circle(context, (GPoint){layer_frame.size.w/2, layer_frame.size.h/2}, 2);				
}

void controller_load_update_layer(Controller* controller) 
{
	Layer* window_layer = window_get_root_layer(controller->window);
	GRect window_layer_frame = layer_get_frame(window_layer);			
	controller->update_layer = layer_create(GRect(window_layer_frame.origin.x + window_layer_frame.size.w - 7, 
																		window_layer_frame.origin.y + 1, 
																		5, 5));																		
	layer_set_update_proc(controller->update_layer, controller_update_layer_update_proc);	
  layer_add_child(window_layer, controller->update_layer);		
}

void controller_redraw_update_layer(Controller* controller, GColor color)
{
	controller_update_color = color;
	if(controller->update_layer)
		layer_mark_dirty(controller->update_layer);			
}
