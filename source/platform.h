#ifndef _PLATFORM_MODULE_
#define _PLATFORM_MODULE_

#include <functional>

class platform
{
public:
	typedef std::function<void()> 		    updatehandler_t;
	typedef std::function<void(int, int)>	mousehandler_t;
	typedef std::function<void(int)>		keyhandler_t;



	platform();
	~platform();

	platform(const platform &) = delete;
	platform & operator = (const platform &) = delete;

	bool update();
	void resize();

	inline void setKeyHandler(keyhandler_t hdl)	 		{onKey=hdl;}
	inline void setMouseHandler(mousehandler_t hdl) 	{onMouse=hdl;}
	inline void setUpdateHandler(updatehandler_t hdl) 	{onUpdate=hdl;}

private:
	updatehandler_t onUpdate = updatehandler_t();
    mousehandler_t  onMouse  = mousehandler_t();
 	keyhandler_t    onKey  	 = keyhandler_t();

	unsigned int _width  = 600;
	unsigned int _height = 800;
};



#endif