#include "main.h"

#include "overground.h"

int main (void)
{
    initialise();

    _update = update_menu;
    _draw = draw_menu;

    for(ever)
    {
        _update();

        _draw();
        draw();
    }
}


void update_menu( void )
{
    uint8_t buttons;
    buttons = read_buttons();

    //TODO: Debouncing
    if(buttons == BTN_A)
    {
        init_overground();
        _update = update_overground;
        _draw = draw_overground;
    }
}

void draw_menu( void )
{
    draw_image(&TITLE, 0, 0);
    draw_battery();
}