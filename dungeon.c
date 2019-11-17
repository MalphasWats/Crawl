#include <stdlib.h>
#include "dungeon.h"

#include "tiles.h"
#include "overground.h"
#include "transitions.h"

uint8_t level=1;
Window dgn_messg, win_hp;

void init_dungeon( void )
{
    display_mode(NORMAL);

    //level = 1;

    generate_random_dungeon();

    _update = update_dungeon;
    _draw = draw_dungeon;
    _update_return = _update;
    _draw_return = _draw;

    loot_table = &LOOT_TABLE;

    //give_item(&loot_table->items[0]);
    //give_item(&loot_table->items[1]);
    //give_item(&loot_table->items[2]);

    dgn_messg = (Window){
        .x=5,
        .y=3,
        .w=6,
        .h=3,
        ._draw=wndw_level,
    };

    show_window(&dgn_messg, THREE_SECONDS);

    win_hp = (Window){
        .x=0,
        .y=0,
        .w=3,
        .h=2,
        ._draw=draw_hp,
    };

    show_window(&win_hp, FIXED);
}

void wndw_level(Window* w)
{
    draw_small_string("LEVEL ", (w->x+1)*8, (w->y+1)*8+2);
    draw_small_int(level, 0, (w->x+4)*8, (w->y+1)*8+2);
}

void draw_hp(Window* w)
{
    draw_small_int(player.hp, 0, w->x+6, w->y+6);
    draw_small_string("/", w->x+10, w->y+6);
    draw_small_int(player.max_hp, 0, w->x+14, w->y+6);
}

void update_dungeon( void )
{
    // Check for exit here...
    Tile tile = get_tile_at(player.x, player.y);
    if (tile.flags & EXIT_DOWN_FLAG)
    {
        level += 1;
        set_transition(FADE_TO_BLACK, init_dungeon);
    }

    check_player_turn();

    //place corpses.
    for (uint8_t m=0 ; m<MAX_MOBS ; m++)
    {
        if (!mobs[m].alive && mobs[m].type > 0)
        {
            if (map->tiles[mobs[m].y*map->cols+mobs[m].x] != STAIRS_DOWN)
                map->tiles[mobs[m].y*map->cols+mobs[m].x] = SKULL_TILE;
        }
    }

    // check if player dead.
    if (player.hp <=  0)
    {
        player.alive=FALSE;
        _update = update_dead;
        _draw = draw_dead;
    }
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

void generate_random_dungeon( void )
{
    map->tileset = &TILES[0];
    map->cols = 32;
    map->rows = 16;

    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            if ( rng() & 1 )
                map->tiles[y*map->cols+x] = 0;
            else
                map->tiles[y*map->cols+x] = 1;
        }
    }

    cellular_automaton();
    trim_holes();
    kill_diags();
    label_caverns();
    carve_tunnels();
    reset_labels();
    uint8_t num_caverns = label_caverns();
    if (num_caverns > 1)
        fill_small_caverns();
    reset_labels();

    place_chests();
    place_pots();
    place_mobs();

    bool player_placed = FALSE;

    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            if ( map->tiles[y*map->cols+x] == 1 )
            {
                map->tiles[y*map->cols+x] = FLOOR;
                if (!player_placed)
                {
                    player.x=x;
                    player.y=y;
                    player_placed = TRUE;

                    map->tiles[y*map->cols+x] = 8;
                }
            }
            else if (map->tiles[y*map->cols+x] == 0)
                map->tiles[y*map->cols+x] = BLOCK;
        }
    }
    uint8_t furthest_distance=0;
    uint8_t furthest_x=0;
    uint8_t furthest_y=0;
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            uint8_t d = distance(player.x, player.y, x, y);
            if (d > furthest_distance && map->tiles[y*map->cols+x] == FLOOR)
            {
                furthest_distance = d;
                furthest_x = x;
                furthest_y = y;
            }
        }
    }
    map->tiles[furthest_y*map->cols+furthest_x] = 9;

    reset_viewport();
}

void cellular_automaton( void )
{
    Map* new_map = malloc(2 + 2 + 3 + MAP_COLS_MAX*MAP_ROWS_MAX);
    new_map->tileset = map->tileset;
    new_map->rows = map->rows;
    new_map->cols = map->cols;
    uint8_t neighbours;

    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            if ( x == 0 || y == 0 || y == map->rows-1 || x == map->cols-1 )
                new_map->tiles[y*map->cols+x] = 0;
            else
            {
                neighbours = 0;

                neighbours += map->tiles[ ((y-1)*map->cols)+(x-1) ] == 0;
                neighbours += map->tiles[ ((y-1)*map->cols)+x ] == 0;
                neighbours += map->tiles[ ((y-1)*map->cols)+(x+1) ] == 0;

                neighbours += map->tiles[ (y*map->cols)+(x-1) ] == 0;
                neighbours += map->tiles[ (y*map->cols)+(x+1) ] == 0;

                neighbours += map->tiles[ ((y+1)*map->cols)+(x-1) ] == 0;
                neighbours += map->tiles[ ((y+1)*map->cols)+x ] == 0;
                neighbours += map->tiles[ ((y+1)*map->cols)+(x+1) ] == 0;

                if ( map->tiles[ (y*map->cols)+x ] == 0 )
                {
                    if ( neighbours >= 3 && neighbours <=8 )
                        new_map->tiles[(y*map->cols)+x] = 0;
                    else
                        new_map->tiles[(y*map->cols)+x] = 1;
                }
                else
                {
                    if ( neighbours == 6 ||
                         neighbours == 7 ||
                         neighbours == 8 )
                        new_map->tiles[(y*map->cols)+x] = 0;
                    else
                        new_map->tiles[(y*map->cols)+x] = 1;
                }
            }
        }
    }
    free(map);
    map = new_map;
}

void trim_holes( void )
{
    uint8_t exits;
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            exits = 0;
            for (uint8_t d=0 ; d<4 ; d++)
            {
                if ( map->tiles[ ( (y+DIRY[d])*map->cols)+(x+DIRX[d]) ] > 0 )
                    exits += 1;
            }
            if (exits == 0 && map->tiles[ (y*map->cols)+x ] > 0)
                map->tiles[ (y*map->cols)+x ] = 0;
            else if (exits == 3 && map->tiles[ (y*map->cols)+x ] == 0)
                map->tiles[ (y*map->cols)+x ] = 1;
        }
    }
}

void kill_diags( void )
{
    uint8_t pattern;
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            pattern = 0;
            pattern |= map->tiles[ (y*map->cols)+x ] << 0;
            pattern |= map->tiles[ (y*map->cols)+(x+1) ] << 1;
            pattern |= map->tiles[ ((y+1)*map->cols)+x ] << 2;
            pattern |= map->tiles[ ((y+1)*map->cols)+(x+1) ] << 3;

            if (pattern == 0b0110 || pattern == 0b1001)
            {
                map->tiles[ (y*map->cols)+x ] = 1;
                map->tiles[ (y*map->cols)+(x+1) ] = 1;
                map->tiles[ ((y+1)*map->cols)+x ] = 1;
                map->tiles[ ((y+1)*map->cols)+(x+1) ] =1;
            }
        }
    }
}

uint8_t label_caverns( void )
{
    reset_labels();
    uint8_t flag = 2;
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            if (map->tiles[ (y*map->cols)+x ] == 1)
            {
                fill_cavern(x, y, 1, flag);
                flag += 1;
            }
        }
    }
    return flag-2;
}

void reset_labels( void )
{
    for (uint8_t y=0 ; y<map->rows ; y++)
    {
        for (uint8_t x=0 ; x<map->cols ; x++)
        {
            if ( map->tiles[y*map->cols+x] > 0)
            {
                map->tiles[y*map->cols+x] = 1;
            }
        }
    }
}

void fill_cavern( uint8_t x, uint8_t y, uint8_t old_flag, uint8_t new_flag )
{
    if (map->tiles[ (y*map->cols)+x ] == old_flag)
    {
        map->tiles[ (y*map->cols)+x ] = new_flag;
        for (uint8_t d=0 ; d<4 ; d++)
        {
            fill_cavern(x+DIRX[d], y+DIRY[d], old_flag, new_flag);
        }
    }
}

//TODO: maybe make doors
void carve_tunnels( void )
{
    uint8_t pattern;
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-3 ; x++)
        {
            pattern = 0;
            for (uint8_t r=0 ; r<4 ; r++)
            {
                if (map->tiles[ (y*map->cols)+(x+r) ])
                    pattern |= 1 << r;
            }

            if (pattern == 0b1001 || pattern == 0b1101 || pattern == 0b1011)
            {
                uint8_t old_flag = map->tiles[ (y*map->cols)+(x+3) ];
                uint8_t new_flag = map->tiles[ (y*map->cols)+x ];
                if (new_flag != old_flag)
                {
                    map->tiles[ (y*map->cols)+(x+1) ] = new_flag;
                    map->tiles[ (y*map->cols)+(x+2) ] = new_flag;

                    fill_cavern(x+3, y, old_flag, new_flag);
                }
            }
        }

    }

    for (uint8_t y=1 ; y<map->rows-3 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            pattern = 0;
            for (uint8_t c=0 ; c<4 ; c++)
            {
                if (map->tiles[ ((y+c)*map->cols)+x ])
                    pattern |= 1 << c;
            }

            if (pattern == 0b1001 || pattern == 0b1101 || pattern == 0b1011)
            {
                uint8_t old_flag = map->tiles[ ((y+3)*map->cols)+x ];
                uint8_t new_flag = map->tiles[ (y*map->cols)+x ];
                if (new_flag != old_flag)
                {
                    map->tiles[ ((y+1)*map->cols)+x ] = new_flag;
                    map->tiles[ ((y+2)*map->cols)+x ] = new_flag;

                    fill_cavern(x, y+3, old_flag, new_flag);
                }
            }
        }

    }
}

void fill_small_caverns( void )
{
    uint8_t sizes[6] = {0, 0, 0, 0, 0, 0};
    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            if (map->tiles[ (y*map->cols)+x ] > 0)
                sizes[ map->tiles[ (y*map->cols)+x ]-1 ] += 1;
        }
    }
    uint8_t biggest = 0;
    uint8_t flag = 1;
    for (uint8_t i=0 ; i<6 ; i++)
    {
        if (sizes[i] > biggest)
        {
            biggest = sizes[i];
            flag = i+1;
        }
    }

    for (uint8_t y=1 ; y<map->rows-1 ; y++)
    {
        for (uint8_t x=1 ; x<map->cols-1 ; x++)
        {
            if (map->tiles[ (y*map->cols)+x ] > 0 &&
                map->tiles[ (y*map->cols)+x ] != flag)
            {
                map->tiles[ (y*map->cols)+x ] = 0;
            }
        }
    }
}


void generate_test_dungeon( void )
{
    // Copy test map
    map->tileset = TEST_DUNGEON.tileset;
    map->cols = TEST_DUNGEON.cols;
    map->rows = TEST_DUNGEON.rows;
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

    player.x=2;
    player.y=1;

    reset_viewport();
}

//TODO: it is probably possible to generate a level where chests can't be placed like this.
void place_chests( void )
{
    uint8_t pattern;
    uint8_t chests_placed = 0;
    uint16_t i, x, y;
    while(chests_placed < 3)
    {
        for (i=0 ; i<map->rows*map->cols ; i=i+rng() % (map->rows*map->cols))
        {
            pattern = 0;
            x = i%map->cols;
            y = i/map->cols;
            if (x>0 && x<map->cols-1 && y>0 && y<map->rows-1)
            {
                for (uint8_t d=0 ; d<8 ; d++)
                {
                    if ( map->tiles[ ( (y+DIRY[d])*map->cols)+(x+DIRX[d]) ] > 0 )
                        pattern |= 1<<d;
                }
                if (pattern == 0b11111111 && map->tiles[i] == 1 && chests_placed < 3)
                {
                    chests_placed += 1;
                    map->tiles[ i ] = CHEST;
                    break;
                }
            }
        }
    }
}

void place_pots( void )
{
    uint8_t pattern;
    uint8_t pots_placed = 0;
    uint16_t i, x, y;
    while(pots_placed < 13)
    {
        for (i=rng() % (map->rows*map->cols) ; i<map->rows*map->cols ; i++)
        {
            pattern = 0;
            x = i%map->cols;
            y = i/map->cols;
            if (x>0 && x<map->cols-1 && y>0 && y<map->rows-1)
            {
                for (uint8_t d=0 ; d<8 ; d++)
                {
                    if ( map->tiles[ ( (y+DIRY[d])*map->cols)+(x+DIRX[d]) ] > 0 )
                        pattern |= 1<<d;
                }
                if ( (pattern == 0b01101110 ||
                      pattern == 0b10011011 ||
                      pattern == 0b11001101 ||
                      pattern == 0b00110111 ||
                      pattern == 0b00100110 ||
                      pattern == 0b11000100 ) &&
                      map->tiles[i] == 1)
                {
                    pots_placed += 1;
                    map->tiles[ i ] = VASE;
                    if (pattern==0b00100110 || pattern==0b11001000)
                        break;
                }
            }
        }
    }
}

void place_mobs( void )
{
    uint8_t pattern;
    uint8_t mobs_placed = 0;
    uint16_t i, x, y;
    while(mobs_placed < 3+level)
    {
        for (i=0 ; i<map->rows*map->cols ; i=i+rng() % (map->rows*map->cols))
        {
            pattern = 0;
            x = i%map->cols;
            y = i/map->cols;
            if (x>0 && x<map->cols-1 && y>0 && y<map->rows-1)
            {
                for (uint8_t d=0 ; d<8 ; d++)
                {
                    if ( map->tiles[ ( (y+DIRY[d])*map->cols)+(x+DIRX[d]) ] > 0 )
                        pattern |= 1<<d;
                }
                if (pattern == 0b11111111 && map->tiles[i] == 1)
                {
                    mobs_placed += 1;
                    spawn_mob(x, y, MOB_BLOB); //TODO: choose monsters
                    break;
                }
            }
        }
    }
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

                .hp=2,
                .max_hp=2,
                .damage=1,
                .defence=0,

                .aggro=FALSE,
            };
            break;
        }
    }
}

void update_dead( void )
{
    uint8_t buttons;
    buttons = read_buttons();

    //TODO: Debouncing
    if(buttons == BTN_A)
    {
        init_overground();
    }
}

void draw_dead( void )
{
    clear_buffer();
    draw_small_string("You Died", 6*8, 3*8+2);
    draw_battery();
}
