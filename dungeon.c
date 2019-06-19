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

    generate_dungeon();

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


void generate_dungeon( void )
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
