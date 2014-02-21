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
    logger::info("create X11 native window");
    data.display = XOpenDisplay(NULL);
    if (!data.display) 
        throw std::runtime_error("can't open X11 display");

    data.root   = XDefaultRootWindow(data.display);
    data.screen = XDefaultScreen(data.display);
    data.visual = XDefaultVisual(data.display, data.screen);
    XWindowAttributes x_attr;
 
    data.window = XCreateSimpleWindow(data.display,data.root,0,0,_width, _height,0,0,0);
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
    resize();
}

void engine::resize()
{
    logger::info("resize to %d x %d",_width, _height);
    XSetWindowAttributes attr; 
    std::memset(&attr,0,sizeof(attr));
    attr.event_mask = StructureNotifyMask|ButtonPressMask|ButtonReleaseMask|Button1MotionMask|KeyPressMask;
    attr.background_pixel   = 0xFFFF0000;

    XWithdrawWindow(data.display,data.window, data.screen);  
    XChangeWindowAttributes(data.display,data.window,CWBackPixel|CWOverrideRedirect|CWSaveUnder|CWEventMask|CWBorderPixel, &attr);
    XResizeWindow(data.display,data.window,_width,_height);
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
            case ButtonPress:
            {
                auto event = reinterpret_cast<XButtonEvent *>(&evt);
                if(onMousePress)onMousePress(event->x, event->y);
                XSetInputFocus(data.display, data.window, RevertToNone, CurrentTime);
                break;
            }
            case ButtonRelease: 
            {
                auto event = reinterpret_cast<XButtonEvent *>(&evt);
                if(onMouseRelease) onMouseRelease(event->x, event->y);
                break;
            }
            case MotionNotify:
            {
                auto event = reinterpret_cast<XMotionEvent *>(&evt);
                if(onMousePress) onMousePress(event->x, event->y);
                break;
            }
            case ClientMessage:
                logger::info("X11 NATIVE EVENT: QUIT");
                return false;
            case MapNotify:
                logger::info("X11 NATIVE EVENT: MAPPING");
                glViewport(0, 0, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height));
                setBackColor(vec(0, 0, .1, 0));
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL); 
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            default:
                break;
        }
    }
    std::lock_guard<std::mutex> lock(aux::globalLocker);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(onUpdate) onUpdate();
    for(const auto &obj : renderQueue) 
        obj->render();
    glXSwapBuffers(data.display,data.window);
    return true;
}

void engine::setBackColor(const vec &c) {glClearColor(c.x, c.y,c.z,c.w);}

#endif