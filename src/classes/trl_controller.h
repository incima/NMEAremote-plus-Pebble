#ifndef TRL_CONTROLLER_H_
#define TRL_CONTROLLER_H_

#include <pebble.h>
#include "list.h"
#include "controller.h"
#include "trl_top_view.h"
#include "trl_bottom_view.h"
#include "trl_switch_view.h"


typedef struct {
	Controller controller;
	TRLSwitchView *top_switch_view;
	TRLSwitchView *bottom_switch_view;	
	Layer *circle_layer;	
} TRLController;

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);
TRLController* controller_get_trl_controller(Controller *);

#endif /* TRL_CONTROLLER_H_ */
