#include "sprite.h"
#include <stdlib.h>
#include <stdio.h>
#include "console.h"
#define TRANSPARENT_CHARACTER L'~'

struct Sprite* Sprite_init(int w, int h)
{
    struct Sprite* self = (struct Sprite*)malloc(sizeof(struct Sprite));
    self->w = w;
    self->h = h;

    self->data = malloc(sizeof(wchar_t)*w*h);
    int i;    
    for(i = 0;i<w*h;i++)
    {
        self->data[i] = TRANSPARENT_CHARACTER;
    }
    return self;
}

void Sprite_free(struct Sprite* self)
{
    free(self->data);
    free(self);
}

struct Sprite* Sprite_fromFile(char* path)
{
    FILE *fp = fopen(path, "r");
    if(fp==0)
    {
        return 0;
    }
    int w,h;
    fscanf(fp, "%d %d", &w, &h);
    
    struct Sprite* spr = Sprite_init(w,h);   

    int i = 0;
    
    while(i<w*h)
    {
        wchar_t c;
        fscanf(fp, "%lc", &c);
        if(c!=L'\n')
        {
            spr->data[i] = c;
            i = i + 1;
        }
    }
    fclose(fp);
    return spr;
}

void Sprite_set(struct Sprite* self, int x, int y, wchar_t c)
{
    self->data[y*self->w+x] = c;
}

wchar_t Sprite_get(struct Sprite* self, int x, int y)
{
    return self->data[y*self->w+x];
}

void Sprite_draw(struct Sprite* self, int x, int y)
{
    if(self==0)
        return;
    int xx;
    int yy;
    for(yy=0;yy<self->h;yy++)
    {
        for(xx=0;xx<self->w;xx++)
        {
            Console_drawChar(xx+x,yy+y,Sprite_get(self,xx,yy));
        }
    }
}
