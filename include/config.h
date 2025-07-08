#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <entity.h>
#include <state.h>
#include <raylib.h>

#include <stdbool.h>

#include <engine.h>
#include <screen.h>

extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;
extern float SCREEN_SCALE;

extern int HEIGHT;
extern int WIDTH;

extern bool DEBUG_DRAW;

extern RenderTexture2D ren_tex;
extern Texture_manager tm;
extern Font font;
extern Camera2D cam;
extern float cam_zoom;
extern State state;

#define CAM_SPEED 100.f

#define TILE_SIZE 32

#define ENTITY_DEFAULT_RADIUS TILE_SIZE*0.5
#define ENTITY_DEFAULT_SPEED  100.0f
#define ENTITY_DEFAULT_RUN_SPEED (ENTITY_DEFAULT_SPEED * 1.5f)

#define CAMERA_DEFAULT_ZOOM 1.5f

extern Entities entities;
extern Screens screens;
extern size_t current_screen;

extern Arena arena;
extern Arena temp_arena;


#endif // _CONFIG_H_
