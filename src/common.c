#include <common.h>
#include <config.h>

void init(void) {
	cam.offset = CLITERAL(Vector2) {
		.x = -(SCREEN_WIDTH*SCREEN_SCALE*0.5f),
		.y = -(SCREEN_HEIGHT*SCREEN_SCALE*0.5f),
	};
	cam.zoom = 1.f;
}

void add_entity(Vector2 pos, Entity_kind kind, Arena *arena, Arena *temp_arena) {
	Entity e = make_entity(&entities, pos, ENTITY_DEFAULT_RADIUS, kind, arena, temp_arena);
	darr_append(entities, e);
}
