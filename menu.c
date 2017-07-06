#include "menu.h"
#include "game.h"

void Menu_run()
{
    int selected = 0;
    int playerCount = 1;
    int time = 0;

    int done = 0;
    while(!done){
        Console_clear();
        
        int key = getch();
        if(key!=ERR)
        {
            LOG("You pressed %d",key);
            if(key==119 || key==259)
            {
                selected = selected + 1;
                if(selected>=2)
                    selected = 0;
            }
            if(key==115 || key==258)
            {
                selected = selected - 1;
                if(selected<0)
                    selected = 1;
            }
            
            if(selected==0 && (key==97 || key==260))
            {
                playerCount = playerCount - 1;
                if(playerCount<=0)
                    playerCount = 4;
            }
            if(selected==0 && (key==100 || key==261))
            {
                playerCount = playerCount + 1;
                if(playerCount>4)
                    playerCount = 1;
            }
            
            if(key==27 || (selected==1 && (key==101 || key==32 || key==10)))
            {
                done = 1;
            }
            if(selected==0 && (key==101 || key==32 || key==10))
            {
                Game_run(playerCount);
            }
        }
        
        
        
        Sprite_draw(Sprites_get(S_LOGO),0,2);
        Sprite_draw(Sprites_get(S_COPYRIGHT),0,24);
        if(time>=15)
        {
            Sprite_draw(Sprites_get(S_MENUBOMB),45,19);
        }else
        {
            Sprite_draw(Sprites_get(S_MENUBOMB2),45,19);
        }
        if(playerCount==1)
            Console_drawStr(26,12,L"1 Player");
        else if(playerCount==2)
            Console_drawStr(26,12,L"2 Players");
        else if(playerCount==3)
            Console_drawStr(26,12,L"3 Players");
        else if(playerCount==4)
            Console_drawStr(26,12,L"4 Players");
        Console_drawStr(28,14,L"Quit");
        
        time = time + 1;
        if(time>50)
            time = 0;
        
        wchar_t ch = L'▶';
        wchar_t ch2 = L'◀';
        if(time>=25)
        {
            ch = L'▷';
            ch2 = L'◁';
        }
            
        if(selected==0)
        {
            Console_drawChar(24,12,ch2); 
            Console_drawChar(36-(playerCount==1),12,ch); 
        }else
        {
            Console_drawChar(26,14,ch);
        }
        
        
        Console_draw();
        usleep(1000000/30);
    }
}
