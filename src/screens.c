#include "screens.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/Xrandr.h>
#include <string.h>

static Display *display;
static Window root_window;
static XRRScreenResources *xrr_scres;

screen *screens_init(void) {
    screen *head = NULL;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "No display in environment!");
        exit(EXIT_FAILURE);
    }
    int unused;
    if (!XRRQueryExtension(display, &unused, &unused) || !XRRQueryVersion(display, &unused, &unused)) {
        fprintf(stderr, "RandR not found!");
        exit(EXIT_FAILURE);
    }
    
    root_window = DefaultRootWindow(display);
    xrr_scres = XRRGetScreenResources(display, root_window);

    for (int i = 0; i < xrr_scres->noutput; ++i) {
        XRROutputInfo *out_info = XRRGetOutputInfo(display, xrr_scres, xrr_scres->outputs[i]);
        if (out_info != NULL && out_info->connection == RR_Connected) {
            XRRCrtcInfo *info = XRRGetCrtcInfo(display, xrr_scres, out_info->crtc);
            
            screen *s = calloc(sizeof(screen), 1);
            if (s == NULL) {
                screens_destroy(head);
                return NULL;
            }
            s->name = out_info->name;
            s->x = info->x;
            s->y = info->y;
            s->width = info->width;
            s->height = info->height;
            s->mm_width = out_info->mm_width;
            s->mm_height = out_info->mm_height;
            s->crtc = out_info->crtc;
            s->mode = info->mode;
            s->rotation = info->rotation;
            s->outputs = info->outputs;
            s->noutput = info->noutput;

            for (int j = 0; j < out_info->nmode; ++j) {
                for (int k = 0; k < xrr_scres->nmode; ++k) {
                    XRRModeInfo m = xrr_scres->modes[k];
                    if (out_info->modes[j] == m.id) {
                        mode *mod = calloc(sizeof(mode), 1);
                        if (mod == NULL) {
                            screens_destroy(head);
                            return NULL;
                        }
                        mod->name = calloc(sizeof(char), 256);
                        if (mod->name == NULL) {
                            screens_destroy(head);
                            return NULL;
                        }
                        strncpy(mod->name, m.name, 256);
                        
                        mod->id = m.id;
                        mod->width = m.width;
                        mod->height = m.height;
                        mod->refresh_rate = (float)m.dotClock/((float)m.hTotal*(float)m.vTotal);

                        mod->next = s->modes;
                        s->modes = mod;
                    }
                }
            }

            s->next = head;
            head = s;
            XRRFreeCrtcInfo(info);
        }
        XRRFreeOutputInfo(out_info);
    }
    return head;
}

void screens_destroy(screen *list) {
    XRRFreeScreenResources(xrr_scres);
    XCloseDisplay(display);
    screen *itr = list;
    while (itr != NULL) {
        screen *next = itr->next;
        if (next != NULL) {
            mode *m_itr = next->modes;
            while (m_itr != NULL) {
                mode *m_next = m_itr->next;
                free(m_itr->name);
                free(m_itr);
                m_itr = m_next;
            }
        }
        free(itr);
        itr = next;
    }
}

void screen_apply(screen *scr) {
    if (scr == NULL) {
        return;
    }
    XGrabServer(display);
    XRRSetScreenSize(display, root_window, scr->width, scr->height, scr->mm_width, scr->mm_height);
    if (XRRSetCrtcConfig(display, xrr_scres, scr->crtc, CurrentTime, scr->x, scr->y, scr->mode, scr->rotation, scr->outputs, scr->noutput) != RRSetConfigSuccess) {
        fprintf(stderr, "Failed to set crtc config.");
        exit(1);
    }
    XUngrabServer(display);
    XSync(display, 0);
}
