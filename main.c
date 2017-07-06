#include <ncurses.h>
#include <unistd.h>
#include "console.h"
#include <locale.h>
#include "sprite.h"
#include "logger.h"
#include "sprites.h"
#include "map.h"
#include <time.h>
#include "bomberman.h"
#include "config.h"
#include "menu.h"
//sudo apt-get install libncurses5-dev
//sudo apt-get install libncursesw5-dev

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "");
    initscr();
	if (COLS < 60 || LINES < 26) {
        endwin();
		printf("Window is too small\n");
        return -1;
	}


    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr,TRUE);	
    curs_set(0);
    scrollok(stdscr, FALSE);
    
    Logger_init();
    Console_init();

    Sprites_init();
    
    Config_init();
    Config_loadFile("config");
    
    Menu_run();
    
    Sprites_free();
	endwin();
    return 0;
}
