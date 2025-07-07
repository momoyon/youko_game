#include <state.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

const char *state_as_str(const State s) {
	switch (s) {
		case STATE_NORMAL: return "Normal";
		case STATE_TILE_EDIT: return "Tile Edit";
		case STATE_COUNT:
		default: ASSERT(false, "UNREACHABLE!");
	}
	return "YOU SHOULDN'T SEE THIS";
}
