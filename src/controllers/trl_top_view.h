#ifndef TRL_TOP_VIEW_H_
#define TRL_TOP_VIEW_H_

#include <pebble.h>
#include <controllers/view.h>

typedef struct {
	View base;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;
	char *title, *value;
} TRLTopView;

TRLTopView* trl_top_view_create(char* title, char *value);
void trl_top_view_destroy(TRLTopView *top_view);
TRLTopView* trl_top_view_from_view(View *view);

#endif /* TRL_TOP_VIEW_H_ */
