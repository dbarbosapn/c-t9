#ifndef _GRAPHICS
#define _GRAPHICS

#include <gtk/gtk.h>

#include "linked_list.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *view;
    GtkWidget *label;
    GtkWidget *buttons[14];
} Graphics;

Graphics *graphics_init();

void add_view_char(GtkWidget *view, char ch, int is_diacritic);

void remove_view_char(GtkWidget *view);

void remove_view_last_word(GtkWidget *view);

void set_label_text(GtkWidget *label, char *text);

gchar *get_view_text(GtkWidget *view);

void switch_last_char(GtkWidget *view, char ch, int is_diacritic);

void fill_label(GtkWidget *label, Node *curr);

void add_view_word(GtkWidget *view, char *word);

void set_view_text(GtkWidget *view, char *text);

#endif
