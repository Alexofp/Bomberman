#include "console.h"

struct Console* Console_get()
{
    static struct Console console;
    return &console;
}

void Console_init()
{
    struct Console* self = Console_get();
    clear();
    int x;
    int y;
    for(y=0;y<WINDOW_HEIGHT;y++)
    {
        for(x=0;x<WINDOW_WIDTH;x++)
        {
            self->consoleBuffer[y*WINDOW_WIDTH+x] = L' ';
            self->consoleBufferSaved[y*WINDOW_WIDTH+x] = L' ';
        }
    }
}

void Console_clear()
{
    struct Console* self = Console_get();
    int x;
    int y;
    for(y=0;y<WINDOW_HEIGHT;y++)
    {
        for(x=0;x<WINDOW_WIDTH;x++)
        {
            self->consoleBuffer[y*WINDOW_WIDTH+x] = L' ';
        }
    }
}

void Console_draw()
{
    struct Console* self = Console_get();
    int x;
    int y;
    for(y=0;y<WINDOW_HEIGHT;y++)
    {
        for(x=0;x<WINDOW_WIDTH;x++)
        {
            if(self->consoleBuffer[y*WINDOW_WIDTH+x]!=self->consoleBufferSaved[y*WINDOW_WIDTH+x])
            {
                self->consoleBufferSaved[y*WINDOW_WIDTH+x] = self->consoleBuffer[y*WINDOW_WIDTH+x];
                move(y,x);
                wchar_t str[2];
                str[0] = self->consoleBufferSaved[y*WINDOW_WIDTH+x];
                str[1] = L'\0';
                addwstr(str);
            }
        }
    }
}

void Console_drawChar(int x,int y,wchar_t c)
{
    struct Console* self = Console_get();
    if(x>=WINDOW_WIDTH || y>=WINDOW_HEIGHT || x<0 || y<0)
    {
        return;
    }
    self->consoleBuffer[y*WINDOW_WIDTH+x] = c;
}

void Console_drawStr(int x,int y,wchar_t* str)
{
    struct Console* self = Console_get();
    int len = wcslen(str);
    int i;
    for(i=0;i<len;i++)
    {
        Console_drawChar(x+i,y,str[i]);
    }
}
