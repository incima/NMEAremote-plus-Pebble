#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <pebble.h>

struct Controller;

typedef struct 
{
	void (*on_did_load)(struct Controller*);
	void (*on_did_unload)(struct Controller*);	
} ControllerHandlers;

typedef struct Controller 
{
	void* vtable;
	ControllerHandlers handlers;	
	Window* window;
} Controller;

typedef struct {
	Controller controller;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;

	Layer *right_bg_layer;
	TextLayer *right_value_layer;
	TextLayer *right_title_layer;

	Layer *left_bg_layer;
	TextLayer *left_value_layer;
	TextLayer *left_title_layer;

	Layer *circle_layer;

	char *top_default_value;
	char *left_default_value;
	char *right_default_value;
	char *top_value;
	char *left_value;
	char *right_value;	
	char *top_title;
	char *left_title;
	char *right_title;
} TRLController;


TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);

void controller_load(Controller*);
void controller_unload(Controller*);
void controller_redraw_if_needed(Controller*);
void controller_cancel_redraw(Controller*);

#endif /* CONTROLLER_H_ */
