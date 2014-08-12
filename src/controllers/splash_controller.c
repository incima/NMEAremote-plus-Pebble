#include "common.h"
#include "splash_controller.h"

SplashController* controller_get_splash_controller(Controller* controller) 
{
	void *controller_pptr = (void*)(controller);		
	return (SplashController*)controller_pptr;	
}

void splash_controller_load(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_load");		
}

void splash_controller_unload(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_unload");		
}

void splash_controller_redraw(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_redraw");		
}

void splash_controller_cancel_redraw(Controller* controller) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "SplashController splash_controller_cancel_redraw");			
}

SplashController* splash_controller_create(Window* window, ControllerHandlers handlers)
{
	SplashController* splash_controller = malloc(sizeof(SplashController));
	memset(splash_controller, 0, sizeof(SplashController));
	__controller_init(&splash_controller->controller, window, handlers, (ControllerVTable) {
		.load = splash_controller_load,
		.unload = splash_controller_unload,
		.redraw = splash_controller_redraw,
		.cancel_redraw = splash_controller_cancel_redraw
	});
	return splash_controller;
}

Controller* splash_controller_get_controller(SplashController* splash_controller)
{
	return &splash_controller->controller;
}