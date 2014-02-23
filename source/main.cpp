#include "game.h"
#include "utils.h"
#include "engine.h"
#include "render.h"

#ifdef PLATFORM_LINUX
int main()
{
    try
    {
        engine  e;

        game::start();  
        
        e.setUpdateHandler([&](float dt)
        {
            game::background->render();
            if(game::player->update(dt)) game::start();
            updater(game::enemys,dt);
            updater(game::bullets,dt);
        });

        e.setKeyHandler([&](int key)
        {
            switch(key)
            {
                case engine::KEY::LEFT  : game::player->left();  break;
                case engine::KEY::RIGHT : game::player->right(); break;
                case engine::KEY::DOWN  : game::player->brake(); break;
                case engine::KEY::UP    : game::player->move();  break;
                case engine::KEY::SPACE : game::bullets.push_back(new bullet(game::player->transform())); break; 
                case engine::KEY::ESC   : game::start(); break;
            }
        });
       e.mainLoop();
    }
    catch(std::exception e)
    {
        logger::error("error : %s",e.what());
    }


    return 0;
}



#endif