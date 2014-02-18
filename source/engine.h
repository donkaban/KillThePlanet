//

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>

#if defined(PLATFORM_ANDROID)
    #include <GLES2/gl2.h>
#elif defined(PLATFORM_LINUX)
    #include <GL/gl.h>
#endif      



