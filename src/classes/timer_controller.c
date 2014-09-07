#include "timer_controller.h"
#include "list.h"

TimerController* timer_controller_from_controller(Controller *controller)
{
	return container_of(controller, TimerController, base);
}

void timer_controller_load(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerController %s", __func__);
	
	TimerController *timer_controller = timer_controller_from_controller(controller);

	Window *window = controller_get_window(controller);
	Layer *window_root_layer = window_get_root_layer(window);		

	timer_controller->timer_view = timer_view_create(layer_get_bounds(window_root_layer));
	
	view_load(timer_view_get_view(timer_controller->timer_view));

	layer_add_child(window_root_layer, view_get_root_layer(timer_view_get_view(timer_controller->timer_view)));
	
	controller_load_update_layer(controller);	
}

void timer_controller_unload(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerController %s", __func__);
		
	TimerController *timer_controller = timer_controller_from_controller(controller);	
	
	view_unload(timer_view_get_view(timer_controller->timer_view));
}

void timer_controller_destroy(Controller *controller)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerController %s", __func__);
		
	TimerController *timer_controller = timer_controller_from_controller(controller);
	free(timer_controller);
}

TimerController* timer_controller_create(Window *window, ControllerHandlers handlers)
{
	TimerController *timer_controller = malloc(sizeof(TimerController));
	memset(timer_controller, 0, sizeof(*timer_controller));
	__controller_init(&timer_controller->base, window, handlers, (ControllerVTable) {
		.load = timer_controller_load,
		.unload = timer_controller_unload,		
		.destroy = timer_controller_destroy		
	});
	return timer_controller;
}

Controller* timer_controller_get_controller(TimerController *timer_controller)
{
	return &timer_controller->base;
}