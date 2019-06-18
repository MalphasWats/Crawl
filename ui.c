#include "ui.h"
#include "ASCII-SMALL.h"

Window message_window;
char message_buffer[24];

void init_windows( void )
{
    for(uint8_t i=0 ; i<MAX_WINDOWS ; i++)
    {
        windows[i] = 0;
    }
}

bool show_window(Window* w, uint16_t lifetime)
{
    if (lifetime > 0)
        w->timer = millis() + lifetime;
    else
        w->timer = 0;

    for(uint8_t i=0 ; i<MAX_WINDOWS ; i++)
    {
        if (windows[i] == 0)
        {
            windows[i] = w;
            return TRUE;
        }
    }
    return FALSE;
}

void close_window(Window* w)
{
    w->timer = 1;
}

void update_windows( void )
{
    for(uint8_t i=0 ; i<MAX_WINDOWS ; i++)
    {
        if (windows[i] != 0 && windows[i]->timer > 0 && windows[i]->timer <= millis())
        {
            windows[i] = 0;
        }
    }
}

void draw_windows( void )
{
    for(uint8_t i=0 ; i<MAX_WINDOWS ; i++)
    {
        if (windows[i] != 0)
        {
            draw_window(windows[i]);
        }
    }
}

void draw_window(Window* w)
{
    uint8_t tile;
    for(uint8_t y=0 ; y<w->h ; y++)
    {
        for(uint8_t x=0 ; x<w->w ; x++)
        {
            if (x==0 && y==0)
                tile = WIN_TL;
            else if (x==w->w-1 && y==0)
                tile = WIN_TR;
            else if (x==0 && y==w->h-1)
                tile = WIN_BL;
            else if (x==w->w-1 && y==w->h-1)
                tile = WIN_BR;
            else if (x==0)
                tile = WIN_L;
            else if (x==w->w-1)
                tile = WIN_R;
            else if (y==0)
                tile = WIN_T;
            else if (y==w->h-1)
                tile = WIN_B;
            else
                tile = WIN_EM;

            draw_tile(&WINDOW_TILES[tile], &WINDOW_TILE_MASKS[tile], (w->x+x)*8, (w->y+y)*8, FALSE);
        }
    }
    //TODO: deal with \n in content
    w->_draw(w);
    //draw_string(w->content, (w->x+1)*8, (w->y+1)*8);
}

void draw_small_string(const __memx char *string, int16_t x, int16_t y)
{
    for(uint8_t i=0 ; string[i] != '\0' ; i++)
    {
        draw_tile(&SMALL_ASCII[(string[i]-32)*8], &SMALL_CHAR_MASK[0], x+(i*4), y, FALSE);
    }
}

void draw_small_int(int16_t n, uint8_t width, int16_t x, int16_t y)
{
    //TODO: Negative numbers
    if (width == 0)
        width = num_digits(n);
    int32_t n_;
    for (uint8_t i=0 ; i<width ; i++)
    {
        n_ = (6554*(int32_t)n)>>16;
        draw_tile(&SMALL_DIGITS[(n - (n_*10))*8], &SMALL_CHAR_MASK[0], x+((width-i-1)*4), y, FALSE);
        n = (int16_t)n_;
    }
}

uint8_t num_digits(int16_t n)
{
    //TODO: negative numbers
    if (n < 10)
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    return 5;
}

void show_message(const __memx char* message)
{
    uint8_t len = 0;
    for(uint8_t i=0 ; message[i] != '\0' ; i++)
    {
        message_buffer[i] = message[i];
        len += 1;
    }
    message_buffer[len] = '\0';

    if (len & 1)
        len += 1;
    len = (len>>1)+2;

    message_window = (Window){
        .x=8 - (len>>1),
        .y=3,
        .w=len,
        .h=3,
        ._draw=draw_message_window,
    };

    show_window(&message_window, THREE_SECONDS);
}

void draw_message_window(Window* w)
{
    draw_small_string(&message_buffer[0], (w->x+1)*8, (w->y+1)*8+2);
}
