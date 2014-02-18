#include "engine.h"
#include "utils.h"
#include "platform.h"


#ifdef PLATFORM_LINUX
int main()
{
	try
	{
		platform p;

		while(p.update()) {}
	}
	catch(std::runtime_error e)
	{
		logger::error(e.what());
	}


	return 0;
}



#endif