#ifndef H_CONFIG
#define H_CONFIG

struct PlayerControls
{
    int up;
    int down;
    int left;
    int right;
    int bomb;
};

struct Config
{
    struct PlayerControls controls[4];
    int bombTimer;
    int bombFireTimer;
    int bombFireCenterTimer;
    int rockChance;
};

struct Config* Config_get();
void Config_init();
void Config_loadFile(char* path);

#endif
