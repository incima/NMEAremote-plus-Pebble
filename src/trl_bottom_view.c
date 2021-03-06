#include "common/common.h"

#include "trl_bottom_view.h"

#undef APP_LOG
#define APP_LOG(...)

static void rounded_layer_update_proc(Layer* layer, GContext *context) 
{		
	GRect layer_bounds = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);	
	graphics_fill_rect(context, layer_bounds, 0, GCornerNone);
}

TRLBottomView* trl_bottom_view_from_view(View *view)
{
	return container_of(view, TRLBottomView, base);
}

void trl_bottom_view_load(View *view)
{			
	TRLBottomView *bottom_view = trl_bottom_view_from_view(view);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLBottomView %p trl_bottom_view_load", bottom_view);

	// bottom layer
	bottom_view->base.root_layer = layer_create(GRect(0, 104, 144, 64));

	// Left bottom layer
	bottom_view->left_bg_layer = layer_create(GRect(0, 0, 70, 64));

	bottom_view->left_value_layer = text_layer_create(GRect(0, 4, 70, 24));
	text_layer_set_background_color(bottom_view->left_value_layer, GColorClear);
	text_layer_set_text_color(bottom_view->left_value_layer, GColorBlack);	
	text_layer_set_font(bottom_view->left_value_layer, fonts_get(BOLD_24_NUMBERS));
	text_layer_set_text_alignment(bottom_view->left_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->left_value_layer, GTextOverflowModeWordWrap);	
	text_layer_set_text(bottom_view->left_value_layer, bottom_view->left_value);

	bottom_view->left_title_layer = text_layer_create(GRect(0, 30, 70, 36));
	text_layer_set_background_color(bottom_view->left_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->left_title_layer, GColorBlack);
	text_layer_set_font(bottom_view->left_title_layer, fonts_get(EXTRA_24_TEXT));
	text_layer_set_text_alignment(bottom_view->left_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->left_title_layer, bottom_view->left_title);

	// Right bottom layer
	bottom_view->right_bg_layer = layer_create(GRect(74, 0, 70, 64));

	bottom_view->right_value_layer = text_layer_create(GRect(74, 4, 70, 24));
	text_layer_set_background_color(bottom_view->right_value_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_value_layer, GColorBlack);
	text_layer_set_font(bottom_view->right_value_layer, fonts_get(BOLD_24_NUMBERS));
	text_layer_set_text_alignment(bottom_view->right_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->right_value_layer, GTextOverflowModeWordWrap);		
	text_layer_set_text(bottom_view->right_value_layer, bottom_view->right_value);

	bottom_view->right_title_layer = text_layer_create(GRect(74, 30, 70, 36));
	text_layer_set_background_color(bottom_view->right_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_title_layer, GColorBlack);
	text_layer_set_font(bottom_view->right_title_layer, fonts_get(EXTRA_24_TEXT));
	text_layer_set_text_alignment(bottom_view->right_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->right_title_layer, bottom_view->right_title);

	// add to root layer
	layer_add_child(bottom_view->base.root_layer, bottom_view->left_bg_layer);	
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->left_value_layer));
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->left_title_layer));	
	layer_add_child(bottom_view->base.root_layer, bottom_view->right_bg_layer);	
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->right_value_layer));
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->right_title_layer));	

	// background rect
	layer_set_update_proc(bottom_view->left_bg_layer, rounded_layer_update_proc);	
	layer_set_update_proc(bottom_view->right_bg_layer, rounded_layer_update_proc);		
}

void trl_bottom_view_unload(View *view)
{			
	TRLBottomView *bottom_view = trl_bottom_view_from_view(view);		

	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLBottomView %p trl_bottom_view_unload", bottom_view);	

	text_layer_destroy(bottom_view->left_value_layer), bottom_view->left_value_layer = NULL;
	text_layer_destroy(bottom_view->left_title_layer), bottom_view->left_title_layer = NULL;
	text_layer_destroy(bottom_view->right_value_layer), bottom_view->right_value_layer = NULL;		
	text_layer_destroy(bottom_view->right_title_layer), bottom_view->right_title_layer = NULL;	
	layer_destroy(bottom_view->left_bg_layer), bottom_view->left_bg_layer = NULL;
	layer_destroy(bottom_view->right_bg_layer), bottom_view->right_bg_layer = NULL;	
	layer_destroy(bottom_view->base.root_layer), bottom_view->base.root_layer = NULL;	
}

void trl_bottom_view_destroy(View *view) 
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLBottomView trl_bottom_view_destroy");	
	TRLBottomView *bottom_view = trl_bottom_view_from_view(view);		
	free(bottom_view);	
}

TRLBottomView* trl_bottom_view_create(char* left_title, char *left_value, char *right_title, char *right_value)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLBottomView trl_bottom_view_create");	

	TRLBottomView *bottom_view = malloc(sizeof(TRLBottomView));
	memset(bottom_view, 0, sizeof(TRLBottomView));
	__view_init(&bottom_view->base, (ViewVTable) {
		.load = trl_bottom_view_load,
		.unload = trl_bottom_view_unload,
		.destroy = trl_bottom_view_destroy
	});
	bottom_view->left_title = left_title;
	bottom_view->left_value = left_value;
	bottom_view->right_title = right_title;
	bottom_view->right_value = right_value;	
	return bottom_view;	
}


