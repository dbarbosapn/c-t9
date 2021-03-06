#ifndef _CALLBACKS
#define _CALLBACKS

void on_button_pressed(GtkButton* button, ButtonData* data);
void on_button_released(GtkButton* button, ButtonData* data);
void on_delete_clicked(GtkButton* button, AppData* data);
void on_t9_switch(GObject* sw, GParamSpec* pspec, AppData* data);
void on_plus_clicked(GtkButton* button, AppData* data);
void on_zero_clicked(GtkButton* button, AppData* data);
void on_hash_clicked(GtkButton* button, AppData* data);

#endif
