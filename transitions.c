#include "transitions.h"

//TODO: transition_done (when I have more!)
bool fade_done;

void set_transition(uint8_t transition, void (*return_to)( void ))
{
    fade_done = FALSE;
    _update_return = return_to;
    _update = wait_for_fade;
    _draw = fade_to_black;
}

void fade_to_black( void )
{
    uint8_t masks[3] = {
        //0b01010101, 0b11111111, 0b10101010,
        0b01010101, 0b00000000, 0b10101010,
    };
    uint8_t steps[3] = {
        1, 2, 1,
    };
    uint32_t timer;

    for(uint8_t i=0 ; i<3 ; i++)
    {
        timer=millis()+FADE_RATE;
        while(millis()<timer){}
        for (uint8_t y=0 ; y<SCREEN_ROWS ; y++)
        {
            for (uint8_t x=0 ; x<SCREEN_WIDTH ; x+=steps[i])
            {
                //buffer[y*SCREEN_WIDTH+x] |= masks[i];
                buffer[y*SCREEN_WIDTH+x] &= masks[i];
            }
        }
        draw();
    }

    timer = millis()+FADE_RATE;
    while(millis()<timer){}

    fade_done = TRUE;
}

void wait_for_fade( void )
{
    if (fade_done)
    {
        _update = _update_return;
        _draw = clear_buffer;
    }
}
