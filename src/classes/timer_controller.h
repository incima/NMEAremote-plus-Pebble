#ifndef TIMER_CONTROLLER_H_
#define TIMER_CONTROLLER_H_

#include "controller.h"
#include "timer_view.h"

typedef struct {
	Controller base;
	TimerView *timer_view;
} TimerController;

TimerController* timer_controller_create(Window *, ControllerHandlers);
Controller* timer_controller_get_controller(TimerController *timer_controller);

#endif /* TIMER_CONTROLLER_H_ */
