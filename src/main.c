
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

	entities.items[0].pos.x = WIDTH*0.5;
	entities.items[0].pos.y = HEIGHT*0.5;

	Texture2D tile_sheet = {0};
	if (!load_texture(&tm, "resources/gfx/tile_sheet.png", &tile_sheet)) {
		return 1;
	}

	/// EDIT TILE VARS
	Vector2 edit_cursor = {0};
	Tile editing_tile = {0};

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

		switch (state) {
			case STATE_NORMAL: {
			} break;
			case STATE_TILE_EDIT: {
				// if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				//
				// }
			} break;
			case STATE_COUNT:
			default: ASSERT(false, "UNREACHABLE!");
		}

		// Update
		for (size_t i = 0; i < entities.count; ++i) {
			Entity *e = &entities.items[i];
			if (e->id == p_id) {
				control_entity(e, cc);
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
				if (IsKeyDown(KEY_Q)) {
					Rectangle tile_sheet_rect = {
						.x = WIDTH*0.5-tile_sheet.width*0.5,
						.y = HEIGHT*0.5-tile_sheet.height*0.5,
						.width = tile_sheet.width,
						.height = tile_sheet.height,
					};
					if (rect_contains_point(tile_sheet_rect, m)) {
						editing_tile.id.x = (m.x - tile_sheet_rect.x) / TILE_SIZE;
						editing_tile.id.y = (m.y - tile_sheet_rect.y) / TILE_SIZE;
					}
				}
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
						Rectangle src = {
							.x = editing_tile.id.x * TILE_SIZE,
							.y = editing_tile.id.y * TILE_SIZE,
							.width = TILE_SIZE,
							.height = TILE_SIZE,
						};

						DrawTextureRec(tile_sheet, src, edit_cursor, WHITE);
						// DrawCircleV(edit_cursor, 4, RED);
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
						
						draw_info_text(&p, arena_alloc_str(temp_arena, "Tile: %d,%d", (int)editing_tile.id.x, (int)editing_tile.id.y), ENTITY_DEFAULT_RADIUS, GRAY);
					} break;
					case STATE_COUNT:
					default: ASSERT(false, "UNREACHABLE!");
				}
			}
			// Non-debug non-camera
			switch (state) {
				case STATE_NORMAL: {
				} break;
				case STATE_TILE_EDIT: {
					if (IsKeyDown(KEY_Q)) {
						DrawRectangleV(v2xx(0), v2(WIDTH, HEIGHT), ColorAlpha(BLACK, 0.5f));
						Vector2 tpos = v2(WIDTH*0.5 - tile_sheet.width*0.5, HEIGHT*0.5 - tile_sheet.height*0.5);
						DrawTextureV(tile_sheet, tpos, WHITE);

						// Draw border around selected tile
						DrawRectangleLines(tpos.x + ((int)editing_tile.id.x * TILE_SIZE), tpos.y + ((int)editing_tile.id.y * TILE_SIZE), TILE_SIZE, TILE_SIZE, RED);
					}
				} break;
				case STATE_COUNT:
				default: ASSERT(false, "UNREACHABLE!");
			}


        EndTextureMode();
        draw_ren_tex(ren_tex, SCREEN_WIDTH, SCREEN_HEIGHT);
        EndDrawing();
	}

	close_window(ren_tex);
	return 0;
}
