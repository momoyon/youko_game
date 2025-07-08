#include <entity.h>
#include <config.h>

#include <raymath.h>
#include <engine.h>

size_t entity_id_counter = 0;
Entity_ids free_entity_ids = {0};

size_t get_unique_id(void) {
    if (free_entity_ids.count > 0) {
        int last_free_id = -1;
        darr_remove(free_entity_ids, int, &last_free_id, (int)free_entity_ids.count-1);
        ASSERT(last_free_id != -1, "We failed to get last free id!");
        return last_free_id;
    } else {
        return entity_id_counter++;
    }
}

Entity make_entity(Entities *entities, Vector2 pos, float radius, Entity_kind kind, Arena *arena, Arena *temp_arena) {
	Entity e = {
		.pos = pos,
		.radius = radius <= 0 ? ENTITY_DEFAULT_RADIUS : radius,
		.kind = kind,
		.id = get_unique_id(),
		.state = 0,
		.speed = ENTITY_DEFAULT_SPEED,
		.run_speed = ENTITY_DEFAULT_RUN_SPEED,
		.arena = arena,
		.temp_arena = temp_arena,
		.entities = entities,
	};

	switch (e.kind) {
		case EK_NONE: {

		} break;
		case EK_PLAYER: {

		} break;
		case EK_COUNT:
		default: ASSERT(false, "UNREACHABLE!");
	}

	return e;
}

void draw_entity(Entity *e) {
	Color c = BLUE;
	switch (e->kind) {
		case EK_NONE: {

		} break;
		case EK_PLAYER: {
			c = RED;
		} break;
		case EK_COUNT:
		default: ASSERT(false, "UNREACHABLE!");
	}

	DrawCircleV(e->pos, e->radius, c);

	if (DEBUG_DRAW) {
		arena_reset(e->temp_arena);
		const char *id_str = arena_alloc_str(*e->temp_arena, "%zu", e->id);
		draw_text_aligned(GetFontDefault(), id_str, e->pos, e->radius, TEXT_ALIGN_V_CENTER, TEXT_ALIGN_H_CENTER, WHITE);
	}
}

void control_entity(Entity *e, Control_config cc) {
	Vector2 dir = {0};
	if (IsKeyDown(cc.move_left)) {
		dir.x--;
	}
	if (IsKeyDown(cc.move_right)) {
		dir.x++;
	}
	if (IsKeyDown(cc.move_up)) {
		dir.y--;
	}
	if (IsKeyDown(cc.move_down)) {
		dir.y++;
	}
	
	float current_speed = IsKeyDown(cc.run) ? e->run_speed : e->speed;

	dir = Vector2Normalize(dir);

	e->pos = Vector2Add(Vector2Scale(dir, current_speed * GetFrameTime()), e->pos);
}

void bound_entity_to(Entity *e, Rectangle bound) {
	if (e->pos.x < bound.x + e->radius)
		e->pos.x = bound.x + e->radius;
	if (e->pos.x > bound.x + bound.width - e->radius)
		e->pos.x = bound.x + bound.width - e->radius;
	if (e->pos.y < bound.y + e->radius)
		e->pos.y = bound.y + e->radius;
	if (e->pos.y > bound.y + bound.height - e->radius)
		e->pos.y = bound.y + bound.height - e->radius;
}

void show_entity_info(Entity *e) {
	Vector2 p = v2(e->pos.x + e->radius*1.5, e->pos.y + e->radius*1.5);
	DrawLineV(e->pos, p, WHITE);
	draw_info_text(&p, arena_alloc_str(*e->temp_arena,
				"pos: %.2f, %.2f",
				e->pos.x, e->pos.y),
			ENTITY_DEFAULT_RADIUS, WHITE);
}
