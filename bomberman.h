#ifndef H_BOMBERMAN
#define H_BOMBERMAN
#include <pthread.h>

struct Map;

struct Bomberman
{
    int alive;
    int x;
    int y;
    int id;
    int direction;
    int shouldBomb;
    int showGrave;
    int bombCount;
    int bombPower;
    int lastd;
    struct Map* map;
    
    pthread_t thread;
    
    /*ai*/
    int isAi;
    int tickAi;
    int lastDirection;
};

void Bomberman_init(struct Bomberman* self, int x, int y, int alive, int i, struct Map* map, int isAi);
void Bomberman_free(struct Bomberman* self);
void Bomberman_draw(struct Bomberman* self);
void Bomberman_drawGrave(struct Bomberman* self);
void Bomberman_handleKey(struct Bomberman* self, int c);

#endif
