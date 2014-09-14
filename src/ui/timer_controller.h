#pragma once

#include "ui/controller.h"
#include "ui/timer_view.h"

typedef struct {
	Controller base;
	TimerView *timer_view;	
} TimerController;

TimerController* timer_controller_create(Window *, ControllerHandlers);
Controller* timer_controller_get_controller(TimerController *timer_controller);
