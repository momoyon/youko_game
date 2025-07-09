#include <screen.h>
#include <config.h>

int screen_id_counter = 0;

Screen make_screen(Arena *arena, Arena *temp_arena) {
	Screen s = {
		.id = screen_id_counter++,
		.arena = arena,
		.temp_arena = temp_arena,
	};

	s.cols = WIDTH / TILE_SIZE;
	s.rows = HEIGHT / TILE_SIZE;

	arr_heap_init(s.tiles, sizeof(Tile)*s.cols*s.rows);

	return s;
}

void set_tile_at(Screen *s, Vector2 cursor, Tile t) {
	int x = cursor.x / TILE_SIZE;
	int y = cursor.y / TILE_SIZE;

	int idx = y * s->cols + x;
	if (idx < 0 || idx > s->tiles.count-1) {
		log_error("Index is outofbounds! %d !! 0 ~ %zu", idx, s->tiles.count-1);
		exit(1);
	}

	s->tiles.items[idx] = t;
}

void draw_screen(Screen *s) {
	draw_tiles(&s->tiles);
	if (DEBUG_DRAW) {
		draw_text_aligned(GetFontDefault(), arena_alloc_str(*s->temp_arena, "Screen %zu", s->id), v2(WIDTH*0.5, 0), ENTITY_DEFAULT_RADIUS, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_CENTER, WHITE);
	}
}

void free_screen(Screen *s) {
	// TODO: We need to free on tiles if they have allocated items
	arr_free(s->tiles);
}
