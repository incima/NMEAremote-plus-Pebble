#pragma once

#include <pebble.h>

#include "common/list.h"
#include "ui/controller.h"
#include "ui/trl_top_view.h"
#include "ui/trl_bottom_view.h"
#include "ui/trl_switch_view.h"

typedef enum {
	TRLControlStateNone=0,
	TRLControlStateTop,
	TRLControlStateBottom	
} TRLControlState;

typedef struct {
	Controller controller;
	TRLControlState control_state;
	TRLSwitchView *top_switch_view;
	TRLSwitchView *bottom_switch_view;	
	Layer *circle_layer;	
} TRLController;

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);
TRLController* controller_get_trl_controller(Controller *);
