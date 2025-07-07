#define ENGINE_IMPLEMENTATION
#include <engine.h>

#include <config.h>
#include <common.h>

#define COMMONLIB_REMOVE_PREFIX
#define COMMONLIB_IMPLEMENTATION
#include <commonlib.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>


int main(void) {
	ren_tex = init_window(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE, "Youko Game", &WIDTH, &HEIGHT);

	font = GetFontDefault();
	if (!IsFontReady(font)) {
		log_error("Failed to get default font?");
		exit(1);
	}

	Control_config cc = {
		KEY_A, KEY_D, KEY_W, KEY_S,

		.run = KEY_LEFT_SHIFT,
	};

	init();
	while (!WindowShouldClose()) {
        BeginDrawing();
        Vector2 m = get_mpos_scaled(SCREEN_SCALE);
		// Vector2 m_world = GetScreenToWorld2D(m, cam);

		// Input
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_entity(m, EK_NONE, &arena, &temp_arena);
		}

		// DEBUG: Move camera
		if (IsKeyDown(KEY_J)) {
			cam.target.x -= CAM_SPEED * GetFrameTime();
		}
		if (IsKeyDown(KEY_L)) {
			cam.target.x += CAM_SPEED * GetFrameTime();
		}
		if (IsKeyDown(KEY_I)) {
			cam.target.y -= CAM_SPEED * GetFrameTime();
		}
		if (IsKeyDown(KEY_K)) {
			cam.target.y += CAM_SPEED * GetFrameTime();
		}

		for (size_t i = 0; i < entities.count; ++i) {
			Entity *e = &entities.items[i];
			if (i == 0) control_entity(e, cc);
		}

		// Draw
        BeginTextureMode(ren_tex);
			draw_text(font, "Hello Buddy", m, 18, WHITE);
			BeginMode2D(cam);
				for (size_t i = 0; i < entities.count; ++i) {
					Entity *e = &entities.items[i];
					draw_entity(e);
				}
			EndMode2D();
            ClearBackground(BLACK);
        EndTextureMode();
        draw_ren_tex(ren_tex, SCREEN_WIDTH, SCREEN_HEIGHT);
        EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}
