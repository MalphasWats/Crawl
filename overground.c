#include "overground.h"

void init_overground( void )
{
    frame_timer = t+FRAME_DURATION;
    f = 0;

    player = (Sprite){.x=2, .y=2, .offset_x=0, .offset_y=0, .tileset=&PLAYER[0]};
}

void update_overground( void )
{
    update_engine();

    uint8_t buttons;
    buttons = read_buttons();
    if ( buttons & BTN_UP )
    {
        player.y -= 1;
        player.offset_y = 8;

        _update = update_player;
    }
    if ( buttons & BTN_DOWN )
    {
        player.y += 1;
        player.offset_y = -8;

        _update = update_player;
    }
    if ( buttons & BTN_LEFT )
    {
        player.x -= 1;
        player.offset_x = 8;

        _update = update_player;
    }
    if ( buttons & BTN_RIGHT )
    {
        player.x += 1;
        player.offset_x = -8;

        _update = update_player;
    }
}

void draw_overground( void )
{
    clear_buffer();
    draw_map(&OVERGROUND);

    draw_sprite(player);
}


void update_player( void )
{
    if ( player.offset_x < 0 )
    {
        player.offset_x += 1;
    }
    if ( player.offset_x > 0 )
    {
        player.offset_x -= 1;
    }
    if ( player.offset_y < 0 )
    {
        player.offset_y += 1;
    }
    if ( player.offset_y > 0 )
    {
        player.offset_y -= 1;
    }

    if ( player.offset_x == 0 && player.offset_y == 0)
    {
        _update = update_overground;
    }
}
