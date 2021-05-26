#ifndef _CALLBACKS
#define _CALLBACKS

void on_button_clicked(GtkButton* button, ButtonData* data);
void on_delete_clicked(GtkButton* button, AppData* data);
void on_t9_switch(GObject* sw, GParamSpec* pspec, AppData* data);

#endif
