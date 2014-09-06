#ifndef FONTS_H_
#define FONTS_H_

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

#endif /* FONTS_H_ */
