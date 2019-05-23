#include "dungeon.h"

#include "tiles.h"

void init_dungeon( void )
{
    display_mode(NORMAL);
    // Copy test map
    map->tileset = &TILES[0];
    map->cols = 32;
    map->rows = 16;
    for (uint16_t i=0 ; i<32*16 ; i++)
        map->tiles[i] = TEST_DUNGEON.tiles[i];

    //player = (Sprite){.x=3, .y=3, .offset_x=0, .offset_y=0, .tileset=&PLAYER[0]};
    player.x=3;
    player.y=3;

    _update = update_dungeon;
    _draw = draw_dungeon;
    _update_return = _update;
    _draw_return = _draw;
}

void update_dungeon( void )
{
    update_engine();

    // Check where we're standing here...

    check_move();
}

void draw_dungeon( void )
{
    clear_buffer();
    draw_map();//map);

    draw_sprite(&player);
}
