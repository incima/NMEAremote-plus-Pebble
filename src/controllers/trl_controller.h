#ifndef TRL_CONTROLLER_H_
#define TRL_CONTROLLER_H_

#include <pebble.h>
#include <list.h>
#include <controllers/controller.h>
#include <controllers/trl_top_view.h>
#include <controllers/trl_bottom_view.h>


typedef struct {
	Controller controller;
	struct list_head top_view_list;	
	ViewListEntry *curr_top_view;
	struct list_head bottom_view_list;	
	ViewListEntry *curr_bottom_view;	
	Layer *circle_layer;	
} TRLController;

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);
TRLController* controller_get_trl_controller(Controller *);

#endif /* TRL_CONTROLLER_H_ */
