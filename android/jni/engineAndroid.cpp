#ifdef PLATFORM_ANDROID
#include "engine.h"
#include "game.h"

#include <thread>
#include <mutex>
#include <jni.h>

static engine   *e ;
static std::mutex locker;

extern "C" {

JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_init(JNIEnv *env, jobject obj)
{
    std::lock_guard<std::mutex> lock(locker);
    try
    {
        e = new engine();
    } 
    catch(std::exception e)
    {
        ERR("[jni] init : %s", e.what());
    }
}

JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_draw(JNIEnv *env, jobject obj)
{
    std::lock_guard<std::mutex> lock(locker);
    e->update();
}

JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_pause(JNIEnv *env, jobject obj)
{}  // todo

JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_resume(JNIEnv *env, jobject obj)
{}  // todo

JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_resize(JNIEnv *env, jobject obj, jint w, jint h)
{
    std::lock_guard<std::mutex> lock(locker);
    e->resize(w,h);
    game::start();
    e->setUpdateHandler([&](float dt)
    {
        game::background->render();
        if(game::player->update(dt)) game::start();
        updater(game::enemys,dt);
        updater(game::bullets,dt);
    });
}
JNIEXPORT void JNICALL 
Java_com_kaban_asteroids_Game_onButton(JNIEnv *env, jobject obj, jint key)
{
    std::lock_guard<std::mutex> lock(locker);
    switch(key)
    {
        case 0 : game::player->left();  break;
        case 1 : game::player->right(); break;
        case 2 : game::player->brake(); break;
        case 3 : game::player->move();  break;
        case 4 : game::bullets.push_back(new bullet(game::player->transform())); break; 
        case 5 : game::start(); break;
    }
}
}


engine::engine()  {}
engine::~engine() {}

bool engine::update()
{
    _update();
    return true;
}
void engine::resize(int w, int h)
{
	 _init();
    width = w;
	height = h;
	NFO("resize: %d x %d", width, height);
    glViewport(0, 0, width, height);
}



#endif