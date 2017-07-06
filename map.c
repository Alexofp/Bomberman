#include "map.h"
#include "console.h"
#include <stdlib.h>
#include "sprites.h"
#include "config.h"

void Map_init(struct Map* self, int w,int h, int playerCount)
{
    self->w = w;
    self->h = h;
    self->map = (struct MapItem*)malloc(sizeof(struct MapItem)*w*h);
    int x,y;
    for(y=0;y<h;y++)
    {
        for(x=0;x<w;x++)
        {
            self->map[y*w+x].type = 0;
            self->map[y*w+x].burnTime = 0;
            self->map[y*w+x].hasPlayer = 0;
            self->map[y*w+x].hasBomb = 0;
            self->map[y*w+x].blockBombs = 0;
            
            if(rand()%100 < Config_get()->rockChance)
            {
                self->map[y*w+x].type = 2;
            }
            
            if((x%2==0 && y%2==0) || x==0 || x==w-1 || y==0 || y==h-1)
                self->map[y*w+x].type = 1;
        }
    }
    self->map[1*w+1].type = 0;
    self->map[1*w+2].type = 0;
    self->map[2*w+1].type = 0;
    
    self->map[1*w+w-2].type = 0;
    self->map[1*w+w-3].type = 0;
    self->map[2*w+w-2].type = 0;
    
    self->map[(h-2)*w+w-2].type = 0;
    self->map[(h-2)*w+w-3].type = 0;
    self->map[(h-3)*w+w-2].type = 0;
    
    self->map[(h-2)*w+1].type = 0;
    self->map[(h-2)*w+2].type = 0;
    self->map[(h-3)*w+1].type = 0;
    
    for(y=0;y<h;y++)
    {
        for(x=0;x<w;x++)
        {
            self->map[y*w+x].hasBonus = 0;
            if(self->map[y*w+x].type==2)
            {
                if(rand()%100 > 80)
                {
                    self->map[y*w+x].hasBonus = rand()%2+1;
                }
            }
        }
    }
    
    
    int i;
    for(i=0;i<BOMB_COUNT;i++)
    {
        Bomb_init(&self->bombs[i], self);
        self->bombsSem[i] = Semaphore_init(1);
    }
    
    self->mapSem = Semaphore_init(1);
    
    Bomberman_init(&self->bombermans[0],1,1,1,0,self,0);
    Bomberman_init(&self->bombermans[1],13,1,1,1,self,(playerCount<=1));
    Bomberman_init(&self->bombermans[2],13,11,1,2,self,(playerCount<=2));
    Bomberman_init(&self->bombermans[3],1,11,1,3,self,(playerCount<=3));
}

void Map_free(struct Map* self)
{
    free(self->map);
    int i;
    for(i=0;i<BOMB_COUNT;i++)
    {
        Semaphore_free(self->bombsSem[i]);
    }
    Semaphore_free(self->mapSem);
    
    for(i=0;i<4;i++)
    {
        Bomberman_free(&self->bombermans[i]);
    }
}

void Map_handleKey(struct Map* self, int key)
{
    int i;
    for(i=0;i<4;i++)
    {
        Bomberman_handleKey(&self->bombermans[i], key);
    }
}

void Map_draw(struct Map* self)
{
    int x,y;
    for(y=0;y<self->h;y++)
    {
        for(x=0;x<self->w;x++)
        {
            if(self->map[y*self->w+x].type==0)
            {
                if(self->map[y*self->w+x].burnTime>0)
                {
                    Sprite_draw(Sprites_get(S_FIRE),x*4,y*2);
                }else
                {
                
                    if(self->map[y*self->w+x].hasBonus==0)
                    {
                        Sprite_draw(Sprites_get(S_FLOOR),x*4,y*2);
                    }else
                    if(self->map[y*self->w+x].hasBonus==1)
                    {
                        Sprite_draw(Sprites_get(S_BONUS1),x*4,y*2);
                    }else
                    if(self->map[y*self->w+x].hasBonus==2)
                    {
                        Sprite_draw(Sprites_get(S_BONUS2),x*4,y*2);
                    }
                    
                }
            }
            if(self->map[y*self->w+x].type==1)
            {
                Sprite_draw(Sprites_get(S_WALL),x*4,y*2);
            }
            if(self->map[y*self->w+x].type==2)
            {
                Sprite_draw(Sprites_get(S_BRICKS),x*4,y*2);
            }
        }
    }
    int i;
    for(i=0;i<4;i++)
    {
        Bomberman_drawGrave(&self->bombermans[i]);
    }
    for(i=0;i<BOMB_COUNT;i++)
    {
        Semaphore_grab(self->bombsSem[i]);
        Bomb_draw(&self->bombs[i]);
        Semaphore_release(self->bombsSem[i]);
    }
    for(i=0;i<4;i++)
    {
        Bomberman_draw(&self->bombermans[i]);
    }
}

void Map_update(struct Map* self)
{
    int i;
    for(i=0;i<BOMB_COUNT;i++)
    {
        Semaphore_grab(self->bombsSem[i]);
        Bomb_update(&self->bombs[i]);
        Semaphore_release(self->bombsSem[i]);
    }

    int x,y;
    for(y=0;y<self->h;y++)
    {
        for(x=0;x<self->w;x++)
        {
            self->map[y*self->w+x].burnTime = self->map[y*self->w+x].burnTime - 1;
            self->map[y*self->w+x].blockBombs = self->map[y*self->w+x].blockBombs - 1;
        }
    }
}

int Map_hasWall(struct Map* self, int x, int y)
{
    if(x<0 || x>=self->w || y<0 || y>=self->h)
        return 1;
    int type = self->map[y*self->w+x].type;
    
    if(type == 0)
        return 0;
    return 1;
}

int Map_hasSomething(struct Map* self, int x, int y)
{
    if(Map_hasWall(self,x,y))
        return 1;
        
    int hasPlayer = self->map[y*self->w+x].hasPlayer;
    int hasBomb = self->map[y*self->w+x].hasBomb;
    int hasFire = self->map[y*self->w+x].burnTime>=0;
    
    if(hasPlayer || hasBomb || hasFire)
        return 1;
}

int Map_wallType(struct Map* self, int x, int y)
{
    if(x<0 || x>=self->w || y<0 || y>=self->h)
        return -1;
    int type = self->map[y*self->w+x].type;
    
    return type;
}

int Map_createBomb(struct Map* self, int x,int y, struct Bomberman* b)
{
    int i;
    int found = -1;
    for(i=0;i<BOMB_COUNT;i++)
    {
        Semaphore_grab(self->bombsSem[i]);
        if(!self->bombs[i].alive)
        {
            Bomb_create(&self->bombs[i],x,y,b);
            Semaphore_release(self->bombsSem[i]);
            found = i;
            break;
        }
        Semaphore_release(self->bombsSem[i]);
    }
    return found;
}

struct MapItem* Map_get(struct Map* self, int x,int y)
{
    if(x<0 || x>=self->w || y<0 || y>=self->h)
        return 0;
        
    return &self->map[y*self->w+x];
}

int Map_inDanger(struct Map* self, int x,int y)
{
    int i;
    for(i=0;i<BOMB_COUNT;i++)
    {
        if(self->bombs[i].alive)
        {
            if(Bomb_willAffect(&self->bombs[i],x,y))
                return 1;
        }
    }
    return 0;
}

int Map_cantGo(struct Map* self, int x, int y)
{
    if(Map_hasWall(self,x,y))
        return 1;
        
    int hasBomb = self->map[y*self->w+x].hasBomb;
    int hasFire = self->map[y*self->w+x].burnTime>0;
    
    if(hasBomb || hasFire)
        return 1;
}

int Map_findSafe(struct Map* self, int x,int y,char* blackMap, int dist, int returnDir)
{
    if(blackMap[y*self->w+x])
        return 99999;
    blackMap[y*self->w+x] = 1;
    if(Map_cantGo(self,x,y) && !returnDir)
    {
        return 99999;
    }
    if(!Map_inDanger(self,x,y))
    {
        return dist;
    }
    
    int m1 = Map_findSafe(self,x-1,y,blackMap, dist+1, 0);
    int m2 = Map_findSafe(self,x+1,y,blackMap, dist+1, 0);
    int m3 = Map_findSafe(self,x,y-1,blackMap, dist+1, 0);
    int m4 = Map_findSafe(self,x,y+1,blackMap, dist+1, 0);
    
    int min = m1;
    if(m2<min)
        min = m2;
    if(m3<min)
        min = m3;
    if(m4<min)
        min = m4;
    if(returnDir)
    {
        if(min==99999)
            return 0;
        if(min==m1)
            return 1;
        if(min==m2)
            return 2;
        if(min==m3)
            return 3;
        if(min==m4)
            return 4;
        return 0;
    }else
    {
        return min;
    }
}

int Map_safeDirection(struct Map* self, int px,int py)
{
    char* blackMap = malloc(self->w*self->h*sizeof(char));
    int x,y;
    for(y=0;y<self->h;y++)
    {
        for(x=0;x<self->w;x++)
        {
            blackMap[y*self->w+x] = 0;
        }
    }
    
    int d = Map_findSafe(self,px,py,blackMap, 0, 1);
    free(blackMap);
    return d;
}

int Map_safeDistIfBomb(struct Map* self, int px,int py, struct Bomberman* b)
{
    char* blackMap = malloc(self->w*self->h*sizeof(char));
    int x,y;
    for(y=0;y<self->h;y++)
    {
        for(x=0;x<self->w;x++)
        {
            blackMap[y*self->w+x] = 0;
        }
    }
    
    int id = Map_createBomb(self,px,py,b);
    self->bombs[id].fakebomb = 1;
    Map_get(self,px,py)->hasBomb = 0;
    
    int d = Map_findSafe(self,px,py,blackMap, 0, 0);
    if(id>=0)
    {
        Bomb_explode(&self->bombs[id]);
    }

    free(blackMap);
    return d;
}

int Map_dirToNearestEnemy(struct Map* self, struct Bomberman* b)
{
    int closest = -1;
    int minDist = 9999;
    
    int i;
    for(i=0;i<4;i++)
    {
        if(&self->bombermans[i] != b && self->bombermans[i].alive)
        {
            int dist = abs(b->x-self->bombermans[i].x) + abs(b->y-self->bombermans[i].y);
            
            if(dist<minDist)
            {
                minDist = dist;
                closest = i;
            }
        }
    }
    if(closest==-1 || minDist<=3)
    {
        return 0;
    }
    if(abs(b->x-self->bombermans[closest].x) > abs(b->y-self->bombermans[closest].y))
    {
        if((b->x-self->bombermans[closest].x)>0)
            return 1;
        else
            return 2;
    }else
    {
        if((b->y-self->bombermans[closest].y)>0)
            return 3;
        else
            return 4;
    }
}
