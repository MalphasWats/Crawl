#include "overground.h"
#include "dungeon.h"
#include "transitions.h"

void init_overground( void )
{
    display_mode(INVERTED);

    map->tileset = OVERGROUND.tileset;
    map->cols = OVERGROUND.cols;
    map->rows = OVERGROUND.rows;

    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            map->tiles[y*map->cols+x] = OVERGROUND.tiles[y*map->cols+x];
        }
    }

    for (uint8_t m=0 ; m<MAX_MOBS ; m++)
    {
        mobs[m].type = 0;
        mobs[m].alive = FALSE;
    }

    player = (Mob){
        .x=11,
        .y=4,
        .offset_x=0,
        .offset_y=0,
        .tileset=&PLAYER[0],
        .flipped=FALSE,
        .alive=TRUE,
        .type=MOB_PLAYER,

        .hp=5,
        .max_hp=5,
        .damage=1,
        .defence=0,
    };
    init_inventory();

    _update = update_overground;
    _draw = draw_overground;
    _update_return = _update;
    _draw_return = _draw;


    reset_viewport();
}

void update_overground( void )
{
    Tile tile = get_tile_at(player.x, player.y);
    if (tile.flags & EXIT_DOWN_FLAG)
    {
        set_transition(FADE_TO_BLACK, init_dungeon);
    }

    check_player_turn();
}

void draw_overground( void )
{
    clear_buffer();
    draw_map();

    draw_mob(&player);
}
