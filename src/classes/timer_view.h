#ifndef TIMER_VIEW_H_
#define TIMER_VIEW_H_

#include <pebble.h>
#include "view.h"

typedef struct {
	View base;
	GRect frame;
	TextLayer *value_layer;
	TextLayer *title_layer;
} TimerView;

TimerView* timer_view_create(GRect frame);
View* timer_view_get_view(TimerView *timer_view);

#endif /* TIMER_VIEW_H_ */
