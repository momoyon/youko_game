#ifndef _COMMON_H_
#define _COMMON_H_

#include <entity.h>

void init(void);

size_t add_entity(Vector2 pos, Entity_kind kind, Arena *arena, Arena *temp_arena);

#endif // _COMMON_H_
