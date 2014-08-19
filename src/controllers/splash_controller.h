#ifndef SPLASH_CONTROLLER_H_
#define SPLASH_CONTROLLER_H_

#include <controllers/controller.h>

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
} SplashController;

SplashController* splash_controller_create(Window* window, ControllerHandlers handlers);
Controller* splash_controller_get_controller(SplashController* splash_controller);
void splash_controller_set_bitmap_from_resource(SplashController* splash_controller, uint32_t resource_id);
void splash_controller_set_updating(SplashController* splash_controller, bool updating);

#endif /* SPLASH_CONTROLLER_H_ */
