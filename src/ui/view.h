#pragma once

#include <pebble.h>

#include "common/list.h"

struct View;

typedef struct ViewVTable {
	void (*load)(struct View*);
	void (*unload)(struct View*);	
	void (*destroy)(struct View*);		
} ViewVTable;

typedef struct View {
	struct View *parent_view;
	//struct list_head child_view_list;
	ViewVTable vtable;
	Layer *root_layer;
	GRect frame;
	bool loaded;
} View;

typedef struct {
	struct list_head list;
	View *view;
} ViewListEntry;

void __view_init(View* view, ViewVTable vtable);
void view_load(View* view);
void view_unload(View* view);
void view_destroy(View* view);
Layer* view_get_root_layer(View *);

ViewListEntry* view_list_entry_create(View *view);
	