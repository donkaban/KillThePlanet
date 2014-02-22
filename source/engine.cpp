#include "engine.h"


void engine::_update()
{
 	auto currTime = timer::get();

 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(onUpdate) onUpdate(currTime-deltaTime);
    for(const auto &obj : renderQueue) 
        obj->render();
    deltaTime = currTime;
}

void engine::_init()
{
  	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glEnable (GL_BLEND);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);    
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}