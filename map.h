#ifndef H_MAP
#define H_MAP
#include "bomb.h"
#include "semaphore.h"
#include "bomberman.h"

#define BOMB_COUNT 16

struct MapItem
{
    int type;
    int burnTime;
    int hasPlayer;
    int hasBomb;
    int hasBonus;
    int blockBombs;
};

struct Map
{
    int w;
    int h;
    struct MapItem* map;
    struct Bomb bombs[BOMB_COUNT];
    struct Semaphore* bombsSem[BOMB_COUNT];
    struct Semaphore* mapSem;
    struct Bomberman bombermans[4];
};

void Map_init(struct Map* self, int w,int h, int playerCount);
void Map_draw(struct Map* self);
void Map_update(struct Map* self);
void Map_handleKey(struct Map* self, int key);
int Map_hasWall(struct Map* self, int x, int y);
int Map_hasSomething(struct Map* self, int x, int y);
int Map_wallType(struct Map* self, int x, int y);
int Map_createBomb(struct Map* self, int x,int y, struct Bomberman* b);
struct MapItem* Map_get(struct Map* self, int x,int y);
//ai
int Map_safeDirection(struct Map* self, int x,int y);
int Map_inDanger(struct Map* self, int x,int y);
int Map_safeDistIfBomb(struct Map* self, int px,int py, struct Bomberman* b);
int Map_dirToNearestEnemy(struct Map* self, struct Bomberman* b);

#endif
