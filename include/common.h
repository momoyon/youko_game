#ifndef _COMMON_H_
#define _COMMON_H_

#include <entity.h>
#include <raylib.h>

void init(Texture2D tile_sheet);

size_t add_entity(Vector2 pos, Entity_kind kind, Arena *arena, Arena *temp_arena);

#endif // _COMMON_H_
