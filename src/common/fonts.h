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
#include "common.h"

typedef enum {
	EXTRA_48_NUMBERS,
	LIGHT_24_TEXT,
	BOLD_24_NUMBERS,
	EXTRA_24_TEXT,
	REGULAR_12_TEXT
} FontType;

typedef struct {
	GFont *extra_48_numbers;
	GFont *light_24_text;	
	GFont *bold_24_numbers;
	GFont *extra_24_text;
	GFont *regular_12_text;
} Fonts;
		
void fonts_load();
void fonts_unload();
GFont* fonts_get(FontType type);
