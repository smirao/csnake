#ifndef GAME_INFO_H_   
#define GAME_INFO_H_

struct game_state
{
    int term_height;
    int term_width;
    int snake_length;
    int eaten;
    int berries_available;
    struct snake_body* head;
    char** game_board;
};

struct snake_body
{
    int vertical_cord;
    int horizontal_cord;
    int direction;
    struct snake_body* next;
};

struct game_state* game_setup();
void game_render(struct game_state* gs);
bool game_decisions(struct game_state* gs, int* key);

#endif 