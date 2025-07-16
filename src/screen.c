#include <screen.h>
#include <config.h>

int screen_id_counter = 0;

Screen make_screen(Arena *arena, Arena *temp_arena, Texture2D tile_sheet) {
	Screen s = {
		.id = screen_id_counter++,
		.arena = arena,
		.temp_arena = temp_arena,
	};

	s.cols = WIDTH / TILE_SIZE;
	s.rows = HEIGHT / TILE_SIZE;

	arr_heap_init(s.tiles, sizeof(Tile)*s.cols*s.rows);
	s.tiles.count = s.cols * s.rows;

	for (size_t y = 0; y < s.rows; ++y) {
		for (size_t x = 0; x < s.cols; ++x) {
			size_t idx = y * s.cols + x;
			// log_debug("Set %zu to %zu %zu", idx, x, y);
			s.tiles.items[idx].tex = tile_sheet;
			s.tiles.items[idx].pos.x = x * TILE_SIZE;
			s.tiles.items[idx].pos.y = y * TILE_SIZE;
		}
	}

	return s;
}

void set_tile_at(Screen *s, Vector2 cursor, Vector2i tile_id) {
	int x = cursor.x / TILE_SIZE;
	int y = cursor.y / TILE_SIZE;

	int idx = y * s->cols + x;
	if (idx < 0 || idx > s->tiles.count-1) {
		log_error("Index is outofbounds! %d !! 0 ~ %zu", idx, s->tiles.count-1);
		exit(1);
	}

	// log_debug("BRUH idx: %d: %d %d", idx, tile_id.x, tile_id.y);
	Tile *tile = &s->tiles.items[idx];
	tile->tex_rect = CLITERAL (Rectangle) {
		.x = tile_id.x * TILE_SIZE,
		.y = tile_id.y * TILE_SIZE,
		.width = TILE_SIZE,
		.height = TILE_SIZE,
	};
	tile->size = CLITERAL(Vector2) { TILE_SIZE, TILE_SIZE };
	// log_debug("Set tile @ %f, %f | tex_rect: %f %f [%f %f]", tile->pos.x, tile->pos.y, tile->tex_rect.x, tile->tex_rect.y, tile->tex_rect.width, tile->tex_rect.height);
}

void draw_screen(Screen *s) {
	draw_tiles(&s->tiles);
}

void free_screen(Screen *s) {
	// TODO: We need to free on tiles if they have allocated items
	arr_free(s->tiles);
}
