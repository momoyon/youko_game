#include <config.h>

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 960;
float SCREEN_SCALE = 0.5f;

int WIDTH = -1;
int HEIGHT = -1;

#ifdef DEBUG
bool DEBUG_DRAW = true;
#else
bool DEBUG_DRAW = false;
#endif

RenderTexture2D ren_tex = {0};
Texture_manager tm = {0};
Font font = {0};
Camera2D cam = {
	.zoom = 1.f,
};
State state = STATE_NORMAL;

Entities entities = {0};
Screens screens = {0};
size_t current_screen = 0;

Arena arena = {0};
Arena temp_arena = {0};
