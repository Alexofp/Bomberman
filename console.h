#ifndef H_CONSOLE
#define H_CONSOLE

#include <ncurses.h>
#include <unistd.h>
#include <wchar.h>

#define WINDOW_WIDTH 120
#define WINDOW_HEIGHT 52

struct Console
{
    wchar_t consoleBuffer[WINDOW_WIDTH*WINDOW_HEIGHT];
    wchar_t consoleBufferSaved[WINDOW_WIDTH*WINDOW_HEIGHT];
};

struct Console* Console_get();
void Console_init();
void Console_clear();
void Console_draw();
void Console_drawChar(int x,int y,wchar_t c);
void Console_drawStr(int x,int y,wchar_t* str);

#endif
