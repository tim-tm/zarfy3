#ifndef _SCREENS_H_
#define _SCREENS_H_

#include <X11/extensions/Xrandr.h>
#include <stddef.h>

typedef struct _mode_ {
    char *name;
    size_t width;
    size_t height;
    size_t id;
    float refresh_rate;

    struct _mode_ *next;
} mode;

typedef struct _screen_ {
    const char *name;
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    size_t mm_width;
    size_t mm_height;
    RRCrtc crtc;
    RRMode mode;
    Rotation rotation;
    RROutput *outputs;
    size_t noutput;
    mode *modes;
    
    struct _screen_ *next;
} screen;

screen *screens_init(void);
void screens_destroy(screen *list);

void screen_apply(screen *scr);

#endif // !_SCREENS_H_
