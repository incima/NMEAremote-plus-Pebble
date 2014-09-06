#include "trl_switch_view.h"

TRLSwitchView* trl_switch_view_from_view(View *view)
{
	return container_of(view, TRLSwitchView, base);
}

void trl_switch_view_load(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_load");		
		
	TRLSwitchView *switch_view = trl_switch_view_from_view(view);
	
	switch_view->base.root_layer = layer_create(switch_view->base.frame);
}

void trl_switch_view_unload(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_unload");		
		
	TRLSwitchView *switch_view = trl_switch_view_from_view(view);
	
	// destroy top views
	struct list_head *ptr, *tmp;	
	ViewListEntry *entry;
  list_for_each_safe(ptr, tmp, &switch_view->view_list) {
		entry = list_entry(ptr, ViewListEntry, list);
		list_del(ptr);	 
		view_destroy(entry->view);		 
		free(entry);
	}		
	switch_view->curr_entry = NULL;
	
	layer_destroy(switch_view->base.root_layer), switch_view->base.root_layer = NULL;					
}

void trl_switch_view_destroy(View *view)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_destroy");		
	TRLSwitchView *switch_view = trl_switch_view_from_view(view);				
	free(switch_view);
}

TRLSwitchView* trl_switch_view_create(Window *window, GRect frame)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_create");		
		
	TRLSwitchView *switch_view = malloc(sizeof(TRLSwitchView));
	memset(switch_view, 0, sizeof(*switch_view));
	INIT_LIST_HEAD(&switch_view->view_list);	
	__view_init(&switch_view->base, (ViewVTable) {
		.load = trl_switch_view_load,
		.unload = trl_switch_view_unload,
		.destroy = trl_switch_view_destroy
	});
	switch_view->window = window;
	switch_view->base.frame = frame;
	return switch_view;
}

void trl_switch_view_add_view(TRLSwitchView *switch_view, View *view)
{		
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_add_view");		
				
	ViewListEntry *entry = view_list_entry_create(view);
	entry->view->parent_view = &switch_view->base; 
	list_add_tail(&entry->list, &switch_view->view_list);
}

static void destroy_property_animation(PropertyAnimation **prop_animation) {
  if (prop_animation == NULL || *prop_animation == NULL) {
    return;
  }
  if (animation_is_scheduled((Animation*)*prop_animation)) {
    animation_unschedule((Animation*)*prop_animation);
  }
  property_animation_destroy(*prop_animation);
  *prop_animation = NULL;
}

void trl_switch_view_curr_animation_started(Animation *animation, ViewListEntry *prev_entry) {
	TRLSwitchView *switch_view = trl_switch_view_from_view(prev_entry->view->parent_view);
	Layer *window_root_layer = window_get_root_layer(switch_view->window);
	layer_add_child(window_root_layer, view_get_root_layer(prev_entry->view));
}

void trl_switch_view_curr_animation_stopped(Animation *animation, bool finished, ViewListEntry *prev_entry) {
	layer_remove_from_parent(view_get_root_layer(prev_entry->view));
	view_unload(prev_entry->view);
}
	
void trl_switch_view_next_animation_started(Animation *animation, ViewListEntry *next_entry) {
	TRLSwitchView *switch_view = trl_switch_view_from_view(next_entry->view->parent_view);
	Layer *window_root_layer = window_get_root_layer(switch_view->window);
	layer_add_child(window_root_layer, view_get_root_layer(next_entry->view));
}

void trl_switch_view_next_animation_stopped(Animation *animation, bool finished, ViewListEntry *next_entry) {
	TRLSwitchView *switch_view = trl_switch_view_from_view(next_entry->view->parent_view);	
	switch_view->curr_entry = next_entry;
}

void trl_switch_view_next(TRLSwitchView *switch_view, bool animate)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_next");			
	
	destroy_property_animation(&switch_view->curr_prop_animation);	
	destroy_property_animation(&switch_view->next_prop_animation);	
		
	ViewListEntry *curr_entry = switch_view->curr_entry;
	ViewListEntry *next_entry = NULL;	
	
	if (curr_entry) {
		Layer *curr_top_layer = view_get_root_layer(curr_entry->view);
	
		GRect curr_from_frame = layer_get_frame(curr_top_layer);
		curr_from_frame.origin.x = 0;
		GRect curr_to_frame = curr_from_frame;
		curr_to_frame.origin.x = -144;
		switch_view->curr_prop_animation = property_animation_create_layer_frame(curr_top_layer, &curr_from_frame, &curr_to_frame);
		animation_set_duration((Animation*)switch_view->curr_prop_animation, 400);
		animation_set_curve((Animation*)switch_view->curr_prop_animation, AnimationCurveEaseIn);
		animation_set_handlers((Animation*)switch_view->curr_prop_animation, (AnimationHandlers) {
	    .started = (AnimationStartedHandler)trl_switch_view_curr_animation_started,		
	    .stopped = (AnimationStoppedHandler)trl_switch_view_curr_animation_stopped,
	  }, curr_entry);
			
		if (curr_entry->list.next != &switch_view->view_list)
			next_entry = list_entry(curr_entry->list.next, ViewListEntry, list);
		else
			next_entry = list_entry(switch_view->view_list.next, ViewListEntry, list);	
	}
	else if (switch_view->view_list.next) {
		next_entry = list_entry(switch_view->view_list.next, ViewListEntry, list);
	}
	
	if (next_entry) {
		Layer *next_top_layer = view_get_root_layer(next_entry->view);
	
		GRect next_from_frame = layer_get_frame(next_top_layer);
		next_from_frame.origin.x = 144;
		GRect next_to_frame = next_from_frame; 
		next_to_frame.origin.x = 0; 	
	
		switch_view->next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
		animation_set_duration((Animation*)switch_view->next_prop_animation, 400);
		animation_set_curve((Animation*)switch_view->next_prop_animation, AnimationCurveEaseIn);	  			
	  animation_set_handlers((Animation*)switch_view->next_prop_animation, (AnimationHandlers) {
	    .started = (AnimationStartedHandler)trl_switch_view_next_animation_started,
	    .stopped = (AnimationStoppedHandler)trl_switch_view_next_animation_stopped,
	  }, next_entry);
	}
	
	if (switch_view->curr_prop_animation)
		animation_schedule((Animation*)switch_view->curr_prop_animation);	
	if (switch_view->next_prop_animation)
		animation_schedule((Animation*)switch_view->next_prop_animation);			
}

void trl_switch_view_prev(TRLSwitchView *switch_view, bool animate)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TRLSwitchView trl_switch_view_prev");			
		
	destroy_property_animation(&switch_view->curr_prop_animation);	
	destroy_property_animation(&switch_view->next_prop_animation);	
		
	ViewListEntry *curr_entry = switch_view->curr_entry;
	ViewListEntry *next_entry = NULL;

	if (curr_entry) {
		Layer *curr_top_layer = view_get_root_layer(curr_entry->view);
		
		GRect curr_from_frame = layer_get_frame(curr_top_layer);
		curr_from_frame.origin.x = 0;	
		GRect curr_to_frame = curr_from_frame;
		curr_to_frame.origin.x = 144;
		switch_view->curr_prop_animation = property_animation_create_layer_frame(curr_top_layer, &curr_from_frame, &curr_to_frame);
		animation_set_duration((Animation*)switch_view->curr_prop_animation, 400);
		animation_set_curve((Animation*)switch_view->curr_prop_animation, AnimationCurveEaseIn);	
		animation_set_handlers((Animation*)switch_view->curr_prop_animation, (AnimationHandlers) {
	    .started = (AnimationStartedHandler)trl_switch_view_curr_animation_started,		
	    .stopped = (AnimationStoppedHandler)trl_switch_view_curr_animation_stopped,
	  }, curr_entry);
						
		if (curr_entry->list.prev != &switch_view->view_list)
			next_entry = list_entry(curr_entry->list.prev, ViewListEntry, list);
		else
			next_entry = list_entry(switch_view->view_list.prev, ViewListEntry, list);
	}		
	else if (switch_view->view_list.prev) {
		next_entry = list_entry(switch_view->view_list.prev, ViewListEntry, list);
	}
			
	if (next_entry) {
		Layer *next_top_layer = view_get_root_layer(next_entry->view);

		GRect next_from_frame = layer_get_frame(next_top_layer);
		next_from_frame.origin.x = -144; 		
		GRect next_to_frame = next_from_frame; 
		next_to_frame.origin.x = 0; 	

		switch_view->next_prop_animation = property_animation_create_layer_frame(next_top_layer, &next_from_frame, &next_to_frame);
		animation_set_duration((Animation*)switch_view->next_prop_animation, 400);
		animation_set_curve((Animation*)switch_view->next_prop_animation, AnimationCurveEaseIn);	  	
		animation_set_handlers((Animation*)switch_view->next_prop_animation, (AnimationHandlers) {
	    .started = (AnimationStartedHandler)trl_switch_view_next_animation_started,
	    .stopped = (AnimationStoppedHandler)trl_switch_view_next_animation_stopped,
	  }, next_entry);					
	}

	if (switch_view->curr_prop_animation)
		animation_schedule((Animation*)switch_view->curr_prop_animation);	
	if (switch_view->next_prop_animation)
		animation_schedule((Animation*)switch_view->next_prop_animation);		
}

