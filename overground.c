#include "overground.h"

uint32_t button_timer;

void init_overground( void )
{
    display_mode(INVERTED);
    frame_timer = t+FRAME_DURATION;
    f = 0;

    button_timer = 0;

    player = (Sprite){.x=2, .y=2, .offset_x=0, .offset_y=0, .tileset=&PLAYER[0]};
}

void update_overground( void )
{
    update_engine();

    if (button_timer <= t)
    {
        uint8_t buttons;
        buttons = read_buttons();
        if ( buttons & BTN_UP )
        {
            move_player(0, -1);
        }
        if ( buttons & BTN_DOWN )
        {
            move_player(0, 1);
        }
        if ( buttons & BTN_LEFT )
        {
            move_player(-1, 0);
        }
        if ( buttons & BTN_RIGHT )
        {
            move_player(1, 0);
        }
        button_timer = t+BUTTON_DELAY;
    }
}

void draw_overground( void )
{
    clear_buffer();
    draw_map(&OVERGROUND);

    draw_sprite(&player);
}

void move_player(int8_t dx, int8_t dy)
{
    int16_t px = player.x+dx;
    int16_t py = player.y+dy;

    _update = update_player;

    // Simple InBounds
    if (px < 0 || px > 15 || py < 0 || py > 7)
    {
        player.offset_x = dx*8;
        player.offset_y = dy*8;
        return;
    }

    Tile tile = get_tile_at(&OVERGROUND, px, py);
    if (tile.flags & COLLIDE_FLAG)
    {
        player.offset_x = dx*8;
        player.offset_y = dy*8;
    }
    else
    {
        player.x = px;
        player.offset_x = -dx*8;

        player.y = py;
        player.offset_y = -dy*8;
    }
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
