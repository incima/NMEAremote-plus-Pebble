#include "timer_view.h"
#include "list.h"

TimerView * timer_view_from_view(View *view)
{
	return container_of(view, TimerView, base);
}

void timer_view_load(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);
		
	TimerView *timer_view = timer_view_from_view(view);	
	
	timer_view->base.root_layer = layer_create(GRect(0, 0, 144, 164));
}

void timer_view_unload(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);
		
	TimerView *timer_view = timer_view_from_view(view);
	
	layer_destroy(timer_view->base.root_layer);
}

void timer_view_destroy(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);
		
	TimerView *timer_view = timer_view_from_view(view);
	free(timer_view);
}

TimerView* timer_view_create()
{
	TimerView *timer_view = malloc(sizeof(TimerView));
	memset(timer_view, 0, sizeof(*timer_view));
	__view_init(&timer_view->base, (ViewVTable) {
		.load = timer_view_load,
		.unload = timer_view_unload,
		.destroy = timer_view_destroy
	});
	return timer_view;
}

View* timer_view_get_view(TimerView *timer_view)
{
	return &timer_view->base;
}
