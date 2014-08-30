#ifndef TRL_CONTROLLER_H_
#define TRL_CONTROLLER_H_

#include <controllers/controller.h>

typedef struct {
	Controller controller;
	Layer *top_layer;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;

	Layer *bottom_layer;
	Layer *right_bg_layer;
	TextLayer *right_value_layer;
	TextLayer *right_title_layer;

	Layer *left_bg_layer;
	TextLayer *left_value_layer;
	TextLayer *left_title_layer;

	Layer *circle_layer;
	
} TRLController;


TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);
TRLController* controller_get_trl_controller(Controller *);

#endif /* TRL_CONTROLLER_H_ */
