#ifndef TILES_H
#define TILES_H

#include "GLYPH.h"
#include "roguelike-engine.h"

static const __flash Tile TILES[] = {
    { .data={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,}, .flags=0b00000000, },  // Empty
    { .data={0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00,}, .flags=0b00000000, },  // Grass 1
    { .data={0x00, 0x60, 0x00, 0x60, 0x06, 0x00, 0x06, 0x00,}, .flags=0b00000000, },  // Grass 2
    { .data={0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,}, .flags=0b00000000, },  // Floor
    { .data={0x00, 0x80, 0x60, 0x58, 0xc4, 0x44, 0x74, 0x4c,}, .flags=0b10000000, },  // Cave TL
    { .data={0x4a, 0x2a, 0xd2, 0x14, 0x22, 0xa2, 0x4c, 0xb0,}, .flags=0b10000000, },  // Cave TR
    { .data={0xe7, 0x98, 0x66, 0xa1, 0xc2, 0xac, 0x92, 0x02,}, .flags=0b10000000, },  // Cave BL
    { .data={0xe5, 0xf9, 0xfc, 0xfd, 0xf9, 0xe1, 0x0e, 0xf1,}, .flags=0b00000001, },  // Cave BR (Door)
    { .data={0x87, 0xa1, 0xa8, 0xaa, 0xaa, 0xa8, 0xa1, 0x87,}, .flags=0b00000000, },  // Stairs Up
    { .data={0xe1, 0x85, 0x15, 0x55, 0x55, 0x15, 0x85, 0xe1,}, .flags=0b00000001, },  // Stairs Down
    { .data={0xfe, 0x03, 0xfd, 0xfd, 0xfd, 0xfd, 0x03, 0xfe,}, .flags=0b11100000, },  // Door Closed
    { .data={0xfe, 0x03, 0x01, 0x01, 0x01, 0x01, 0x03, 0xfe,}, .flags=0b00000000, },  // Door Open
    { .data={0xfe, 0xa9, 0x9b, 0xcd, 0x89, 0xdb, 0x89, 0xfe,}, .flags=0b11000000, },  // Wall Block
    { .data={0x00, 0xf8, 0x94, 0x94, 0xb4, 0x94, 0x94, 0xf8,}, .flags=0b10000010, },  // Chest Closed
    { .data={0x00, 0xfe, 0x9d, 0x9d, 0xbd, 0x9d, 0x9d, 0xfe,}, .flags=0b00000000, },  // Chest Open
    { .data={0x00, 0x38, 0x4c, 0x92, 0x92, 0x92, 0x4c, 0x38,}, .flags=0b10000100, },  // Vase 1
    { .data={0x00, 0x04, 0x22, 0x80, 0x84, 0x82, 0x48, 0x20,}, .flags=0b00000000, },  // Broken vase
    { .data={0x42, 0xdb, 0x3c, 0x74, 0x9c, 0x74, 0xdb, 0x42,}, .flags=0b00000000, },  // Skull
};

#define FLOOR 3
#define BLOCK 12
#define DOOR 10


#define SKULL_TILE 17

static const __flash Tile PLAYER[] = {
    { .data={0x00, 0x10, 0x02, 0xf5, 0xf7, 0x05, 0x12, 0x00,}, .flags=0b00000000, },
    { .data={0x00, 0x00, 0x54, 0x6a, 0x6e, 0xaa, 0x04, 0x00,}, .flags=0b00000000, },
    { .data={0x00, 0x10, 0x02, 0xf5, 0xf7, 0x05, 0x12, 0x00,}, .flags=0b00000000, },
    { .data={0x00, 0x20, 0x44, 0xea, 0x6e, 0x1a, 0x04, 0x00,}, .flags=0b00000000, },
};

static const __flash Tile BLOB[] = {
    { .data={0x00, 0x80, 0xe0, 0xb0, 0xf0, 0xb0, 0xe0, 0x80,}, .flags=0b00000000, },
    { .data={0x80, 0xc0, 0xc0, 0x60, 0xe0, 0x60, 0xc0, 0x80,}, .flags=0b00000000, },
    { .data={0x00, 0x80, 0xe0, 0xb0, 0xf0, 0xb0, 0xe0, 0x80,}, .flags=0b00000000, },
    { .data={0x00, 0x00, 0xe0, 0xd8, 0xfc, 0xd8, 0xe0, 0x00,}, .flags=0b00000000, },
};

#define MOB_PLAYER 0
#define MOB_BLOB   1









#endif
