#include "sprites.h"
#include "sprite.h"


struct Sprites* Sprites_getObject()
{
    static struct Sprites s;
    return &s;
}

void Sprites_load(enum SPRITES name, char* path)
{
    struct Sprites* self = Sprites_getObject();

    self->sprites[(int)name] = Sprite_fromFile(path);
}

void Sprites_init()
{
    struct Sprites* self = Sprites_getObject();
    int i;
    for(i=0;i<16;i++)
    {
        self->sprites[i] = 0;
    }

    Sprites_load(S_BOMBERMAN,"sprites/bomberman");
    Sprites_load(S_WALL,"sprites/wall");
    Sprites_load(S_FLOOR,"sprites/floor");
    Sprites_load(S_BRICKS,"sprites/bricks");
    Sprites_load(S_BOMB3,"sprites/bomb3");
    Sprites_load(S_BOMB2,"sprites/bomb2");
    Sprites_load(S_BOMB1,"sprites/bomb1");
    Sprites_load(S_FIRE,"sprites/fire");
    Sprites_load(S_GRAVE,"sprites/grave");
    Sprites_load(S_BONUS1,"sprites/bonus1");
    Sprites_load(S_BONUS2,"sprites/bonus2");
    Sprites_load(S_LOGO,"sprites/logo");
    Sprites_load(S_COPYRIGHT,"sprites/copyright");
    Sprites_load(S_MENUBOMB,"sprites/menubomb");
    Sprites_load(S_MENUBOMB2,"sprites/menubomb2");
}

void Sprites_free()
{
    struct Sprites* self = Sprites_getObject();
    int i;
    for(i=0;i<16;i++)
    {
        if(self->sprites[i])
        {
            Sprite_free(self->sprites[i]);
        }
    }
}

struct Sprite* Sprites_get(enum SPRITES name)
{
    struct Sprites* self = Sprites_getObject();

    return self->sprites[(int)name];
}
