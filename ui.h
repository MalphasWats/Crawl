#ifndef UI_H
#define UI_H

#include "GLYPH.h"

#define MAX_WINDOWS 5

static const __flash uint8_t WINDOW_TILES[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Empty
    0x00, 0xf8, 0x04, 0x02, 0x02, 0x02, 0x82, 0x42,  // Top-left
    0x42, 0x82, 0x02, 0x02, 0x02, 0x04, 0xf8, 0x00,  // Top-right
    0x00, 0x1f, 0x20, 0x40, 0x40, 0x40, 0x41, 0x42,  // Bottom-left
    0x42, 0x41, 0x40, 0x40, 0x40, 0x20, 0x1f, 0x00,  // bottom-right
    0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,  // Left-right
    0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,  // Top-bottom
};

#define WIN_BL 0*8
#define WIN_TL 1*8
#define WIN_TR 2*8
#define WIN_BL 3*8
#define WIN_BR 4*8
#define WIN_LR 5*8
#define WIN_TB 6*8

typedef enum { FIXED, TIMED, DISMISS, CHOOSE } WindowActions;

typedef struct Window {
    int8_t x;
    int8_t y;

    int8_t w;
    int8_t h;

    WindowActions t;

    uint32_t timer;

    char* content;

    void (*_callback)( uint8_t i );
} Window;

bool add_window(Window w);

Window windows[MAX_WINDOWS];
Window* focus_window;

#endif
