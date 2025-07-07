#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <entity.h>
#include <raylib.h>

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;
extern float SCREEN_SCALE;

extern int HEIGHT;
extern int WIDTH;

extern RenderTexture2D ren_tex;
extern Font font;

#define ENTITY_DEFAULT_RADIUS 16

extern Entities entities;


#endif // _CONFIG_H_
