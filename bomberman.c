#include "bomberman.h"
#include "sprites.h"
#include "logger.h"
#include "config.h"
#include <ctype.h>
#include "map.h"

void Bomberman_aiInit(struct Bomberman* self)
{
    self->lastDirection = -1;
    self->tickAi = 0;
}

int opposite_dir(int dir)
{
    if(dir==1)
        return 2;
    if(dir==2)
        return 1;
    if(dir==3)
        return 4;
    if(dir==4)
        return 3;
    return 0;
}

void Bomberman_ai(struct Bomberman* self)
{
    int bombChanceBoost = 0;
    self->tickAi--;
    if(self->tickAi<=0)
    {
        self->tickAi = 2+rand()%3;
    }else
    {
        return;
    }
    int safeDir = Map_safeDirection(self->map, self->x,self->y);
    if(safeDir!=0)
    {
        self->direction = safeDir;
        self->lastDirection = self->direction;
        self->tickAi = 1;
    }else
    {
        int freeLeft = !Map_hasSomething(self->map,self->x-1,self->y);
        int freeRight = !Map_hasSomething(self->map,self->x+1,self->y);
        int freeUp = !Map_hasSomething(self->map,self->x,self->y-1);
        int freeDown = !Map_hasSomething(self->map,self->x,self->y+1);
        
        int count = freeLeft+freeRight+freeUp+freeDown;
        
        int lastDirectionFree = 0;
        if((self->lastDirection == 1 && freeLeft) || (self->lastDirection == 2 && freeRight) ||
          (self->lastDirection == 3 && freeUp) || (self->lastDirection == 4 && freeDown))
        {
            lastDirectionFree = 1;
        }
        
        if(count==1)
            bombChanceBoost = bombChanceBoost + 50;
        else if(count==2)
            bombChanceBoost = bombChanceBoost + 20;
        
        if(count==0)
        {
        }else
        if(count==1)
        {
            self->direction = freeLeft+freeRight*2+freeUp*3+freeDown*4;
            self->lastDirection = self->direction;
        }else
        if(count == 2 && lastDirectionFree)
        {
            self->direction = self->lastDirection;
        }else
        {
            int nearestEnemyDirection = Map_dirToNearestEnemy(self->map,self);
            int canGo = ((nearestEnemyDirection==1 && freeLeft) || (nearestEnemyDirection==2 && freeRight)
                      || (nearestEnemyDirection==3 && freeUp) || (nearestEnemyDirection==4 && freeDown) );
            if(nearestEnemyDirection!=0 && canGo && rand()%100<60)
            {
                self->direction = nearestEnemyDirection;
                self->lastDirection = self->direction;
            }else
            {
                int randDir = 0;
                while(1)
                {
                    randDir = rand()%4+1;
                    if(opposite_dir(randDir)==self->lastDirection)
                        continue;

                        if(randDir==1 && !freeLeft)
                            continue;
                        if(randDir==2 && !freeRight)
                            continue;
                        if(randDir==3 && !freeUp)
                            continue;
                        if(randDir==4 && !freeDown)
                            continue;
                        break;

                }
                self->direction = randDir;
                self->lastDirection = self->direction;
            }
        }
        int newx = self->x+(self->direction==2)-(self->direction==1);
        int newy = self->y+(self->direction==4)-(self->direction==3);
        if(Map_inDanger(self->map,newx,newy))
            self->direction = 0;
    }
    int newx = self->x+(self->direction==2)-(self->direction==1);
    int newy = self->y+(self->direction==4)-(self->direction==3);
    int bombChance = rand()%100;
    if(bombChance>(90-bombChanceBoost) && self->direction!=0)
    {
        int dist = Map_safeDistIfBomb(self->map, self->x,self->y, self);
        
        if(dist<=3)
        {
            self->shouldBomb = 1;
            self->direction = 0;
            self->tickAi = 1;
        }
    }
}

void* Bomberman_run(void* ptr)
{
    struct Bomberman* self = (struct Bomberman*)ptr;
    
    while(self->alive)
    {
        if(Map_get(self->map,self->x,self->y)->burnTime>0)
        {
            Semaphore_grab(self->map->mapSem);
            self->showGrave = 1;
            self->alive = 0;
            Map_get(self->map,self->x,self->y)->hasPlayer = 0;
            Semaphore_release(self->map->mapSem);
            break;
        }
    
        if(self->isAi)
        {
            Bomberman_ai(self);
        }else
        {
            //LOG("%d",Map_safeDistIfBomb(self->map, self->x,self->y, self));
        }
        //self->lastd = Map_safeDirection(self->map, self->x,self->y);
    
        if(self->shouldBomb)
        {
            self->shouldBomb = 0;
            Semaphore_grab(self->map->mapSem);
            if(Map_get(self->map,self->x,self->y)->hasBomb==0 && self->bombCount>0)
            {
                self->bombCount--;
                Map_createBomb(self->map,self->x,self->y,self);
            }
            Semaphore_release(self->map->mapSem);
        }
        if(self->direction!=0)
        {
            int oldx = self->x;
            int oldy = self->y;
            if(self->direction==3)
            {
                self->y = self->y - 1;
            }
            if(self->direction==4)
            {
                self->y = self->y + 1;
            }
            if(self->direction==1)
            {
                self->x = self->x - 1;
            }
            if(self->direction==2)
            {
                self->x = self->x + 1;
            }
            self->direction = 0;
            
            Semaphore_grab(self->map->mapSem);
            int hasPlayer = Map_get(self->map,self->x,self->y)->hasPlayer;
            int hasBomb = Map_get(self->map,self->x,self->y)->hasBomb;
            if(Map_hasWall(self->map,self->x,self->y) || hasPlayer || hasBomb)
            {
                self->x = oldx;
                self->y = oldy;
            }else
            {
                Map_get(self->map,oldx,oldy)->hasPlayer = 0;
                Map_get(self->map,self->x,self->y)->hasPlayer = 1;
                
                if(Map_get(self->map,self->x,self->y)->hasBonus>0)
                {
                    if(Map_get(self->map,self->x,self->y)->hasBonus==1)
                    {
                        self->bombCount++;
                    }else
                    {
                        self->bombPower++;
                    }
                    
                    Map_get(self->map,self->x,self->y)->hasBonus = 0;
                }
            }
            Semaphore_release(self->map->mapSem);
        }
        usleep(1000000/10);
    }
}


void Bomberman_init(struct Bomberman* self, int x, int y, int alive, int i, struct Map* map, int isAi)
{
    self->x = x;
    self->y = y;
    self->alive = alive;
    self->id = i;
    self->direction = 0;
    self->shouldBomb = 0;
    self->map = map;
    self->showGrave = 0;
    self->bombCount = 2;
    self->bombPower = 2;
    self->isAi = isAi;
    Bomberman_aiInit(self);
    
    self->thread = 0;
    if(self->alive)
    {
        Map_get(self->map,self->x,self->y)->hasPlayer = 1;
        pthread_create(&self->thread, NULL, Bomberman_run, self);
    }
    
}

void Bomberman_free(struct Bomberman* self)
{
    self->alive = 0;
    
    if(self->thread>0)
    {
        pthread_join(self->thread, NULL);
    }
}

void Bomberman_draw(struct Bomberman* self)
{
    if(self->alive)
    {
        Sprite_draw(Sprites_get(S_BOMBERMAN),self->x*4,self->y*2);

        /*if(self->lastd==1)
        {
            Console_drawStr(self->x*4,self->y*2,L"<");
        }
        if(self->lastd==2)
        {
            Console_drawStr(self->x*4,self->y*2,L">");
        }
        if(self->lastd==3)
        {
            Console_drawStr(self->x*4,self->y*2,L"^");
        }
        if(self->lastd==4)
        {
            Console_drawStr(self->x*4,self->y*2,L"V");
        }*/
    }
}

void Bomberman_drawGrave(struct Bomberman* self)
{
    if(!self->alive && self->showGrave)
    {
        Sprite_draw(Sprites_get(S_GRAVE),self->x*4,self->y*2);
    }
}

void Bomberman_handleKey(struct Bomberman* self, int d)
{
    if(self->isAi)
        return;

    if(self->alive)
    {
        int c = d;
        if(isalpha(d))
        {
            c = tolower(c);
        }
        
        if(c==Config_get()->controls[self->id].up)
        {
            self->direction = 3;
        }
        if(c==Config_get()->controls[self->id].down)
        {
            self->direction = 4;
        }
        if(c==Config_get()->controls[self->id].left)
        {
            self->direction = 1;
        }
        if(c==Config_get()->controls[self->id].right)
        {
            self->direction = 2;
        }
        if(c==Config_get()->controls[self->id].bomb)
        {
            self->shouldBomb = 1;
        }
    }
}
