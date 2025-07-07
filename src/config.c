#include <config.h>

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
float SCREEN_SCALE = 0.5f;

int WIDTH = 1280;
int HEIGHT = 720;

#ifdef DEBUG
bool DEBUG_DRAW = true;
#else
bool DEBUG_DRAW = false;
#endif

RenderTexture2D ren_tex = {0};
Font font = {0};

Entities entities = {0};

Arena arena = {0};
Arena temp_arena = {0};
