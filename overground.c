#include "overground.h"

uint32_t button_timer;
bool fade_done;

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
        button_timer = t+BUTTON_DELAY;

        uint8_t buttons;
        buttons = read_buttons();
        if ( buttons & BTN_UP )
        {
            move_player(0, -1);
            return;
        }
        if ( buttons & BTN_DOWN )
        {
            move_player(0, 1);
            return;
        }
        if ( buttons & BTN_LEFT )
        {
            move_player(-1, 0);
            return;
        }
        if ( buttons & BTN_RIGHT )
        {
            move_player(1, 0);
            return;
        }
    }

    Tile tile = get_tile_at(&OVERGROUND, player.x, player.y);
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
    draw_map(&OVERGROUND);

    draw_sprite(&player);
}

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
        _update = update_overground;
        _draw = draw_overground;
    }
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
