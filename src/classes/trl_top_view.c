#include "trl_top_view.h"
#include "common.h"
#include "fonts.h"
#include "list.h"

#undef APP_LOG
#define APP_LOG(...)

TRLTopView* trl_top_view_from_view(View *view)
{
	return container_of(view, TRLTopView, base);
}

void trl_top_view_load(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLTopView trl_top_view_load");		

	TRLTopView *top_view = trl_top_view_from_view(view);

	// Top layer	
	top_view->base.root_layer = layer_create(GRect(0, 0, 144, 104));	

	top_view->top_value_layer = text_layer_create(GRect(0, 10, 144, 54));
	text_layer_set_text_color(top_view->top_value_layer, GColorWhite);
	text_layer_set_background_color(top_view->top_value_layer, GColorClear);
	text_layer_set_font(top_view->top_value_layer, fonts_get(EXTRA_48_NUMBERS));
	text_layer_set_text_alignment(top_view->top_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(top_view->top_value_layer, GTextOverflowModeWordWrap);
	text_layer_set_text(top_view->top_value_layer, top_view->value);

	top_view->top_title_layer = text_layer_create(GRect(0, 64, 144, 40));
	text_layer_set_text_color(top_view->top_title_layer, GColorWhite);
	text_layer_set_background_color(top_view->top_title_layer, GColorClear);
	text_layer_set_font(top_view->top_title_layer, fonts_get(LIGHT_24_TEXT));
	text_layer_set_text_alignment(top_view->top_title_layer, GTextAlignmentCenter);
	text_layer_set_text(top_view->top_title_layer, top_view->title);

	layer_add_child(top_view->base.root_layer, text_layer_get_layer(top_view->top_value_layer));
	layer_add_child(top_view->base.root_layer, text_layer_get_layer(top_view->top_title_layer));			
}

void trl_top_view_unload(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLTopView trl_top_view_unload");	

	TRLTopView *top_view = trl_top_view_from_view(view);	

	text_layer_destroy(top_view->top_value_layer), top_view->top_value_layer = NULL;
	text_layer_destroy(top_view->top_title_layer), top_view->top_title_layer = NULL;		
	layer_destroy(top_view->base.root_layer), top_view->base.root_layer = NULL;				
}

void trl_top_view_destroy(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLTopView trl_top_view_destroy");		
	TRLTopView *top_view = trl_top_view_from_view(view);				
	free(top_view);
}

TRLTopView* trl_top_view_create(char* title, char *value)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLTopView trl_top_view_create");		

	TRLTopView *top_view = malloc(sizeof(TRLTopView));
	memset(top_view, 0, sizeof(TRLTopView));
	__view_init(&top_view->base, (ViewVTable) {
		.load = trl_top_view_load,
		.unload = trl_top_view_unload,
		.destroy = trl_top_view_destroy
	});
	top_view->title = title;
	top_view->value = value;
	return top_view;
}



