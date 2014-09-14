/**
 * NMEAremote (c) + Pebble
 * 
 * Copyright (c) 2014 Holger Franzen for incima
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
	