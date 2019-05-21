#include "overground.h"

void init_overground( void )
{
    frame_timer = t+FRAME_DURATION;
    f = 0;
}

void update_overground( void )
{
    update_engine();

}

void draw_overground( void )
{
    clear_buffer();
    draw_map(&OVERGROUND);
}
