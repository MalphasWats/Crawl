#include "overground.h"

uint32_t t;
uint32_t frame_timer;

uint8_t f;

void init_overground( void )
{
    t = 0;
    frame_timer = t+FRAME_DURATION;

    f = 0;
}

void update_overground( void )
{
    t = millis();
    if (frame_timer <= t)
    {
        f += 1;
        if ( f == 4 )
        {
            f = 0;
        }
        frame_timer = t+FRAME_DURATION;
    }

}

void draw_overground( void )
{
    clear_buffer();
    draw_map(&OVERGROUND);
}
