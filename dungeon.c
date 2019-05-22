#include "dungeon.h"

#include "tiles.h"

void init_dungeon( void )
{
    display_mode(NORMAL);
    // Copy test map
    map->tileset = &TILES[0];

    for (uint16_t i=0 ; i<32*16 ; i++)
        map->tiles[i] = TEST_DUNGEON.tiles[i];

    player = (Sprite){.x=3, .y=3, .offset_x=0, .offset_y=0, .tileset=&PLAYER[0]};
}

void update_dungeon( void )
{
    update_engine();
}

void draw_dungeon( void )
{
    clear_buffer();
    draw_map(map);

    draw_sprite(&player);
}
