#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>
#include <sys/types.h>
#include <pthread.h>
#include <include/timing.h>
#include <include/game.h>

void* thread(void* key)
{
    while (true)
    {
        *(int*)key = getch();
    }
    return NULL;
}

int main()
{
    printf("\e[?25l"); // Get rid of Cursor
    srand(time(NULL));   // Initialization, should only be called once.
    initscr(); // init for curses
    intrflush(stdscr, false); // arrow key setup
    keypad(stdscr, true); // more arrow key setup

    pthread_t tid; //thread
    int* key = (int*)malloc(sizeof(int)); // point in memory where last key is detected
    *key = KEY_LEFT; // CHOOSE STARTING DIRECTION HERE, KEEP NULL IF NO STARTING DIRECTION

    if (pthread_create(&tid, NULL, thread, key) != 0)
    {
        perror("pthread_create() error");
        exit(1);
    }

    struct game_state* gs = game_setup(); 
    

    while (game_decisions(gs, key))
    {
        game_render(gs);
        delay(150);
    }

    if (pthread_cancel(tid) != 0)
    {
        perror("pthread_cancel error");
        exit(1);
    }

    system("clear");
    printf("SCORE: \e[0;32m%d\e[0m\n", gs->eaten);

	return 0;
}