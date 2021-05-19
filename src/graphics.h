#ifndef _GRAPHICS
#define _GRAPHICS

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *view;
    GtkWidget *label;
    GtkWidget *buttons[14];
} Graphics;

Graphics graphics_init();

#endif