#include <stdlib.h>
#include <sys/ioctl.h>
#include <include/sys_info.h>
#include <stdbool.h>
#include <curses.h>
#include <time.h>

struct game_state
{
    int term_height;
    int term_width;
    int snake_length;
    int eaten;
    int berries_available;
    int max_berries;
    struct snake_body* head;
    struct snake_body* tail;
    char** game_board;
};

struct snake_body
{
    int vertical_cord;
    int horizontal_cord;
    int direction;
    struct snake_body* next;
};


struct game_state* game_setup()
{
    // initilize and populate structs
    struct winsize ws = get_terminal_dimensions();
    struct game_state* gs = (struct game_state*)malloc(sizeof(struct game_state));


    gs->term_width = ws.ws_col;
    gs->term_height = ws.ws_row;
    gs->snake_length = 1;
    gs->eaten = 0;
    gs->berries_available = 0;
    gs->max_berries = gs->term_width/5 * gs->term_height/5;
    gs->head = (struct snake_body*)malloc(sizeof(struct snake_body));
    gs->tail = gs->head;
    gs->head->horizontal_cord = (gs->term_width-1)/2;
    gs->head->vertical_cord = (gs->term_height-1)/2;
    gs->head->direction = KEY_LEFT;
    gs->head->next = NULL;
    gs->game_board = (char**)malloc(sizeof(char*)*gs->term_height);

    // 2d pointer array for game board
    for (int i = 0; i < gs->term_height; i++)
    {
        gs->game_board[i] = (char*)malloc(sizeof(char)*gs->term_width);
    }

    // create board border using #, head of snake using @, and all spaces using the space char (0x20)
    for (int i = 0; i < gs->term_height; i++)
    {
        for (int j = 0; j < gs->term_width; j++)
        {
            if (i == 0 || i == gs->term_height-1 || j == 0 || j == gs->term_width-1)
            {
                gs->game_board[i][j] = '#';
            }
            else if (i == (gs->term_height-1)/2 && j == (gs->term_width-1)/2)
            {
                gs->game_board[i][j] = '@';
            }
            else 
            {
                gs->game_board[i][j] = ' ';
            }
        }
    }

    return gs;
}

void clear_snake_from_board(struct game_state* gs)
{
    for (int i = 1; i < gs->term_height-1; i++)
    {
        for (int j = 1; j < gs->term_width-1; j++)
        {
            if (gs->game_board[i][j] != '*') gs->game_board[i][j] = ' ';
        }
    }
}

void game_render(struct game_state* gs)
{
    system("clear"); // clear everything from screen

    for (int i = 0; i < gs->term_height; i++)
    {
        for (int j = 0; j < gs->term_width; j++)
        {
            putchar(gs->game_board[i][j]); // write char to stdout one by one in gs->game_board
        }
    }
    fflush(stdout); // No idea why, but this is necessary
}

bool detect_collision(struct game_state* gs)
{
    // returning true if snake is detected hitting boarder
    if (gs->head->vertical_cord == 0 || gs->head->vertical_cord == gs->term_height-1 || gs->head->horizontal_cord == 0 || gs->head->horizontal_cord == gs->term_width-1) return true;

    // go through each part of the body to see if the snake head has interacted with any of it's body
    struct snake_body* tmp_ptr = gs->head->next;

    for (int i = 1; i < gs->snake_length; i++)
    {
        if (tmp_ptr->horizontal_cord == gs->head->horizontal_cord && tmp_ptr->vertical_cord == gs->head->vertical_cord) return true;
        tmp_ptr = tmp_ptr->next;
    }
    return false;
}


bool draw_snake(struct game_state* gs, int* key)
{
    struct snake_body* current_snake_ptr = gs->head;
    int current_horizontal_cord, current_vertical_cord;


    for (int i=0; i < gs->snake_length; i++)
    {
        if (i==0)
        {
            // move head in direction of key press, make sure to add a body part if there is a berry within that direction
            current_horizontal_cord = current_snake_ptr->horizontal_cord;
            current_vertical_cord = current_snake_ptr->vertical_cord;

            switch(*key)
            {
                
                case KEY_UP: current_snake_ptr->vertical_cord--;
                break;
                case KEY_DOWN: current_snake_ptr->vertical_cord++;
                break;
                case KEY_LEFT: current_snake_ptr->horizontal_cord--;
                break;
                case KEY_RIGHT: current_snake_ptr->horizontal_cord++;
                break;
            }

            if (detect_collision(gs)) return true;

            // If berry in spot of overtake, add body part
            if (gs->game_board[current_snake_ptr->vertical_cord][current_snake_ptr->horizontal_cord] == '*')
            {
                gs->eaten++;
                gs->berries_available--;
                gs->snake_length++;
                struct snake_body* tail = gs->tail;
                struct snake_body* new_part = (struct snake_body*)malloc(sizeof(struct snake_body));
                new_part->next = NULL;
                tail->next = new_part;
                gs->tail = new_part;
            }

            gs->game_board[current_snake_ptr->vertical_cord][current_snake_ptr->horizontal_cord] = '@';
        }
        else
        {
            // otherwise replace the snake's current body part with that of it's predicesor 
            int tmp_current_horizontal_cord = current_snake_ptr->horizontal_cord;
            int tmp_current_vertical_cord =  current_snake_ptr->vertical_cord;

            current_snake_ptr->horizontal_cord = current_horizontal_cord;
            current_snake_ptr->vertical_cord = current_vertical_cord;

            current_horizontal_cord = tmp_current_horizontal_cord;
            current_vertical_cord = tmp_current_vertical_cord;

            gs->game_board[current_snake_ptr->vertical_cord][current_snake_ptr->horizontal_cord] = 'o';
        }

        current_snake_ptr = current_snake_ptr->next;
    }
    return false;
}

void draw_berries(struct game_state* gs)
{
    // given the amount of allowed berries to be added, add berries in indices that are only space chars
    if (gs->berries_available < gs->max_berries )
    {
        for (int i = gs->berries_available; i < gs->max_berries ; i++)
        {
            bool successful_berry = false;
            while (!successful_berry)
            {
                int random_vertical_cord = rand()%(gs->term_height - 2) + 1;
                int random_horizontal_cord = rand()%(gs->term_width - 2) + 1;

                if (gs->game_board[random_vertical_cord][random_horizontal_cord] == ' ')
                {
                    gs->game_board[random_vertical_cord][random_horizontal_cord] = '*';
                    successful_berry = true;
                    gs->berries_available++;
                }
            }
        }
    }
}

bool game_decisions(struct game_state* gs, int* key)
{
    clear_snake_from_board(gs);
    if (draw_snake(gs, key)) return false; // if there is a collistion detected at the snake's head, the game is over
    draw_berries(gs);

    return true;
}
