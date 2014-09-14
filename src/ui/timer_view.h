#pragma once

#include <pebble.h>

#include "ui/view.h"

typedef struct {
	View base;
	GRect frame;
	TextLayer *value_layer;
	TextLayer *title_layer;
} TimerView;

TimerView* timer_view_create(GRect frame);
View* timer_view_get_view(TimerView *timer_view);
