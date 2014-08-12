#ifndef SPLASH_CONTROLLER_H_
#define SPLASH_CONTROLLER_H_

#include <controllers/controller.h>

typedef struct {
	Controller controller;
} SplashController;

SplashController* splash_controller_create(Window* window, ControllerHandlers handlers);
Controller* splash_controller_get_controller(SplashController* splash_controller);


#endif /* SPLASH_CONTROLLER_H_ */
