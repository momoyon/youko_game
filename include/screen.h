#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <tile.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

extern int screen_id_counter;

typedef struct {
	Tiles tiles;
	size_t id;
	int cols, rows;
	Arena *arena;
	Arena *temp_arena;

	//
	const char *screen_str;
} Screen;

Screen make_screen(Arena *arena, Arena *temp_arena);
void draw_screen(Screen *s);
void free_screen(Screen *s);

#endif // _SCREEN_H_
