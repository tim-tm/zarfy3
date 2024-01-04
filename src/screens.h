#ifndef _SCREENS_H_
#define _SCREENS_H_

#include <stddef.h>

typedef struct _screen_ {
    const char* name;
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    // TODO: Save possible resolutions for selection

    struct _screen_* next;
} screen;

screen* screens_init(void);
void screens_destroy(screen* list);

#endif // !_SCREENS_H_
