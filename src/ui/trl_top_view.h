#pragma once

#include <pebble.h>

#include "ui/view.h"

typedef struct {
	View base;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;
	char *title, *value;
} TRLTopView;

TRLTopView* trl_top_view_create(char* title, char *value);
TRLTopView* trl_top_view_from_view(View *view);
