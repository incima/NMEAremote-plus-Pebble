#include "trl_bottom_view.h"
#include "common.h"
#include "list.h"

static void rounded_layer_update_proc(Layer* layer, GContext *context) 
{	
	GRect layer_bounds = layer_get_bounds(layer);		
	graphics_context_set_fill_color(context, GColorWhite);	
	graphics_fill_rect(context, layer_bounds, 4, GCornersAll);
}

TRLBottomView* trl_bottom_view_from_view(View *view)
{
	return container_of(view, TRLBottomView, base);
}

void trl_bottom_view_load(View *view)
{
	TRLBottomView *bottom_view = trl_bottom_view_from_view(view);
			
	// bottom layer
	bottom_view->base.root_layer = layer_create(GRect(0, 104, 144, 64));
	
	// Left bottom layer
	bottom_view->left_bg_layer = layer_create(GRect(0, 0, 70, 64));
	layer_set_update_proc(bottom_view->left_bg_layer, rounded_layer_update_proc);	

  bottom_view->left_value_layer = text_layer_create(GRect(0, 0, 70, 24));
  text_layer_set_background_color(bottom_view->left_value_layer, GColorClear);
  text_layer_set_text_color(bottom_view->left_value_layer, GColorBlack);	
  text_layer_set_font(bottom_view->left_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(bottom_view->left_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->left_value_layer, GTextOverflowModeWordWrap);	
	text_layer_set_text(bottom_view->left_value_layer, bottom_view->left_value);
  
	bottom_view->left_title_layer = text_layer_create(GRect(0, 27, 70, 37));
  text_layer_set_background_color(bottom_view->left_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->left_title_layer, GColorBlack);
  text_layer_set_font(bottom_view->left_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(bottom_view->left_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->left_title_layer, bottom_view->left_title);
	
	// Right bottom layer
	bottom_view->right_bg_layer = layer_create(GRect(74, 0, 70, 64));
	layer_set_update_proc(bottom_view->right_bg_layer, rounded_layer_update_proc);

  bottom_view->right_value_layer = text_layer_create(GRect(74, 0, 70, 24));
  text_layer_set_background_color(bottom_view->right_value_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_value_layer, GColorBlack);
  text_layer_set_font(bottom_view->right_value_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS)));
  text_layer_set_text_alignment(bottom_view->right_value_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(bottom_view->right_value_layer, GTextOverflowModeWordWrap);		
	text_layer_set_text(bottom_view->right_value_layer, bottom_view->right_value);
				
  bottom_view->right_title_layer = text_layer_create(GRect(74, 27, 70, 37));
  text_layer_set_background_color(bottom_view->right_title_layer, GColorClear);
	text_layer_set_text_color(bottom_view->right_title_layer, GColorBlack);
  text_layer_set_font(bottom_view->right_title_layer, fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT)));
  text_layer_set_text_alignment(bottom_view->right_title_layer, GTextAlignmentCenter);
	text_layer_set_text(bottom_view->right_title_layer, bottom_view->right_title);

	// add to root layer
	layer_add_child(bottom_view->base.root_layer, bottom_view->left_bg_layer);	
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->left_value_layer));
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->left_title_layer));	
	layer_add_child(bottom_view->base.root_layer, bottom_view->right_bg_layer);	
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->right_value_layer));
	layer_add_child(bottom_view->base.root_layer, text_layer_get_layer(bottom_view->right_title_layer));		
}

void trl_bottom_view_unload(View *view)
{
	TRLBottomView *bottom_view = trl_bottom_view_from_view(view);
		
  text_layer_destroy(bottom_view->left_value_layer), bottom_view->left_value_layer = NULL;
  text_layer_destroy(bottom_view->left_title_layer), bottom_view->left_title_layer = NULL;
  text_layer_destroy(bottom_view->right_value_layer), bottom_view->right_value_layer = NULL;		
  text_layer_destroy(bottom_view->right_title_layer), bottom_view->right_title_layer = NULL;	
	layer_destroy(bottom_view->left_bg_layer), bottom_view->left_bg_layer = NULL;
	layer_destroy(bottom_view->right_bg_layer), bottom_view->right_bg_layer = NULL;	
  layer_destroy(bottom_view->base.root_layer), bottom_view->base.root_layer = NULL;	
}

TRLBottomView* trl_bottom_view_create(char* left_title, char *left_value, char *right_title, char *right_value)
{
	TRLBottomView *bottom_view = malloc(sizeof(TRLBottomView));
	memset(bottom_view, 0, sizeof(TRLBottomView));
	__view_init(&bottom_view->base, (ViewVTable) {
		.load = trl_bottom_view_load,
		.unload = trl_bottom_view_unload
	});
	bottom_view->left_title = left_title;
	bottom_view->left_value = left_value;
	bottom_view->right_title = right_title;
	bottom_view->right_value = right_value;	
	return bottom_view;	
}

void trl_bottom_view_destroy(TRLBottomView *bottom_view) 
{
	view_unload(&bottom_view->base);
	free(bottom_view);	
}

