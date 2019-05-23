#ifndef OVERGROUND_H
#define OVERGROUND_H

#include "GLYPH.h"
#include "roguelike-engine.h"
//#include "ASCII.h"

#include "tiles.h"

static const __flash Map OVERGROUND = {
    .cols = 16,
    .rows = 8,
    .tileset = &TILES[0],
    .tiles = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,
      0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   4,   5,   0,   1,   2,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,   7,   0,   0,   0,   0,
      0,   1,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   2,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,
      0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    }
};

void init_overground( void );
void update_overground( void );
void draw_overground( void );
void fade_to_black( void );
void wait_for_fade( void );

#endif
