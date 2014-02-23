#include "utils.h"
#include "engine.h"
#include "render.h"

class entity
{
public:
    entity(engine &e) : e(e) {} 
    virtual ~entity() {}
    virtual bool update(float) = 0;
    float   distance(const vec &p) 
    {
        auto dir = p - pos;
        return dir.length();
    }
protected:    
    engine      &e; 
    object::ptr model;
    vec         pos;
    float       collisionRadius = 0;
};


class bullet : public entity
{
public:
    bullet(engine &e, mat::ref t, float speed) : 
        entity(e), 
        speed(speed)
    {
        model = std::make_shared<object>(object(
        {0,-.3,0,.5,.5,.3,-.5,0,1,1,0,.5,0,.5,0,-.3,-.5, 0,0,1},
        {0,1,2,2,3,0}));    
        model->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::bullet));    
        model->transform(t * mat::translate({0,2,0}) * mat::scale({1,1,1}));
        e.addObject(model);
    }
    virtual ~bullet() {e.removeObject(model);}
    virtual bool update(float dt) override
    {
        model->translate({0,dt *(14 + speed),0});
        pos = model->position();
        return false;
    }
private:
    float speed = 0;
    vec   pos;
};

class ship : public entity
{
   static const int const_angle = 120;     
   static const int const_accel = 12;     
public:    
    ship(engine & e) : entity(e)
    {   
        model = std::make_shared<object>(object(
        {0,-2.5,0,1,0,2.5,-1,0,0,.5,1,.5,1,1,.5,.5,2.5,0,1,1,-1,.5,1,1,.5,-2.5,-1,0,0,.5,-.5,2.5,0,1,1},
        {0,1,2,2,1,3,0,2,4,4,5,0,6,5,4,}));      
        model->scale({.2,.2,.2});
        model->setMaterial(std::make_shared<material>(shaders::vertex_base,shaders::ship));
        e.addObject(model);
    };
    void left()  {angle = const_angle;};
    void right() {angle =-const_angle;}
    void move()  {accel = const_accel;}
    void brake() {accel = 0;}
    void shoot()
    {
        bullets.push_back(new bullet(e, model->transform(), accel));
    }
    virtual bool update(float dt) override
    {
        if(angle!=0) {model->rotate({0,0,angle * dt});    angle *= 0.950;}   
        if(accel!=0) {model->translate({0,accel * dt,0}); accel *= 0.995;}  
        pos = model->position();
        if(pos.y < -6 || pos.y > 6) {pos.y = -pos.y;model->position(pos);}
        if(pos.x < -4 || pos.x > 4) {pos.x = -pos.x;model->position(pos);}
        for(auto b : bullets) b->update(dt);
        return false;
    }

private:
    float accel = 0; 
    float angle = 0;    
    std::vector<bullet *> bullets;
};




#ifdef PLATFORM_LINUX
int main()
{
    try
    {
        engine  q;
        ship    player(q);
                
        q.setUpdateHandler([&](float dt)
        {
            player.update(dt);
            logger::info("distance : %f", player.distance({0,0,0}));
        });

        q.setKeyHandler([&](int key)
        {
            switch(key)
            {
                case engine::KEY::LEFT  : player.left();  break;
                case engine::KEY::RIGHT : player.right(); break;
                case engine::KEY::DOWN  : player.brake(); break;
                case engine::KEY::UP    : player.move();  break;
                case engine::KEY::SPACE : player.shoot(); break;
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