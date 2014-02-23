#include "utils.h"
#include "engine.h"
#include "render.h"

#include <list>

class ship;
class bullet;
class enemy;

struct  game
{
    static std::shared_ptr<ship> player;
    static std::list<bullet *>   bullets;
    static std::list<enemy  *>   enemys;

    static void start();
};


class entity
{
public:
    static const int scrW = 6;
    static const int scrH = 4;

    entity();          
    virtual ~entity(); 
    virtual bool update(float) = 0;
    bool bounder(mat &);
protected:    
    vec         pos;
};

class enemy : public entity, public object
{
public:    
    enemy(float, vec::ref, vec::ref);
    void spawn();
    virtual bool update(float) override;
private:
    float size;
    vec   vel;
    vec   rot;
};

class bullet : public entity, public object
{
public:
    bullet(mat::ref t);
    virtual ~bullet();
    virtual bool update(float dt) override;
};

class ship : public entity, public object
{
   static const int const_angle = 120;     
   static const int const_accel = 12;     
public:    
    ship();
    void left();  
    void right(); 
    void move();  
    void brake();
    virtual bool update(float dt) override;
private:
    float accel = 0; 
    float angle = 0;    
};

