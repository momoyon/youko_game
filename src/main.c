
#include <tile.h>

#include <config.h>
#include <common.h>

#include <screen.h>
#include <misc.h>

#define COMMONLIB_REMOVE_PREFIX
#define COMMONLIB_IMPLEMENTATION
#include <commonlib.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#define ENGINE_IMPLEMENTATION
#include <engine.h>

int main(void) {
	ren_tex = init_window(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE, "Youko Game", &WIDTH, &HEIGHT);
	init();

	font = GetFontDefault();
	if (!IsFontReady(font)) {
		log_error("Failed to get default font?");
		exit(1);
	}

	Control_config cc = {
		KEY_A, KEY_D, KEY_W, KEY_S,

		.run = KEY_LEFT_SHIFT,
	};

	size_t p_id = add_entity(v2xx(0), EK_PLAYER, &arena, &temp_arena);
	log_debug("Player id: %zu", p_id);

	Texture2D tile_sheet = {0};
	if (!load_texture(&tm, "resources/gfx/tile_sheet.png", &tile_sheet)) {
		return 1;
	}

	Vector2 edit_cursor = {0};

	while (!WindowShouldClose()) {
		arena_reset(&temp_arena);

		cam.zoom += (cam_zoom - cam.zoom) * GetFrameTime() * 10.f;

        BeginDrawing();
        Vector2 m = get_mpos_scaled(SCREEN_SCALE);
		Vector2 m_world = GetScreenToWorld2D(m, cam);

		// Input
	
		if (IsKeyPressed(KEY_TAB)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				if (state == 0) {
					state = STATE_COUNT-1;
				} else {
					state--;
				}
			} else {
				state = (state + 1) % STATE_COUNT;
			}
		}
		
		cam_zoom = CAMERA_DEFAULT_ZOOM;
		if (IsKeyDown(KEY_ONE)) {
			cam_zoom = 1.f;
		} if (IsKeyDown(KEY_TWO)) {
			cam_zoom = 2.f;
		}
		
		// Add entity
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_entity(m_world, EK_NONE, &arena, &temp_arena);
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

		// Update
		for (size_t i = 0; i < entities.count; ++i) {
			Entity *e = &entities.items[i];
			if (e->id == p_id) {
				control_entity(e, cc);
				Rectangle r = {
					.x = 0,
					.y = 0,
					.width = WIDTH,
					.height = HEIGHT
				};
				Rectangle rw = {
					.x = GetScreenToWorld2D(v2(0,0), cam).x,
					.y = GetScreenToWorld2D(v2(0,0), cam).y,
				};
				rw.width  = GetScreenToWorld2D(v2(WIDTH, HEIGHT), cam).x - rw.x;
				rw.height = GetScreenToWorld2D(v2(WIDTH, HEIGHT), cam).y - rw.y;
				bound_entity_to(e, rw);
			}
		}

		switch (state) {
			case STATE_NORMAL: {
			} break;
			case STATE_TILE_EDIT: {
				edit_cursor = snap_to_tile(m_world);
			} break;
			case STATE_COUNT:
			default: ASSERT(false, "UNREACHABLE!");
		}

		// Draw
        BeginTextureMode(ren_tex);
            ClearBackground(BLACK);
			BeginMode2D(cam);
				for (size_t i = 0; i < entities.count; ++i) {
					Entity *e = &entities.items[i];
					draw_entity(e);
					if (e->id == p_id) {
						show_entity_info(e);
					}
				}

				switch (state) {
					case STATE_NORMAL: {
					} break;
					case STATE_TILE_EDIT: {
						DrawCircleV(edit_cursor, 4, RED);
					} break;
					case STATE_COUNT:
					default: ASSERT(false, "UNREACHABLE!");
				}

			EndMode2D();

			if (DEBUG_DRAW) {
				Vector2 p = v2(10, 10);
				draw_info_text(&p, state_as_str(state), ENTITY_DEFAULT_RADIUS, YELLOW);
				draw_info_sep(&p, 2.f, 100, WHITE);

				draw_info_text(&p, arena_alloc_str(temp_arena, "Cam %.2f, %.2f (%.2f)", cam.target.x, cam.target.y, cam_zoom), ENTITY_DEFAULT_RADIUS, WHITE);

				switch (state) {
					case STATE_NORMAL: {
					} break;
					case STATE_TILE_EDIT: {
						draw_info_sep(&p, 2.f, 100, WHITE);
						draw_info_text(&p, arena_alloc_str(temp_arena, "Screen %zu", current_screen), ENTITY_DEFAULT_RADIUS, GRAY);
					} break;
					case STATE_COUNT:
					default: ASSERT(false, "UNREACHABLE!");
				}
			}


        EndTextureMode();
        draw_ren_tex(ren_tex, SCREEN_WIDTH, SCREEN_HEIGHT);
        EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}
