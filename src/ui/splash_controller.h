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

#include "ui/controller.h"

typedef struct {
	Controller controller;
	Layer *circle_layer1;
	Layer *circle_layer2;
	Layer *circle_layer3;
	int circle_counter;			
	bool updating;
	BitmapLayer *splash_layer;
	GBitmap *splash_bitmap;
	TextLayer *info_layer;
	char *info_text;
} SplashController;

SplashController* splash_controller_create(Window* window, ControllerHandlers handlers);
Controller* splash_controller_get_controller(SplashController* splash_controller);
void splash_controller_set_bitmap_from_resource(SplashController* splash_controller, uint32_t resource_id);
void splash_controller_set_info_text(SplashController* splash_controller, char *info_text);
void splash_controller_set_updating(SplashController* splash_controller, bool updating);
