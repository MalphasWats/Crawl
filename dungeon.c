#include "dungeon.h"

#include <stdio.h>

#include "tiles.h"
#include "ui.h"

uint8_t level;
char message_buffer[7];
Window message;

void init_dungeon( void )
{
    display_mode(NORMAL);

    level = 1;
    generate_dungeon();

    _update = update_dungeon;
    _draw = draw_dungeon;
    _update_return = _update;
    _draw_return = _draw;

    sprintf(message_buffer, "LEVEL %d", level);

    message = (Window){
        .x=3,
        .y=3,
        .w=10,
        .h=3,

        .actions=TIMED,

        .timer=t + 3000,

        .content=&message_buffer[0],
        ._callback=0,
    };

    add_window(&message);
}

void update_dungeon( void )
{
    update_engine();

    // Check for exit here...

    CollideType c = check_move();
    if (c == MAP)
    {
        // Check for doors/chests etc.
        Tile tile = get_tile_at(collide_x, collide_y);
        if (tile.flags & FLAG_CHEST)
        {
            click();
            sprintf(message_buffer, "CHEST");
            message.timer = millis()+2500;

            map->tiles[collide_y*map->cols+collide_x] += 1;
        }
    }
    else if (c == MOB)
    {
        // Attack mob
        click();

        hit_mob(&player, collide_mob);
    }

    // Update Mobs?
}

void draw_dungeon( void )
{
    clear_buffer();
    draw_map();

    for (uint8_t m=0 ; m<MAX_MOBS ; m++)
    {
        if (mobs[m].alive)
        {
            draw_mob(&mobs[m]);
        }
    }

    draw_mob(&player);
}


void generate_dungeon( void )
{
    // Copy test map
    map->tileset = &TILES[0];
    map->cols = 32;
    map->rows = 16;
    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            if (TEST_DUNGEON.tiles[y*map->cols+x] == 16) // Test monster
            {
                map->tiles[y*map->cols+x] = 3;
                spawn_mob(x, y, MOB_BLOB);
            }
            else
            {
                map->tiles[y*map->cols+x] = TEST_DUNGEON.tiles[y*map->cols+x];
            }
        }
    }

    player.x=3;
    player.y=2;
}

void spawn_mob(uint8_t x, uint8_t y, uint8_t mob)
{
    //TODO: use mob var to spawn different types
    for (uint8_t m=0 ; m<MAX_MOBS ; m++)
    {
        if (!mobs[m].alive)
        {
            mobs[m] = (Mob){
                .x=x,
                .y=y,
                .offset_x=0,
                .offset_y=0,
                .tileset=&BLOB[0],
                .flipped=FALSE,
                .alive=TRUE,
                .type=mob,

                .hp=1,
                .max_hp=1,
                .damage=1,
                .defence=0,
            };
            break;
        }
    }
}

void hit_mob(Mob* attacker, Mob* defender)
{
    //TODO: deal with defence
    defender->hp -= attacker->damage;

    add_floater((Floater){
        .x=defender->x*8,
        .y=defender->y*8 - 4,
        .counter=0,
        .timer=t+FLOATER_DELAY,
        .tileset=&DAMAGE_FLOATERS[0],
        .value=attacker->damage,
    });
}
