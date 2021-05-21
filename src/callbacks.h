#ifndef _CALLBACKS
#define _CALLBACKS

void on_button_clicked(GtkButton* button, gpointer param);
void on_button_0_clicked(GtkButton* button, gpointer param);
void on_button_r_clicked(GtkButton* button, gpointer param);
void on_delete_clicked(GtkButton* button, gpointer param);
void on_switch(GtkSwitch * sw, gboolean *state, AppData *data);

#endif
