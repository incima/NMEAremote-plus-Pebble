#ifndef TRL_CONTROLLER_H_
#define TRL_CONTROLLER_H_

#include <list.h>
#include <controllers/controller.h>

typedef struct {
  struct list_head list;			
	Layer *root_layer;
	TextLayer *top_value_layer;
	TextLayer *top_title_layer;
} TRLTopView;

typedef struct {
  struct list_head list;			
	Layer *root_layer;
	Layer *left_bg_layer;
	TextLayer *left_value_layer;
	TextLayer *left_title_layer;
	Layer *right_bg_layer;
	TextLayer *right_value_layer;
	TextLayer *right_title_layer;	
} TRLBottomView;

typedef struct {
	Controller controller;
	struct list_head top_view_list;	
	TRLTopView *current_top_view;
	struct list_head bottom_view_list;	
	TRLBottomView *current_bottom_view;	
	Layer *circle_layer;	
} TRLController;

TRLController* trl_controller_create(Window* window, ControllerHandlers handlers);
Controller* trl_controller_get_controller(TRLController*);
TRLController* controller_get_trl_controller(Controller *);

#endif /* TRL_CONTROLLER_H_ */
