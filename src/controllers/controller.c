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
	if (controller->handlers.on_did_load)
		controller->handlers.on_did_load(controller);
}

void controller_unload(Controller* controller) 
{
	ControllerVTable *vtable = controller_get_vtable(controller);	
	if (vtable->unload)
		vtable->unload(controller);
	if (controller->handlers.on_did_unload)
		controller->handlers.on_did_unload(controller);	
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