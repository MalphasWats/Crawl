#include "overground.h"
#include "dungeon.h"

bool fade_done;

void init_overground( void )
{
    display_mode(INVERTED);

    for (uint8_t y=0 ; y<OVERGROUND.cols ; y++)
        for (uint8_t x=0 ; x<OVERGROUND.cols ; x++)
            map->tiles[y*OVERGROUND.cols+x] = OVERGROUND.tiles[y*OVERGROUND.cols+x];

    player = (Sprite){.x=11, .y=4, .offset_x=0, .offset_y=0, .tileset=&PLAYER[0]};

    _update = update_overground;
    _draw = draw_overground;
    _update_return = _update;
    _draw_return = _draw;
}

void update_overground( void )
{
    update_engine();

    Tile tile = check_move();

    //Tile tile = get_tile_at(map, player.x, player.y);
    if (tile.flags & EXIT_DOWN_FLAG)
    {
        fade_done = FALSE;
        _update = wait_for_fade;
        _draw = fade_to_black;
    }
}

void draw_overground( void )
{
    clear_buffer();
    draw_map(map);

    draw_sprite(&player);
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
