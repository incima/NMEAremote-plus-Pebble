#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <pebble.h>

struct Controller;

typedef struct 
{
	void (*did_load)(struct Controller*);
	void (*did_unload)(struct Controller*);	
	void (*did_finish)(struct Controller*);		
} ControllerHandlers;

typedef struct Controller 
{
	void* vtable;
	ControllerHandlers handlers;	
	Window* window;
	Layer* update_layer;
	GColor* update_color;
} Controller;

typedef struct {
	void (*load)(Controller*);
	void (*unload)(Controller*);	
	void (*redraw)(Controller*);
	void (*destroy)(Controller*);			
} ControllerVTable;

void __controller_init(Controller* controller, Window* window, ControllerHandlers handlers, ControllerVTable vtable);
void controller_load(Controller*);
void controller_unload(Controller*);
void controller_redraw(Controller*);
void controller_load_update_layer(Controller* controller);
void controller_redraw_update_layer(Controller* controller, GColor color);
void controller_destroy(Controller *);

#endif /* CONTROLLER_H_ */
