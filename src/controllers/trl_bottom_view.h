#ifndef TRL_BOTTOM_VIEW_H_
#define TRL_BOTTOM_VIEW_H_

#include <pebble.h>
#include "controllers/view.h"

typedef struct {
	View base;
	Layer *left_bg_layer;
	TextLayer *left_value_layer;
	TextLayer *left_title_layer;
	Layer *right_bg_layer;
	TextLayer *right_value_layer;
	TextLayer *right_title_layer;	
	char *left_title, *left_value;
	char *right_value, *right_title;
} TRLBottomView;


TRLBottomView* trl_bottom_view_create(char* left_title, char *left_value, char *right_title, char *right_value);
void trl_bottom_view_destroy(TRLBottomView *bottom_view);
TRLBottomView* trl_bottom_view_from_view(View *view);

#endif /* TRL_BOTTOM_VIEW_H_ */
