#ifndef _CALLBACKS
#define _CALLBACKS

void on_button_clicked(GtkButton* button, gpointer param);
void on_delete_clicked(GtkButton* button, gpointer param);
void on_t9_switch(GObject* sw, GParamSpec* pspec, AppData* data);

#endif
