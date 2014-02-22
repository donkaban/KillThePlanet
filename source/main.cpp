#include "utils.h"
#include "engine.h"
#include "render.h"


#ifdef PLATFORM_LINUX
int main()
{
    try
    {
        #define W 0.5
        engine q;
        
        auto bat = object::batman({3,3,3});
         auto cube   = object::cube({3.1,3.1,3.1});
         auto center = object::cube({3,3,3});

         //q.addObject(center);
        // q.addObject(cube);
         q.addObject(bat);
      
        //cube->rotate  ({3,3,0.01});
          
        q.setUpdateHandler([&]()
        {
            bat->setColor({rnd(),rnd(),rnd(),.5});
            cube->rotate  ({0.01,0.01,0.01});
            center->rotate({0.01,-0.01,0.01});
           // logger::info("time: %f", timer::get());
        });

        #define R 0.3
        q.setKeyHandler([&](int key){
            switch(key)
            {
                case 131 : bat->rotate({ 0, 0, -R}); break;
                case 132 : bat->rotate({ 0, 0,  R}); break;
                case 133 : bat->rotate({ R, 0, 0}); break;
                case 134 : bat->rotate({-R, 0, 0}); break;
            }



        });




        while(q.update()) {}
    }
    catch(std::exception e)
    {
        logger::error(e.what());
    }


    return 0;
}



#endif