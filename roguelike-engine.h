#ifndef ROGUELIKE_ENGINE_H
#define ROGUELIKE_ENGINE_H

#include "GLYPH.h"
#include "ui.h"

#define FRAME_DURATION 160
#define BUTTON_DELAY 150

#define COLLIDE_FLAG        0b10000000
#define FLAG_LOS            0b01000000
#define FLAG_DOOR           0b00100000

#define EXIT_DOWN_FLAG      0b00000001

#define FLAG_CHEST          0b00000010
#define FLAG_DESTRUCTABLE   0b00000100

#define MAX_INVENTORY 8

#define MAX_MOBS 8
#define MAX_FLOATERS 5
#define FLOATER_DELAY 30

#define MAP_COLS_MAX 32
#define MAP_ROWS_MAX 16

static const __flash uint8_t INV_BRACKETS[] = {
    0xfc, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x3f, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,

    0xe7, 0x81, 0x81, 0x00, 0x00, 0x81, 0x81, 0xe7, // Cursor
};

#define CURSOR 16

typedef struct Tile {
    uint8_t data[8];
    uint8_t flags;
} Tile;

typedef struct Mob {
    int16_t x;
    int16_t y;

    int8_t offset_x;
    int8_t offset_y;

    int8_t counter;
    int8_t dx;
    int8_t dy;

    bool flipped;

    const __flash Tile* tileset;

    bool alive;
    uint8_t type;

    int8_t hp;
    int8_t max_hp;

    uint8_t damage;
    uint8_t defence;

//    uint8_t tx;
//    uint8_t ty;
    bool aggro;
} Mob;

typedef enum { FOOD, WEAPON, ARMOUR } ItemType;

typedef struct Item {
    //const __flash char* name[17];
    char name[17];
    uint8_t tile[8];
    ItemType type;
    int8_t value;
    uint8_t rarity;
} Item;

typedef struct LootTable {
    uint8_t num_items;
    Item items[];
} LootTable;

typedef struct Map {
    uint16_t cols;
    uint16_t rows;
    const __flash Tile* tileset;
    uint8_t tiles[];
} Map;

typedef struct Viewport {
    int8_t x;
    int8_t y;
    int8_t offset_x;
    int8_t offset_y;
} Viewport;

typedef struct Floater {
    uint8_t x;
    uint8_t y;
    uint8_t counter;
    uint32_t timer;
    const __flash uint8_t* tileset;
    const __flash uint8_t* masks;
    uint8_t value;
} Floater;

static const __flash uint8_t DAMAGE_FLOATERS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // padding
    0x00, 0x10, 0x10, 0x00, 0x00, 0x08, 0x3c, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x34, 0x24, 0x2c, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x24, 0x2c, 0x3c, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x0c, 0x08, 0x3c, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x2c, 0x24, 0x34, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x3c, 0x28, 0x38, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x04, 0x34, 0x0c, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x18, 0x2c, 0x18, 0x00,
    0x00, 0x10, 0x10, 0x00, 0x1c, 0x14, 0x3c, 0x00,
};
static const __flash uint8_t DAMAGE_FLOATER_MASKS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // padding
    0xef, 0xc7, 0xc7, 0xef, 0xf7, 0xc3, 0x81, 0xc3,
    0xef, 0xc7, 0xc7, 0xcb, 0x81, 0x81, 0x81, 0xd3,
    0xef, 0xc7, 0xc7, 0xcb, 0x81, 0x81, 0x81, 0xc3,
    0xef, 0xc7, 0xc7, 0xe3, 0xe1, 0xc3, 0x81, 0xc3,
    0xef, 0xc7, 0xc7, 0xc3, 0x81, 0x81, 0x81, 0xcb,
    0xef, 0xc7, 0xc7, 0xc3, 0x81, 0x83, 0x83, 0xc7,
    0xef, 0xc7, 0xc7, 0xeb, 0xc1, 0x81, 0xc1, 0xf3,
    0xef, 0xc7, 0xc7, 0xe7, 0xc3, 0x81, 0xc3, 0xe7,
    0xef, 0xc7, 0xc7, 0xe3, 0xc1, 0xc1, 0x81, 0xc3,
};

static const __flash uint8_t STATUS_FLOATERS[] = {
    0x00, 0x00, 0x00, 0x0e, 0x5e, 0x5e, 0x00, 0x00, //!
};

static const __flash uint8_t STATUS_FLOATER_MASKS[] = {
    0xff, 0xff, 0xf1, 0xa0, 0x00, 0x00, 0xa1, 0xff, //!
};

#define FLOAT_STAT_AGGRO 0*8

void init_engine( void );
void gameloop( void );

void draw_map();

void add_floater(Floater f);
void update_floaters( void );
void draw_floaters( void );

void draw_mob(Mob *s);

Tile get_tile_at(uint16_t x, uint16_t y);

void check_player_turn( void );
void move_player(int8_t dx, int8_t dy);
void player_walk_ani( void );

void mob_move_ani( void );

void set_bump_ani(int8_t dx, int8_t dy, bool turn);
void player_bump_ani( void );
void player_bumpturn_ani( void );

void reset_viewport( void );
void move_viewport( void );
void update_viewport_ani( void );

bool on_screen(uint8_t x, uint8_t y);
uint8_t distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
bool line_of_sight(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void hit_mob(Mob* attacker, Mob* defender);

void init_inventory( void );
void update_inventory( void );
void draw_inventory( Window* w );
void give_item(const __flash Item* item);

void (*_update)( void );
void (*_draw)( void );

void (*_update_return)( void );
void (*_draw_return)( void );

uint32_t t;
uint32_t frame_timer;
uint32_t button_timer;

uint8_t f;

//char message_buffer[7];
//Window message;

Mob player;
Mob mobs[MAX_MOBS];

Map* map;
const __flash LootTable* loot_table;

uint8_t collide_x;
uint8_t collide_y;
Mob* collide_mob;


static const __flash int8_t DIRX[] = { 0, 1, 0, -1,  1, 1, -1, -1};
static const __flash int8_t DIRY[] = {-1, 0, 1,  0, -1, 1,  1, -1};


#endif
