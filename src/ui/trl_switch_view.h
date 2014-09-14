#pragma once

#include <pebble.h>

#include "common/list.h"
#include "ui/view.h"

typedef struct {
	View base;
	struct list_head view_list;	
	ViewListEntry *curr_entry;	
	Window *window;
	PropertyAnimation *curr_prop_animation;
	PropertyAnimation *next_prop_animation;	
} TRLSwitchView;

TRLSwitchView* trl_switch_view_create(Window *, GRect);
void trl_switch_view_add_view(TRLSwitchView *, View *view);

void trl_switch_view_next(TRLSwitchView *switch_view, bool animate);
void trl_switch_view_prev(TRLSwitchView *switch_view, bool animate);
