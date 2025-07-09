#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <tile.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

extern int screen_id_counter;
typedef struct Screen Screen;

typedef struct {
	Screen *items;
	size_t count;
	size_t capacity;
} Screens; // @Darr

struct Screen {
	Tiles tiles;
	size_t id;
	int cols, rows;
	Arena *arena;
	Arena *temp_arena;
};

Screen make_screen(Arena *arena, Arena *temp_arena, Texture2D tile_sheet);
void set_tile_at(Screen *s, Vector2 cursor, Vector2i tile_id);
void draw_screen(Screen *s);
void free_screen(Screen *s);

#endif // _SCREEN_H_
