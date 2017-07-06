#ifndef H_SPRITES
#define H_SPRITES

enum SPRITES { S_BOMBERMAN, S_WALL, S_FLOOR, S_BRICKS, S_BOMB3, S_BOMB2, S_BOMB1, S_FIRE, S_GRAVE, S_BONUS1, S_BONUS2, S_LOGO, S_COPYRIGHT, S_MENUBOMB, S_MENUBOMB2 };

struct Sprite;

struct Sprites
{
    struct Sprite* sprites[16];
};

struct Sprites* Sprites_getObject();

void Sprites_load(enum SPRITES name, char* path);

void Sprites_init();

void Sprites_free();

struct Sprite* Sprites_get(enum SPRITES name);

#endif
