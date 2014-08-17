#ifndef TRL_CONTROLLER_H_
#define TRL_CONTROLLER_H_

#include <controllers/controller.h>

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

	char *top_value;
	char *left_value;
	char *right_value;	
	char *top_title;
	char *left_title;
	char *right_title;
} TRLController;

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);

#endif /* TRL_CONTROLLER_H_ */
