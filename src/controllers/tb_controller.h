#ifndef TB_CONTROLLER_H_
#define TB_CONTROLLER_H_

#include <controller/controller.h>

struct TBController {
	Controller controller;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;
	TextLayer *bottom_value_layer;
	TextLayer *bottom_title_layer;
	
	char *top_value;
	char *top_title;
	char *bottom_value;
	char *bottom_title;
}


#endif /* TB_CONTROLLER_H_ */
