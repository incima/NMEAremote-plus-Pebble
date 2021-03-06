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
#include "ui/view.h"

typedef struct {
	View base;
	struct list_head view_list;	
	ViewListEntry *curr_entry;	
	Window *window;
	PropertyAnimation *curr_prop_animation;
	PropertyAnimation *next_prop_animation;	
} TRLSwitchView;

TRLSwitchView* trl_switch_view_create(Window *, GRect);
void trl_switch_view_add_view(TRLSwitchView *, View *view);

void trl_switch_view_next(TRLSwitchView *switch_view, bool animate);
void trl_switch_view_prev(TRLSwitchView *switch_view, bool animate);
