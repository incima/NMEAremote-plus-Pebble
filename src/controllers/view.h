#ifndef VIEW_H_
#define VIEW_H_

#include <pebble.h>
#include "list.h"

struct View;

typedef struct ViewVTable {
	void (*load)(struct View*);
	void (*unload)(struct View*);	
} ViewVTable;

typedef struct View {
	ViewVTable vtable;
	Layer *root_layer;
	bool loaded;
} View;

typedef struct {
	struct list_head list;
	View *view;
} ViewListEntry;

void __view_init(View* view, ViewVTable vtable);
void view_load(View* view);
void view_unload(View* view);
Layer* view_get_root_layer(View *);

ViewListEntry* view_list_entry_create(View *view);
	
#endif /* VIEW_H_ */
