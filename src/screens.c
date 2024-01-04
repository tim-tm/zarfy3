#include "screens.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/Xrandr.h>

screen* screens_init(void) {
    screen* head = NULL;

    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("No display in environment!");
        exit(EXIT_FAILURE);
    }
    XRRScreenResources* scr = XRRGetScreenResources(display, DefaultRootWindow(display));
 
    for (int i = 0; i < scr->noutput; ++i) {
        XRROutputInfo* out_info = XRRGetOutputInfo(display, scr, scr->outputs[i]);
        if (out_info != NULL && out_info->connection == RR_Connected) {
            XRRCrtcInfo* info = XRRGetCrtcInfo(display, scr, out_info->crtc);
        
            screen* s = malloc(sizeof(screen));
            if (s == NULL) {
                screens_destroy(head);
                return NULL;
            }
            s->name = out_info->name;
            s->x = info->x;
            s->y = info->y;
            s->width = info->width;
            s->height = info->height;
            
            s->next = head;
            head = s;
            XRRFreeCrtcInfo(info);
        }
        XRRFreeOutputInfo(out_info);
    }
    XRRFreeScreenResources(scr);
    XCloseDisplay(display);
    return head;
}

void screens_destroy(screen* list) {
    screen* itr = list;
    while (itr != NULL) {
        screen* next = itr->next;
        free(itr);
        itr = next;
    }
}
