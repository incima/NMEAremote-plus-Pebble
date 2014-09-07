#ifndef TIMER_VIEW_H_
#define TIMER_VIEW_H_

#include "view.h"

typedef struct {
	View base;
} TimerView;

TimerView* timer_view_create();
View* timer_view_get_view(TimerView *timer_view);

#endif /* TIMER_VIEW_H_ */
