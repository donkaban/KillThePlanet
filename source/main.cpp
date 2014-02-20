#include "utils.h"
#include "engine.h"
#include "render.h"


#ifdef PLATFORM_LINUX
int main()
{
    try
    {
        engine q;
        object o(
        {
            { 1, 1, 0,   0, 1,  0, 0, 1},
            {-1, 1, 0,   0, 0,  0, 0, 1},
            {-1,-1, 0,   1, 0,  0, 0, 1},
            { 1,-1, 0,   1, 1,  0, 0, 1},
        },
        {0, 1, 2, 2, 3, 0}   
        );




        q.setUpdateHandler([&]()
        {
            q.setBackColor(vec4(rnd(),rnd(),rnd(),1));
            logger::info("time: %f", q.getTime());
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