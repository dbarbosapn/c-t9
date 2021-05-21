#ifndef _GRAPHICS
#define _GRAPHICS

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *view;
    GtkWidget *label;
    GtkWidget *buttons[14];
} Graphics;

Graphics *graphics_init();

void add_view_char(GtkWidget *view, char ch);

gboolean remove_view_char(GtkWidget *view);

void set_label_text(GtkWidget *label, char *text);

gchar *get_view_text(GtkWidget *view);

#endif