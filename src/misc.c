#include <misc.h>
#include <config.h>

void draw_grid(void) {
	// DrawLineV(
}

Vector2 snap_to_tile(Vector2 v) {
	Vector2 res = {
		.x = (float)((int)v.x / (int)TILE_SIZE) * TILE_SIZE,
		.y = (float)((int)v.y / (int)TILE_SIZE) * TILE_SIZE,
	};

	return res;
}
