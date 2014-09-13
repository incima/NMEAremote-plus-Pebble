#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <pebble.h>

struct Controller;

typedef struct {
	void (*did_load)(struct Controller*);
	void (*did_unload)(struct Controller*);	
} ControllerHandlers;

typedef struct {
	void (*load)(struct Controller*);
	void (*unload)(struct Controller*);	
	void (*redraw)(struct Controller*);
	void (*destroy)(struct Controller*);	
	bool (*on_button_up)(struct Controller*, ClickRecognizerRef);
	bool (*on_button_down)(struct Controller*, ClickRecognizerRef);	
} ControllerVTable;

typedef struct Controller  {
	ControllerVTable vtable;
	ControllerHandlers handlers;	
	Window* window;
	Layer* update_layer;
	GColor* update_color;
	bool loaded;
} Controller;


void __controller_init(Controller* controller, Window* window, ControllerHandlers handlers, ControllerVTable vtable);
void controller_load(Controller*);
void controller_unload(Controller*);
void controller_redraw(Controller*);
void controller_load_update_layer(Controller* controller);
void controller_redraw_update_layer(Controller* controller, GColor color);
void controller_destroy(Controller *);
bool controller_on_button_up(Controller *controller, ClickRecognizerRef);
bool controller_on_button_down(Controller *controller, ClickRecognizerRef);

Window* controller_get_window(Controller *);

#endif /* CONTROLLER_H_ */
