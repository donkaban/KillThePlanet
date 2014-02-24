#include "engine.h"
#include <cstdlib>
#include "shaders.h"


std::unordered_map<std::string, material::ptr> engine::materials;
 
void engine::_update()
{
    auto currTime = timer::get();
 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(onUpdate) onUpdate(currTime-deltaTime);
    deltaTime = currTime;
}

void engine::_init()
{
    srand (time(NULL));
  	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glEnable (GL_BLEND);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);    
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    NFO("create materials");
    materials["ship"]   = std::make_shared<material>(shaders::vertex_base,shaders::ship);
    materials["bullet"] = std::make_shared<material>(shaders::vertex_base,shaders::bullet);
    materials["enemy"]  = std::make_shared<material>(shaders::vertex_base,shaders::enemy);
    materials["back"]   = std::make_shared<material>(shaders::vertex_base,shaders::back);
}

