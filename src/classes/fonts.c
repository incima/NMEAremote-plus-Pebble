#include "fonts.h"

static Fonts *_fonts;

void fonts_load()
{
	if (_fonts)
		return;	
	_fonts = malloc(sizeof(Fonts));
	memset(_fonts, 0, sizeof(*_fonts));
	_fonts->light_24_text = fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_LIGHT_24_TEXT));
	_fonts->extra_48_numbers = fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_48_NUMBERS));
	_fonts->bold_24_numbers = fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_BOLD_24_NUMBERS));
	_fonts->extra_24_text = fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_EXTRA_24_TEXT));
	_fonts->regular_12_text = fonts_load_custom_font(resource_get_handle(FONT_OPENSANS_12_TEXT));
}

void fonts_unload()
{
	if (_fonts == NULL)
		return;	
	fonts_unload_custom_font(_fonts->extra_48_numbers);
	fonts_unload_custom_font(_fonts->light_24_text);
	fonts_unload_custom_font(_fonts->bold_24_numbers);
	fonts_unload_custom_font(_fonts->extra_24_text);
	fonts_unload_custom_font(_fonts->regular_12_text);				
	free(_fonts);
	_fonts = NULL;
}

GFont* fonts_get(FontType type)
{
	switch (type) {
		case EXTRA_48_NUMBERS: 	return _fonts->extra_48_numbers;
		case LIGHT_24_TEXT: 		return _fonts->light_24_text;
		case BOLD_24_NUMBERS:		return _fonts->bold_24_numbers;
		case EXTRA_24_TEXT:			return _fonts->extra_24_text;
		case REGULAR_12_TEXT:		return _fonts->regular_12_text;
	}
	return NULL;	
}