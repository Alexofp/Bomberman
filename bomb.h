#ifndef H_BOMB
#define H_BOMB

struct Bomberman;
struct Map;

struct Bomb
{
    int alive;
    int x,y;
    int time;
    int power;
    int fakebomb;
    struct Map* map;
    struct Bomberman* bomberman;
};

void Bomb_init(struct Bomb* self, struct Map* map);
void Bomb_create(struct Bomb* self, int x, int y, struct Bomberman* b);
void Bomb_update(struct Bomb* self);
void Bomb_draw(struct Bomb* self);
void Bomb_explode(struct Bomb* self);
int Bomb_willAffect(struct Bomb* self, int x, int y);

#endif
