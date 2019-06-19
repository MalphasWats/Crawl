#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "GLYPH.h"
#include "roguelike-engine.h"

#define FADE_RATE 160

#define FADE_TO_BLACK 0

void set_transition(uint8_t transition, void (*return_to)( void ));
void fade_to_black( void );
void wait_for_fade( void );

#endif
