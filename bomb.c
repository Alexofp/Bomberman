#include "bomb.h"
#include "sprites.h"
#include "map.h"
#include "bomberman.h"
#include "config.h"

void Bomb_init(struct Bomb* self, struct Map* map)
{
    self->alive = 0;
    self->map = map;
}
void Bomb_create(struct Bomb* self, int x, int y, struct Bomberman* b)
{
    self->alive = 1;
    self->x = x;
    self->y = y;
    self->time = Config_get()->bombTimer;
    self->bomberman = b;
    self->fakebomb = 0;
    
    Map_get(self->map,self->x,self->y)->hasBomb = 1;
}
void Bomb_update(struct Bomb* self)
{
    if(self->alive)
    {
        self->time = self->time - 1;
        
        if(self->time <= 0 || Map_get(self->map,self->x,self->y)->burnTime>0 || self->fakebomb)
        {
            Bomb_explode(self);
        }
    }
}
void Bomb_draw(struct Bomb* self)
{
    if(self->alive)
    {
        if(self->time > (Config_get()->bombTimer*2/3))
        {
            Sprite_draw(Sprites_get(S_BOMB3),self->x*4,self->y*2);
        }else
        if(self->time > (Config_get()->bombTimer/3))
        {
            Sprite_draw(Sprites_get(S_BOMB2),self->x*4,self->y*2);
        }else
        {
            Sprite_draw(Sprites_get(S_BOMB1),self->x*4,self->y*2);
        }
    }
}

void Bomb_wave(struct Bomb* self, int dx, int dy)
{
    int power = self->bomberman->bombPower;
    int d = 1;
    Map_get(self->map,self->x,self->y)->burnTime = Config_get()->bombFireCenterTimer;
    
    while(d<=power)
    {
        int x = self->x+d*dx;
        int y = self->y+d*dy;
        if(Map_get(self->map,x,y)->blockBombs>0)
        {
            return;
        }else
        if(Map_wallType(self->map,x,y)==0)
        {
            if(Map_get(self->map,x,y)->burnTime<Config_get()->bombFireTimer)
            {
                Map_get(self->map,x,y)->burnTime = Config_get()->bombFireTimer;
            }
        }else
        if(Map_wallType(self->map,x,y)==2)
        {
            Map_get(self->map,x,y)->type = 0;
            Map_get(self->map,x,y)->blockBombs = 5;
            return;
        }else
        {
            break;
        }
        d = d + 1;
    }
}

void Bomb_explode(struct Bomb* self)
{
    if(self->alive && !self->fakebomb)
    {
        Bomb_wave(self,1,0);
        Bomb_wave(self,-1,0);
        Bomb_wave(self,0,1);
        Bomb_wave(self,0,-1);
    }
    self->alive = 0;
    
    Semaphore_grab(self->map->mapSem);
    Map_get(self->map,self->x,self->y)->hasBomb = 0;
    if(!self->fakebomb)
        self->bomberman->bombCount++;
    Semaphore_release(self->map->mapSem);
}

int Bomb_willAffectWave(struct Bomb* self, int cx, int cy, int dx, int dy)
{
    int power = self->bomberman->bombPower;
    int d = 1;

    while(d<=power)
    {
        int x = self->x+d*dx;
        int y = self->y+d*dy;
        
        if(x==cx && y==cy)
            return 1;
        
        if(Map_wallType(self->map,x,y)==0)
        {

        }else
        if(Map_wallType(self->map,x,y)==2)
        {
            return 0;
        }else
        {
            break;
        }
        d = d + 1;
    }
    return 0;
}

int Bomb_willAffect(struct Bomb* self, int x, int y)
{
    if(x==self->x && y==self->y)
        return 1;

    if(self->alive)
    {
        int a1 = Bomb_willAffectWave(self,x,y,1,0);
        int a2 = Bomb_willAffectWave(self,x,y,-1,0);
        int a3 = Bomb_willAffectWave(self,x,y,0,1);
        int a4 = Bomb_willAffectWave(self,x,y,0,-1);
        return (a1||a2||a3||a4);
    }
    return 0;
}
