#ifndef H_SPRITE
#define H_SPRITE
#include <wchar.h>

struct Sprite
{
    int w;
    int h;
    wchar_t* data;
};

struct Sprite* Sprite_init(int w, int h);
struct Sprite* Sprite_fromFile(char* path);
void Sprite_free(struct Sprite* self);

void Sprite_set(struct Sprite* self, int x, int y, wchar_t c);
wchar_t Sprite_get(struct Sprite* self, int x, int y);
void Sprite_draw(struct Sprite* self, int x, int y);

#endif
