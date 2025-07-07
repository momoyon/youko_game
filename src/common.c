#include <common.h>
#include <config.h>

void add_entity(Vector2 pos, Entity_kind kind, Arena *arena, Arena *temp_arena) {
	Entity e = make_entity(&entities, pos, ENTITY_DEFAULT_RADIUS, kind, arena, temp_arena);
	darr_append(entities, e);
}
