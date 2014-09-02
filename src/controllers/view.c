#include "view.h"
#include "common.h"

ViewVTable* view_get_vtable(View *view) {
	return &view->vtable;
}

void __view_init(View* view, ViewVTable vtable)
{
	memset(view, 0, sizeof(View));
	view->vtable = vtable;
}

Layer* view_get_root_layer(View *view) 
{
	if (!view->loaded)
		view_load(view);			
	return view->root_layer;
}

void view_load(View* view)
{
	if (view->loaded)
		return;
	ViewVTable *vtable = view_get_vtable(view);
	if (vtable->load)
		vtable->load(view);
	view->loaded = true;
}

void view_unload(View* view)
{
	if (!view->loaded)
		return;
	ViewVTable *vtable = view_get_vtable(view);
	if (vtable->unload)
		vtable->unload(view);	
	view->loaded = false;	
}

/**
 * ViewListEntry
 */
ViewListEntry* view_list_entry_create(View *view)
{
	ViewListEntry *entry = malloc(sizeof(ViewListEntry));
	memset(entry, 0, sizeof(ViewListEntry));
	entry->view = view;
	return entry;
}

