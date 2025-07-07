#ifndef _CONTROL_CONFIG_H_
#define _CONTROL_CONFIG_H_

typedef struct Control_config Control_config;

struct Control_config {
	int move_left, move_right, move_up, move_down;
	int run;
};

#endif // _CONTROL_CONFIG_H_
