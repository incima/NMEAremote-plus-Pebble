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

struct Controller;

typedef struct {
	void (*did_load)(struct Controller*);
	void (*did_unload)(struct Controller*);	
} ControllerHandlers;

typedef struct {
	void (*load)(struct Controller*);
	void (*unload)(struct Controller*);	
	void (*redraw)(struct Controller*);
	void (*destroy)(struct Controller*);	
	bool (*on_button_up)(struct Controller*, ClickRecognizerRef);
	bool (*on_button_down)(struct Controller*, ClickRecognizerRef);	
} ControllerVTable;

typedef struct Controller  {
	ControllerVTable vtable;
	ControllerHandlers handlers;	
	Window* window;
	Layer* update_layer;
	GColor* update_color;
	bool loaded;
} Controller;


void __controller_init(Controller* controller, Window* window, ControllerHandlers handlers, ControllerVTable vtable);
void controller_load(Controller*);
void controller_unload(Controller*);
void controller_redraw(Controller*);
void controller_load_update_layer(Controller* controller);
void controller_redraw_update_layer(Controller* controller, GColor color);
void controller_destroy(Controller *);
bool controller_on_button_up(Controller *controller, ClickRecognizerRef);
bool controller_on_button_down(Controller *controller, ClickRecognizerRef);
Window* controller_get_window(Controller *);
