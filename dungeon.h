#ifndef MAIN_H
# define MAIN_H

#include "GLYPH.h"
#include "roguelike-engine.h"
#include "ui.h"

#include "tiles.h"

static const __flash Map TEST_DUNGEON = {
    .cols = 32,
    .rows = 16,
    .tileset = &TILES[0],
    .tiles = {
        12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
        12,  12,   8,  15,  12,  12,   3,   3,   3,  12,  12,  12,  12,  12,  12,   3,   3,   3,   3,  12,  12,  12,   3,  12,  12,  12,  12,  12,   9,   3,  12,  12,
        12,   3,   3,  15,  12,  15,   3,   3,  13,  12,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,  12,  12,  12,   3,  12,  12,
        12,   3,  12,  12,  12,  15,   3,   3,   3,   3,   3,  15,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,  16,   3,  12,  12,  12,  12,  12,   3,  12,  12,
        12,   3,  12,  12,  12,  15,   3,   3,   3,   3,   3,  15,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,  12,  12,  12,  12,  12,  12,   3,   3,  12,
        12,   3,  12,   3,  12,  12,   3,   3,   3,   3,   3,  12,  12,  12,  12,  12,   3,   3,   3,  12,  12,   3,   3,  12,  12,  12,  15,  12,  12,  12,   3,  12,
        12,   3,   3,   3,  12,  12,   3,   3,   3,   3,   3,  12,  12,  12,  12,  12,   3,   3,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,
        12,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,   3,   3,   3,   3,   3,   3,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,
        12,  12,  12,  12,   3,   3,   3,   3,  16,   3,   3,   3,   3,  16,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,
        12,  12,  12,  12,   3,   3,   3,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,
        12,  12,  12,  12,  12,  12,   3,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,  12,   3,   3,  16,   3,   3,   3,   3,   3,   3,   3,  12,
        12,  13,   3,   3,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,
        12,  16,   3,   3,   3,   3,   3,   3,   3,   3,  12,  12,   3,   3,   3,   3,   3,  12,  12,  12,  12,  12,   3,   3,   3,   3,   3,   3,  16,  12,  12,  12,
        12,  12,  12,  12,   3,   3,   3,  12,  12,  12,  12,  12,   3,   3,   3,  12,   3,   3,  12,  12,  12,  12,  12,  15,  12,  12,   3,  16,  15,  12,  12,  12,
        12,  12,  12,  12,   3,  15,  15,  12,  12,  12,  13,   3,   3,   3,   3,  12,   3,   3,   3,   3,  12,  12,  12,  12,  12,  12,  12,  15,  13,  12,  12,  12,
        12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
    }
};

static const __flash LootTable LOOT_TABLE = {
    .num_items=3,
    .items = {
        {.name="Rusty Sword", .tile={0x03, 0x05, 0x0a, 0x54, 0x68, 0x30, 0x58, 0x80,}, .type=WEAPON, .value=1, .rarity=0,},
        {.name="Leather Armour", .tile={0x0e, 0x67, 0xff, 0xfc, 0xfc, 0xff, 0x67, 0x0e,}, .type=ARMOUR, .value=1, .rarity=0,},
        {.name="Red Potion", .tile={0x00, 0x70, 0xe9, 0xe3, 0xef, 0xe9, 0x70, 0x00,}, .type=FOOD, .value=1, .rarity=1,},
    },
};

#define RED_POTION 2

void init_dungeon( void );
void update_dungeon( void );
void draw_dungeon( void );

void wndw_level(Window* w);
void draw_hp(Window* w);

void generate_random_dungeon( void );
void cellular_automaton( void );
void trim_holes( void );
void kill_diags( void );
uint8_t label_caverns( void );
void reset_labels( void );
void fill_cavern( uint8_t x, uint8_t y, uint8_t old_flag, uint8_t new_flag );
void carve_tunnels( void );
void fill_small_caverns( void );
void place_chests( void );

void generate_dungeon( void );
void spawn_mob(uint8_t x, uint8_t y, uint8_t mob);

void update_dead( void );
void draw_dead( void );
#endif
