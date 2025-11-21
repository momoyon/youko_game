#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>

#include <stb_ds.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

// Vector helpers
Vector2 v2xx(float v);
Vector2 v2(float x, float y);
#define v2_sub Vector2Subtract
#define v2_mag2 Vector2LengthSqr

// Vector2i
typedef struct {
	int x, y;
} Vector2i;

Vector2i v2vi(Vector2 v);
bool v2i_equal(Vector2i a, Vector2i b);

// Rectangle
bool rect_contains_point(Rectangle r1, Vector2 p);
bool rect_contains_rect(Rectangle r1, Rectangle r2);
bool rect_intersects_rect(Rectangle r1, Rectangle r2);
bool rect_resolve_rect_collision(Rectangle* rect1, const Rectangle rect2);
int rect_resolve_circle_collision(Vector2 *circlePos, float radius, Rectangle rect)
// void rect_get_3d_points(Rectangle rect, Vector3f* p0, Vector3f* p1, Vector3f* p2, Vector3f* p3);
// void rect_get_points(Rectangle rect, Vector2* p0, Vector2* p1, Vector2* p2, Vector2* p3);

// Window
RenderTexture2D init_window(int screen_width, int screen_height, float scl, const char *title, int *width_out, int *height_out);
void close_window(RenderTexture2D ren_tex);

typedef enum {
	TEXT_ALIGN_H_LEFT = 0,
	TEXT_ALIGN_H_CENTER,
	TEXT_ALIGN_H_RIGHT,
	TEXT_ALIGN_H_COUNT,
} Text_align_h;

// NOTE: Start enum at 10 to not conflict with Text_align_h
typedef enum {
	TEXT_ALIGN_V_TOP = 10,
	TEXT_ALIGN_V_CENTER,
	TEXT_ALIGN_V_BOTTOM,
	TEXT_ALIGN_V_COUNT,
} Text_align_v;

// Draw
void draw_ren_tex(RenderTexture2D ren_tex, int screen_width, int screen_height);
void draw_text_aligned(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, Color color);
void draw_text_aligned_ex(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, float rotation, Color color);
void draw_text(Font font, const char *text, Vector2 pos, int font_size, Color color);
void draw_info_text(Vector2 *p, const char *text, int font_size, Color color);
void draw_info_sep(Vector2 *p, int thickness, float len, Color color);

// Misc
Vector2 get_mpos_scaled(float scl);

// Assets Manager
typedef struct {
	const char *key;
	Texture2D value;
} Texture_KV;

typedef struct {
	Texture_KV *texture_map;
} Texture_manager;

bool load_texture(Texture_manager *tm, const char *filepath, Texture2D *tex_out);

// UI


#endif // _ENGINE_H_

// IMPLEMENTATION ////////////////////////////////
#ifdef ENGINE_IMPLEMENTATION

// Vector helpers
Vector2 v2xx(float v) { return CLITERAL(Vector2) { v, v }; }
Vector2 v2(float x, float y) { return CLITERAL(Vector2) { x, y }; }

// Vector2i
Vector2i v2vi(Vector2 v) { return CLITERAL(Vector2i) { (int)v.x, (int)v.y }; }

bool v2i_equal(Vector2i a, Vector2i b) {
	return a.x == b.x && a.y == b.y;
}

// Rectangle
bool rect_contains_point(Rectangle r1, Vector2 p) {
  return (p.x >= r1.x && p.x < r1.x + r1.width &&
	  p.y >= r1.y && p.y < r1.y + r1.height);
}

bool rect_contains_rect(Rectangle r1, Rectangle r2) {
  return (rect_contains_point(r1, v2(r2.x, r2.y)) &&
	  rect_contains_point(r1, (Vector2) {r2.x + r2.width,
					     r2.y + r2.height}));
}

bool rect_intersects_rect(Rectangle r1, Rectangle r2) {
  const float rect1_l = r1.x;
  const float rect1_r = r1.x+r1.width;
  const float rect1_t = r1.y;
  const float rect1_b = r1.y+r1.height;

  const float rect2_l = r2.x;
  const float rect2_r = r2.x+r2.width;
  const float rect2_t = r2.y;
  const float rect2_b = r2.y+r2.height;

  return (rect1_r >= rect2_l &&
	  rect1_l <= rect2_r &&
	  rect1_t <= rect2_b &&
	  rect1_b >= rect2_t);
}

bool rect_resolve_rect_collision(Rectangle* rect1, const Rectangle rect2) {
  const float rect1_l = rect1->x;
  const float rect1_r = rect1->x+rect1->width;
  const float rect1_t = rect1->y;
  const float rect1_b = rect1->y+rect1->height;

  const float rect2_l = rect2.x;
  const float rect2_r = rect2.x+rect2.width;
  const float rect2_t = rect2.y;
  const float rect2_b = rect2.y+rect2.height;

  // resolve collision only if it ever happens
  if (rect_intersects_rect(*rect1, rect2)) {
    Vector2 cb2_bot = {0.f, rect2_b};
    Vector2 cb1_top = {0.f, rect1_t};
    float d2_top = v2_mag2(v2_sub(cb1_top, cb2_bot));
    Vector2 cb2_left = {rect2_l, 0.f};
    Vector2 cb1_right = {rect1_r, 0.f};
    float d2_right = v2_mag2(v2_sub(cb1_right, cb2_left));
    Vector2 cb2_right = {rect2_r, 0.f};
    Vector2 cb1_left = {rect1_l, 0.f};
    float d2_left = v2_mag2(v2_sub(cb1_left, cb2_right));
    Vector2 cb2_top = {0.f, rect2_t};
    Vector2 cb1_bot = {0.f, rect1_b};
    float d2_bot = v2_mag2(v2_sub(cb1_bot, cb2_top));

    float min_d2 = fminf(d2_top, fminf(d2_left, fminf(d2_right, d2_bot)));

    if (min_d2 == d2_top) {
      rect1->y = rect2_b;
    } else if (min_d2 == d2_left) {
      rect1->x = rect2_r;
    } else if (min_d2 == d2_right) {
      rect1->x = rect2_l - rect1->width;
    } else if (min_d2 == d2_bot) {
      rect1->y = rect2_t - rect1->height;
    } else {
      ASSERT(0, "UNREACHABLE");
    }
    return true;
  }
  return false;
}

int rect_resolve_circle_collision(Vector2 *circlePos, float radius, Rectangle rect)
{
    /* Find closest point on rect to circle center */
    float closestX = circlePos->x;
    if (closestX < rect.x) closestX = rect.x;
    else if (closestX > rect.x + rect.width) closestX = rect.x + rect.width;

    float closestY = circlePos->y;
    if (closestY < rect.y) closestY = rect.y;
    else if (closestY > rect.y + rect.height) closestY = rect.y + rect.height;

    /* Vector from closest point to circle center */
    float dx = circlePos->x - closestX;
    float dy = circlePos->y - closestY;
    float distSq = dx*dx + dy*dy;

    /* No collision if distance squared greater than radius squared */
    if (distSq >= radius*radius || distSq == 0.0f)
    {
        /* Handle special case: circle center exactly on closest point (corner on center or center exactly on edge).
           If center is inside rectangle (distSq == 0) we must push the circle out along the shortest axis.
        */
        if (distSq == 0.0f)
        {
            /* Determine if center is inside rect */
            if (circlePos->x > rect.x && circlePos->x < rect.x + rect.width &&
                circlePos->y > rect.y && circlePos->y < rect.y + rect.height)
            {
                /* Compute penetration depths to each side */
                float leftPen  = circlePos->x - rect.x;
                float rightPen = rect.x + rect.width - circlePos->x;
                float topPen   = circlePos->y - rect.y;
                float bottomPen= rect.y + rect.height - circlePos->y;

                /* Find minimal penetration axis */
                float minPen = leftPen;
                int axis = 0; /* 0 = left, 1 = right, 2 = top, 3 = bottom */
                if (rightPen < minPen) { minPen = rightPen; axis = 1; }
                if (topPen < minPen) { minPen = topPen; axis = 2; }
                if (bottomPen < minPen) { minPen = bottomPen; axis = 3; }

                /* Push circle out along that axis so circle just touches rect edge */
                switch(axis)
                {
                    case 0: circlePos->x = rect.x - radius; break;                 /* left */
                    case 1: circlePos->x = rect.x + rect.width + radius; break;   /* right */
                    case 2: circlePos->y = rect.y - radius; break;                /* top */
                    case 3: circlePos->y = rect.y + rect.height + radius; break;  /* bottom */
                }
                return 1;
            }
        }
        return 0;
    }

    /* Collision: compute distance and push circle out along separation vector */
    float dist = sqrtf(distSq);
    /* Normalized separation vector from rect to circle */
    float nx = dx / dist;
    float ny = dy / dist;
    /* Amount to move circle so it's exactly radius away from closest point */
    float penetration = radius - dist;
    circlePos->x += nx * penetration;
    circlePos->y += ny * penetration;

    return 1;
}

// void rect_get_3d_points(Rectangle rect, Vector3f* p0, Vector3f* p1, Vector3f* p2, Vector3f* p3) {
//   Vector2 p0_ = v2_add(rect.pos, (Vector2) {0.f, 0.f});
//   Vector2 p1_ = v2_add(rect.pos, (Vector2) {rect.size.x, 0.f});
//   Vector2 p2_ = v2_add(rect.pos, (Vector2) {rect.size.x, rect.size.y});
//   Vector2 p3_ = v2_add(rect.pos, (Vector2) {0.f, rect.size.y});
//
//   *p0 = (Vector3f) {p0_.x, p0_.y, 0.f};
//   *p1 = (Vector3f) {p1_.x, p1_.y, 0.f};
//   *p2 = (Vector3f) {p2_.x, p2_.y, 0.f};
//   *p3 = (Vector3f) {p3_.x, p3_.y, 0.f};
// }
//
// void rect_get_points(Rectangle rect, Vector2* p0, Vector2* p1, Vector2* p2, Vector2* p3) {
//   *p0 = v2_add(rect.pos, (Vector2) {0.f, 0.f});
//   *p1 = v2_add(rect.pos, (Vector2) {rect.size.x, 0.f});
//   *p2 = v2_add(rect.pos, (Vector2) {rect.size.x, rect.size.y});
//   *p3 = v2_add(rect.pos, (Vector2) {0.f, rect.size.y});
// }
//

// Setup
RenderTexture2D init_window(int screen_width, int screen_height, float scl, const char *title, int *width_out, int *height_out) {
	SetTraceLogLevel(LOG_NONE);
	InitWindow(screen_width, screen_height, title);

    int width = screen_width * scl;
    int height = screen_height * scl;

    *width_out = width;
    *height_out = height;

	log_info("Created Window with dimensions %dx%d", screen_width, screen_height);

	RenderTexture2D ren_tex = LoadRenderTexture((int)(width), (int)(height));
	if (!IsRenderTextureReady(ren_tex)) {
		log_error("Failed to create RenderTexture2D!");
		exit(1);
	}
	log_info("Created RenderTexture2D with dimensions %dx%d (Scaled down by %.2f)", ren_tex.texture.width, ren_tex.texture.height, scl);

	return ren_tex;
}

void close_window(RenderTexture2D ren_tex) {
	UnloadRenderTexture(ren_tex);
	CloseWindow();
}

// Draw
void draw_ren_tex(RenderTexture2D ren_tex, int screen_width, int screen_height) {
	const Rectangle src = {
		.x = 0,
		.y = 0,
		.width = ren_tex.texture.width,
		// NOTE: We flip the height because y-axis is flipped internally (in opengl land probably)
		.height = -ren_tex.texture.height,
	};

	const Rectangle dst = {
		.x = 0,
		.y = 0,
		.width  = screen_width,
		.height = screen_height,
	};
	DrawTexturePro(ren_tex.texture, src, dst, CLITERAL(Vector2) { 0.f, 0.f }, 0.f, WHITE);
}

void draw_text_aligned(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, Color color) {
    draw_text_aligned_ex(font, text, pos, font_size, align_v, align_h, 0.0, color);
}

void draw_text_aligned_ex(Font font, const char *text, Vector2 pos, int font_size, const Text_align_v align_v, const Text_align_h align_h, float rotation, Color color) {
	Vector2 origin = {0};
	// RLAPI Vector2 MeasureTextEx(Font font, const char *text, float fontSize, float spacing);    // Measure string size for Font
	float spacing = 2.f;
	Vector2 text_size = MeasureTextEx(font, text, font_size, spacing);

	switch (align_h) {
		case TEXT_ALIGN_H_LEFT: {
		} break;
		case TEXT_ALIGN_H_CENTER: {
			origin.x = text_size.x * 0.5f;
		} break;
		case TEXT_ALIGN_H_RIGHT: {
			origin.x = text_size.x;
		} break;
		case TEXT_ALIGN_H_COUNT: {
		} break;
		default: ASSERT(false, "UNREACHABLE");
	}

	switch (align_v) {
		case TEXT_ALIGN_V_TOP: {
		} break;
		case TEXT_ALIGN_V_CENTER: {
			origin.y = text_size.y * 0.5f;
		} break;
		case TEXT_ALIGN_V_BOTTOM: {
			origin.y = text_size.y;
		} break;
		case TEXT_ALIGN_V_COUNT: {
		} break;
		default: ASSERT(false, "UNREACHABLE");
	}

	DrawTextPro(font, text, pos, origin, rotation, font_size, spacing, color);
}

void draw_text(Font font, const char *text, Vector2 pos, int font_size, Color color) {
	draw_text_aligned(font, text, pos, font_size, TEXT_ALIGN_V_TOP, TEXT_ALIGN_H_LEFT, color);
}

void draw_info_text(Vector2 *p, const char *text, int font_size, Color color) {
    draw_text(GetFontDefault(), text, *p, font_size, color);
    p->y += font_size + 2;
}

void draw_info_sep(Vector2 *p, int thickness, float len, Color color) {
	Vector2 end = {
		.x = p->x + len,
		.y = p->y
	};
	DrawLineEx(*p, end, thickness, color);
	p->y += thickness + 2.f;
}

// Misc
Vector2 get_mpos_scaled(float scl) {
	Vector2 m = GetMousePosition();
	m.x *= scl;
	m.y *= scl;
	return m;
}

// Assets Manager
bool load_texture(Texture_manager *tm, const char *filepath, Texture2D *tex_out) {
	Texture_KV *tex_KV = hmgetp_null(tm->texture_map, filepath);

	if (tex_KV != NULL) {
		*tex_out = tex_KV->value;
		log_debug("Found '%s' at texture_map index [%zu]", filepath, hmlenu(tm->texture_map));
	} else {
		Texture2D tex = LoadTexture(filepath);
		if (!IsTextureReady(tex)) return false;
		*tex_out = tex;
		hmput(tm->texture_map, filepath, tex);
		log_debug("Added '%s' to texture_map index [%zu]", filepath, hmlenu(tm->texture_map));
	}

	return true;
}

#endif // ENGINE_IMPLEMENTATION
