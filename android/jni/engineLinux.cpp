#ifdef PLATFORM_LINUX

#include "utils.h"
#include "engine.h"

#include <stdexcept>
#include <cstring>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include "GL/glx.h"

static struct data_t
{
    Display  * display;
    Window     window;
    Window     root;    
    int        screen;
    Visual   * visual; 
    GLXContext context;
} data;

engine::~engine()
{
    glXDestroyContext(data.display, data.context);
    XDestroyWindow(data.display, data.window);
}

engine::engine()
{
    NFO("create X11 native window");
    data.display = XOpenDisplay(NULL);
    if (!data.display) 
        throw std::runtime_error("can't open X11 display");

    data.root   = XDefaultRootWindow(data.display);
    data.screen = XDefaultScreen(data.display);
    data.visual = XDefaultVisual(data.display, data.screen);
    XWindowAttributes x_attr;
 
    data.window = XCreateSimpleWindow(data.display,data.root,0,0,width, height,0,0,0);
    XStoreName(data.display,data.window, "killTHePlanet!");
    XGetWindowAttributes(data.display,DefaultRootWindow(data.display),&x_attr);
    Atom wmEvents[] = 
    {
        XInternAtom(data.display, "WM_DELETE_WINDOW", True),
    };    
    XSetWMProtocols(data.display, data.window, wmEvents, 1);
    int attr[] = 
    {
        GLX_USE_GL, True,
        GLX_RGBA,   True,
        GLX_DOUBLEBUFFER,
        GLX_BUFFER_SIZE,32,
        GLX_DEPTH_SIZE, 24,
        None
    };
    auto visual= glXChooseVisual(data.display, data.screen, attr);
    if(!visual)
        throw std::runtime_error("[GLX] unable to find visual");
    data.context = glXCreateContext(data.display, visual, NULL, True);
    if(!data.context)
        throw std::runtime_error("[GLX] unable to create window context");
    glXMakeCurrent(data.display, data.window, data.context);
    resize(width,height);
}
void engine::resize(int w, int h)
{
    _init();
    NFO("resize to %d x %d",width, height);
    XSetWindowAttributes attr; 
    std::memset(&attr,0,sizeof(attr));
    attr.event_mask = StructureNotifyMask|ButtonPressMask|ButtonReleaseMask|Button1MotionMask|KeyPressMask;
    attr.background_pixel   = 0xFFFF0000;

    XWithdrawWindow(data.display,data.window, data.screen);  
    XChangeWindowAttributes(data.display,data.window,CWBackPixel|CWOverrideRedirect|CWSaveUnder|CWEventMask|CWBorderPixel, &attr);
    XResizeWindow(data.display,data.window,width,height);
    XRaiseWindow(data.display, data.window);
    XMapWindow(data.display,data.window);
    XMoveWindow(data.display,data.window,0,0);
    XFlush(data.display);
}

bool engine::update()
{
    XEvent evt;
    for (int i = 0; i < XPending(data.display); i++)
    { 
        XNextEvent(data.display, &evt);
        switch (evt.type)
        {
            case KeyPress:
            {
                auto event = reinterpret_cast<XKeyEvent *>(&evt);
                if(onKey) onKey(event->keycode);
                break;
            }
            case ClientMessage:
                NFO("X11 NATIVE EVENT: QUIT");
                return false;
            default:
                break;
        }
    }
    _update();
    glXSwapBuffers(data.display,data.window);
    return true;
}


#endif