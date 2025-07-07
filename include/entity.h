#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <raylib.h>
#include <control_config.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

typedef struct Entity Entity;

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} Entity_ids;

size_t get_unique_id(void);

// NOTE: How we assign unique ids for entities:
// 1. We first check if `free_entity_ids` is empty:
//      - true If empty: go 2.
//      - false If not empty: pop last id as unique id
// 2. get `entity_id_counter` as unique id and increment `entity_id_counter`
extern size_t entity_id_counter;
extern Entity_ids free_entity_ids;

typedef enum {
    EK_NONE,
    EK_COUNT,
} Entity_kind;

const char *entity_kind_as_str(const Entity_kind k);

typedef struct {
    Entity *items;
    size_t count;
    size_t capacity;
} Entities; // @Darr

struct Entity {
    Vector2 pos;
    float radius;
    Entity_kind kind;
    size_t id;
    int state;

	float run_speed, speed;

    Texture2D tex;

    Arena *arena; // All entity-related allocations
    Arena *temp_arena;

    Entities *entities;
};

Entity make_entity(Entities *entities, Vector2 pos, float radius, Entity_kind kind, Arena *arena, Arena *temp_arena);
void draw_entity(Entity *e);
void control_entity(Entity *e, Control_config cc);

#endif // _ENTITY_H_
