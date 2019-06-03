#include "overground.h"
#include "dungeon.h"

bool fade_done;

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

    _update = update_overground;
    _draw = draw_overground;
    _update_return = _update;
    _draw_return = _draw;
}

void update_overground( void )
{
    Tile tile = get_tile_at(player.x, player.y);
    if (tile.flags & EXIT_DOWN_FLAG)
    {
        fade_done = FALSE;
        _update = wait_for_fade;
        _draw = fade_to_black;
    }

    check_player_turn();
}

void draw_overground( void )
{
    clear_buffer();
    draw_map();

    draw_mob(&player);
}


//TODO: probably move this to engine.
void fade_to_black( void )
{
    uint8_t masks[3] = {
        0b01010101, 0b11111111, 0b10101010,
    };
    uint8_t steps[3] = {
        1, 2, 1,
    };
    uint32_t timer;

    for(uint8_t i=0 ; i<3 ; i++)
    {
        timer=millis()+160;
        while(millis()<timer){}
        for (uint8_t y=0 ; y<8 ; y++)
        {
            for (uint8_t x=0 ; x<128 ; x+=steps[i])
            {
                buffer[y*SCREEN_WIDTH+x] |= masks[i];
            }
        }
        draw();
    }

    timer = millis()+160;
    while(millis()<timer){}

    fade_done = TRUE;
}

void wait_for_fade( void )
{
    if (fade_done)
    {
        player.y+=1;
        init_dungeon();
    }
}
