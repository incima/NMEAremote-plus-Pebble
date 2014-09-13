#include "timer_view.h"
#include "list.h"
#include "fonts.h"

TimerView * timer_view_from_view(View *view)
{
	return container_of(view, TimerView, base);
}

void timer_view_load(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);

	TimerView *timer_view = timer_view_from_view(view);	

	timer_view->base.root_layer = layer_create(timer_view->frame);

	int w = timer_view->frame.size.w;
	int h = timer_view->frame.size.h;	
	int y = (h / 2) - (54+40) / 2;

	timer_view->value_layer = text_layer_create(GRect(0, y, w, 54));
	text_layer_set_text_color(timer_view->value_layer, GColorWhite);
	text_layer_set_background_color(timer_view->value_layer, GColorClear);
	text_layer_set_font(timer_view->value_layer, fonts_get(EXTRA_48_NUMBERS));
	text_layer_set_text_alignment(timer_view->value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(timer_view->value_layer, GTextOverflowModeWordWrap);
	text_layer_set_text(timer_view->value_layer, values.starttime_str);

	timer_view->title_layer = text_layer_create(GRect(0, y+54, w, 40));
	text_layer_set_text_color(timer_view->title_layer, GColorWhite);
	text_layer_set_background_color(timer_view->title_layer, GColorClear);
	text_layer_set_font(timer_view->title_layer, fonts_get(LIGHT_24_TEXT));
	text_layer_set_text_alignment(timer_view->title_layer, GTextAlignmentCenter);
	text_layer_set_text(timer_view->title_layer, "TO GUN");

	layer_add_child(timer_view->base.root_layer, text_layer_get_layer(timer_view->value_layer));
	layer_add_child(timer_view->base.root_layer, text_layer_get_layer(timer_view->title_layer));		
}

void timer_view_unload(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);

	TimerView *timer_view = timer_view_from_view(view);
	text_layer_destroy(timer_view->value_layer), timer_view->value_layer = NULL;
	text_layer_destroy(timer_view->title_layer), timer_view->title_layer = NULL;					
	layer_destroy(timer_view->base.root_layer);
}

void timer_view_destroy(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TimerView %s", __func__);		
	TimerView *timer_view = timer_view_from_view(view);
	free(timer_view);
}

TimerView* timer_view_create(GRect frame)
{
	TimerView *timer_view = malloc(sizeof(TimerView));
	memset(timer_view, 0, sizeof(*timer_view));
	__view_init(&timer_view->base, (ViewVTable) {
		.load = timer_view_load,
		.unload = timer_view_unload,
		.destroy = timer_view_destroy
	});
	timer_view->frame = frame;
	return timer_view;
}

View* timer_view_get_view(TimerView *timer_view)
{
	return &timer_view->base;
}
