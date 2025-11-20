#ifndef _TILE_H_
#define _TILE_H_

#include <engine.h>
#include <raylib.h>
#include <raymath.h>

typedef struct {
	Vector2 pos;
	Vector2 size;
	Texture2D tex;
	Rectangle tex_rect;
	Vector2i id;
	bool exists;
	bool coll;
} Tile;

typedef struct {
	Tile *items;
	size_t count;
	size_t capacity;
} Tiles; // @Harr ; Heap static array

Tile load_tile_from_sheet(Texture2D sheet, Vector2 id);
void draw_tile(Tile *tile);
void draw_tiles(Tiles *tiles);

#endif // _TILE_H_
