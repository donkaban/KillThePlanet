#include "game.h"

std::shared_ptr<object> game::background;
std::shared_ptr<ship>   game::player;
std::list<bullet *>     game::bullets;
std::list<enemy  *>     game::enemys;

void game::start()
{
    float phi = 0.0;
    enemys.clear();
    player = std::make_shared<ship>();
    for(auto i = 0; i< 12; i++)
    { 
        float x = 3 * sin(phi);
        float y = 3 * cos(phi);
        enemys.push_back(new enemy(rnd(.2,.6),{x,y,0},{rnd(-.3,.3),rnd(-.3,.3),0})); 
        phi+=0.5;
    }
    background = std::make_shared<object>(object(
        {1,1,0,1,0,1,-1,0,1,1,-1,-1,0,0,1,-1,1,0,0,0},
        {2,1,0,3,2,0},
        engine::getMaterial("back")));
        background->translate({0,0,-30});
        background->scale({9,16,0});
}


entity::entity()  {} 
entity::~entity() {}
bool entity::bounder(mat &t)
{
    bool ret = false;
    pos = t.position();
    if(pos.y < -scrW || pos.y > scrW) {pos.y = -pos.y; ret=true;}
    if(pos.x < -scrH || pos.x > scrH) {pos.x = -pos.x; ret=true;}    
    return ret;
} 
enemy::enemy(float size, vec::ref p, vec::ref v) : 
    object({0,-1,0,0,0,0,0,1,1,0,1,0,-1, 0,1,-1,0,-1,1,1,0,1,0,0,0},
           {2,1,0,3,2,0,0,1,3,3,1,4,4,1,2,2,3,4},
           engine::getMaterial("enemy")),
    size(size)

{
    NFO("create enemy");
    pos = p;
    position(pos);
    rot = vec({rnd(-45,45),rnd(-45,45),rnd(-45,45)});
    scale({size,size,size});
    vel = vec({rnd(-.3,.3),rnd(-.3,.3),0});
}
void enemy::spawn()
{
    if(size < .2) return;
    auto v = vel * 2;
    game::enemys.push_back(new enemy(size/2.f,pos,{v.x,v.y,0}));    
    game::enemys.push_back(new enemy(size/2.f,pos,{-v.x,v.y,0}));    
    game::enemys.push_back(new enemy(size/2.f,pos,{v.x,-v.y,0}));    
    game::enemys.push_back(new enemy(size/2.f,pos,{-v.x,-v.y,0}));    

}
bool enemy::update(float dt)
{
    pos += vel*dt;  
    position(pos);
    rotate(rot * dt);
    bounder(_transform);
    render();
    return false;
}

bullet::bullet(mat::ref t) : 
    object({0,-.3,0,.5,.5,.3,-.5,0,1,1,0,.5,0,.5,0,-.3,-.5, 0,0,1},
           {0,1,2,2,3,0},engine::getMaterial("bullet"))
{
    NFO("create bullet");
    transform(t * mat::translate({0,2,0}) * mat::scale({1,1,1}));
}
bullet::~bullet() {}
bool bullet::update(float dt) 
{
    render();
    translate({0,dt *(24),0});
    for(auto i = game::enemys.begin(); i!= game::enemys.end();i++)
    {
        auto dir = (*i)->position() - pos;
        if(dir.length() < 0.5)
        {
            (*i)->spawn();
            delete *i;
            game::enemys.erase(i);
            return true;
        }    
    }    
    return bounder(_transform);
}

ship::ship() : 
    object({0,-2.5,0,1,0,2.5,-1,0,0,.5,1,.5,1,1,.5,.5,2.5,0,1,1,-1,.5,1,1,.5,-2.5,-1,0,0,.5,-.5,2.5,0,1,1},
           {0,1,2,2,1,3,0,2,4,4,5,0,6,5,4,}, engine::getMaterial("ship"))
{   
    NFO("create ship");
    scale({.2,.2,.2});
}

void ship::left()  {angle = const_angle;};
void ship::right() {angle =-const_angle;}
void ship::move()  {accel = const_accel;}
void ship::brake() {accel = 0;}
bool ship::update(float dt)
{
    if(angle!=0) {rotate({0,0,angle * dt});    angle *= 0.950;}   
    if(accel!=0) {translate({0,accel * dt,0}); accel *= 0.995;}  
    if(bounder(_transform)) position(pos);
    for(auto &e : game::enemys)
    {
        auto dir = e->position() - pos;
        if(dir.length() < 0.5)
            return true;    
    }    
    render();    
    return false;
}
