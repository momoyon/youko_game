#include <entity.h>
#include <config.h>

size_t entity_id_counter = 0;
Entity_ids free_entity_ids = {0};

size_t get_unique_id(void) {
    if (free_entity_ids.count > 0) {
        int last_free_id = -1;
        darr_remove(free_entity_ids, int, &last_free_id, (int)free_entity_ids.count-1);
        ASSERT(last_free_id != -1, "We failed to get last free id!");
        return last_free_id;
    } else {
        return entity_id_counter++;
    }
}

Entity make_entity(Entities *entities, Vector2 pos, float radius, Entity_kind kind, Arena *arena, Arena *temp_arena) {
	Entity e = {
		.pos = pos,
		.radius = radius <= 0 ? ENTITY_DEFAULT_RADIUS : radius,
		.kind = kind,
		.id = get_unique_id(),
		.state = 0,
		.arena = arena,
		.temp_arena = temp_arena,
		.entities = entities,
	};

	switch (e.kind) {
		case EK_NONE: {

		} break;
		case EK_COUNT:
		default: ASSERT(false, "UNREACHABLE!");
	}

	return e;
}
