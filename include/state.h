#ifndef _STATE_H_
#define _STATE_H_

typedef enum {
	STATE_NORMAL,
	STATE_TILE_EDIT,
	STATE_COUNT,
} State;

const char *state_as_str(const State s);
#endif // _STATE_H_
