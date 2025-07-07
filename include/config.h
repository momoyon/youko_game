#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <entity.h>
#include <raylib.h>

#include <stdbool.h>

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;
extern float SCREEN_SCALE;

extern int HEIGHT;
extern int WIDTH;

extern bool DEBUG_DRAW;

extern RenderTexture2D ren_tex;
extern Font font;

#define ENTITY_DEFAULT_RADIUS 16

extern Entities entities;

extern Arena arena;
extern Arena temp_arena;


#endif // _CONFIG_H_
