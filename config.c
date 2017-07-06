#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"

struct Config* Config_get()
{
    static struct Config config;
    return &config;
}

void Config_init()
{
    struct Config* self = Config_get();
    self->bombTimer = 60;
    self->bombFireTimer = 50;
    self->bombFireCenterTimer = 60;
    self->rockChance = 80;
}

void Config_loadFile(char* path)
{
    struct Config* self = Config_get();
    
    FILE *fp = fopen(path, "r");
    if(fp==0)
    {
        return;
    }
    
    while(!feof(fp))
    {
        char com[255];
        fscanf(fp, "%s", com);
        if(strcmp(com,"controls") == 0)
        {
            int playerid;
            fscanf(fp, "%d", &playerid);
            
            char dir[10];
            fscanf(fp, "%s", dir);
            
            int key;
            fscanf(fp, "%d", &key);
            
            if(strcmp(dir,"up") == 0)
            {
                self->controls[playerid].up = key;
                LOG("Binded key %d to up of player %d",key,playerid);
            }
            if(strcmp(dir,"down") == 0)
            {
                self->controls[playerid].down = key;
                LOG("Binded key %d to down of player %d",key,playerid);
            }
            if(strcmp(dir,"left") == 0)
            {
                self->controls[playerid].left = key;
                LOG("Binded key %d to left of player %d",key,playerid);
            }
            if(strcmp(dir,"right") == 0)
            {
                self->controls[playerid].right = key;
                LOG("Binded key %d to right of player %d",key,playerid);
            }
            if(strcmp(dir,"bomb") == 0)
            {
                self->controls[playerid].bomb = key;
                LOG("Binded key %d to bomb of player %d",key,playerid);
            }
        }else
        if(strcmp(com,"bombTimer") == 0)
        {
            int bombTimer;
            fscanf(fp, "%d", &bombTimer);
            
            self->bombTimer = bombTimer;
        }else
        if(strcmp(com,"bombFireTimer") == 0)
        {
            int bombFireTimer;
            fscanf(fp, "%d", &bombFireTimer);
            
            self->bombFireTimer = bombFireTimer;
        }else
        if(strcmp(com,"bombFireCenterTimer") == 0)
        {
            int bombFireCenterTimer;
            fscanf(fp, "%d", &bombFireCenterTimer);
            
            self->bombFireCenterTimer = bombFireCenterTimer;
        }else
        if(strcmp(com,"rockChance") == 0)
        {
            int rockChance;
            fscanf(fp, "%d", &rockChance);
            
            self->rockChance = rockChance;
        }
    }
}
