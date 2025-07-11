#include <common.h>
#include <config.h>

void init(Texture2D tile_sheet) {
	cam.offset = CLITERAL(Vector2) {
		.x = (WIDTH*0.5f),
		.y = (HEIGHT*0.5f),
	};
	cam.target = CLITERAL(Vector2) {
		.x = (WIDTH*0.5f),
		.y = (HEIGHT*0.5f),
	};

	cam.zoom = CAMERA_DEFAULT_ZOOM;

	arena = arena_make(32*1024);
	temp_arena = arena_make(1*1024);

	Screen scr = make_screen(&arena, &temp_arena, tile_sheet);
	darr_append(screens, scr);
}

size_t add_entity(Vector2 pos, Entity_kind kind, Arena *arena, Arena *temp_arena) {
	Entity e = make_entity(&entities, pos, ENTITY_DEFAULT_RADIUS, kind, arena, temp_arena);
	darr_append(entities, e);
	return e.id;
}
