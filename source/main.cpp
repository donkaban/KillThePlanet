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
        object::ptr o = std::make_shared<object>(object(
        {
            { W, W, 0,   1, 0,  0, 0, 1},
            {-W, W, 0,   0, 0,  0, 0, 1},
            {-W,-W, 0,   0, 1,  0, 0, 1},
            { W,-W, 0,   1, 1,  0, 0, 1},
        },
        {2,3,0,0,1,2}));

        material::ptr m1 = std::make_shared<material>("mat1",shaders::vertex_base,shaders::fragment_base);
        o->setMaterial(m1);
        q.addObject(o);
        


        q.setUpdateHandler([&]()
        {
            o->setColor({rnd(),rnd(),rnd(),1});
            o->rotate({0.01,-0.01,0.02});
            logger::info("time: %f", timer::get());
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