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

	s.screen_str = arena_alloc_str(*temp_arena, "Screen %zu", s.id);

	return s;
}

void draw_screen(Screen *s) {
	draw_tiles(&s->tiles);
	if (DEBUG_DRAW) {
		draw_text_aligned(GetFontDefault(), s->screen_str, v2(WIDTH*0.5, 0), ENTITY_DEFAULT_RADIUS, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_CENTER, WHITE);
	}
}

void free_screen(Screen *s) {
	// TODO: We need to free on tiles if they have allocated items
	arr_free(s->tiles);
}
