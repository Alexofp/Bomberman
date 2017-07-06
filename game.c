#include "game.h"

void Game_run(int playerCount)
{
    struct Map map;
    Map_init(&map, 15, 13, playerCount);
    
    int timeBeforeQuit = 200;
    int done = 0;
    while(!done){
        Console_clear();

        int key = getch();
        if(key!=ERR)
        {
            LOG("You pressed %d",key);
            Map_handleKey(&map, key);
            if(key==27)
            {
                done = 1;
            }
        }
        Map_update(&map);
        
        int count = 0;
        int i;
        for(i=0;i<4;i++)
        {
            if(map.bombermans[i].alive)
            {
                count = count + 1;
            }
        }
        if(count<=1)
        {
            timeBeforeQuit = timeBeforeQuit - 1;
            if(timeBeforeQuit<=0)
                done = 1;
        }
        
        Map_draw(&map);

        Console_draw();
        usleep(1000000/30);
    }

    Map_free(&map);
}
