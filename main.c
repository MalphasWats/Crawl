#include "main.h"

#include "overground.h"
#include "ui.h"

uint32_t timer;

int main (void)
{
    initialise();
    init_engine();
    _update = update_menu;
    _draw = draw_menu;

    timer=0;

    for(ever)
    {
        gameloop();
    }
}


void update_menu( void )
{
    if (timer <= t)
    {
        rng();
        timer = t+15;
    }

    uint8_t buttons;
    buttons = read_buttons();

    //TODO: Debouncing
    if(buttons == BTN_A)
    {
        init_overground();
    }
}

void draw_menu( void )
{
    draw_image(&TITLE, 0, 0);
    draw_battery();
}
