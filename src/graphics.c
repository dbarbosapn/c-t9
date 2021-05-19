
//gcc -o graphics graphics.c `pkg-config --libs --cflags gtk+-3.0` && ./graphics


#include <gtk/gtk.h>


void onClickPrint(GtkApplication* app, gpointer user_data) {
     printf("Heelo: %c\n", *(char *)user_data );
}

int main(int argc, char *argv[]) {
     GtkWidget *window;
     GtkWidget *vbox;
     GtkWidget *vbox2;
     GtkWidget *button;
     gchar *values[12] = {
          "1", "2 abc", "3 def",
          "4 ghi", "5 jkl", "6 mno",
          "7 pqrs", "8 tuv", "9 wxyz",
          "* +", "0 __", "#"
     };


     gtk_init(&argc, &argv);

     //--------------------------Window----------------------------------
     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(window), "Center");
     gtk_window_set_default_size(GTK_WINDOW(window), 250, 350);
     gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
     gtk_container_set_border_width(GTK_CONTAINER(window), 20);

     vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
     gtk_container_add(GTK_CONTAINER(window), vbox);



     //--------------------------Text View----------------------------------

     GtkWidget *view;
     GtkTextBuffer *buffer;

     view = gtk_text_view_new ();

     buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

     gtk_text_buffer_set_text (buffer, "nothin", -1);
     // gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
     // gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);
     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);

     gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);


     //--------------------------Middle buttons----------------------------------

     vbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
     button = gtk_button_new_with_label("cycle");
     gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 0);
     button = gtk_button_new_with_label("delete");
     gtk_box_pack_end(GTK_BOX(vbox2), button, FALSE,TRUE, 0);

     gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE,FALSE, 8);


     //--------------------------Grid----------------------------------

     GtkWidget *grid;
     grid = gtk_grid_new();
     gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

     for (int pos=0,i=0; i < 8; i+=2) {
          for (int j=0; j < 12; j+=4) {
               button = gtk_button_new_with_label(values[pos]);
               g_signal_connect(button, "pressed", G_CALLBACK(onClickPrint), (void*)&(values[pos][0]));     //onClick event
               gtk_grid_attach(GTK_GRID(grid), button, j, i, 4, 2);
               pos++;
          }
     }
     gtk_widget_set_halign(vbox,GTK_ALIGN_CENTER);
     gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE,FALSE, 20);


     //--------------------------End----------------------------------
     gtk_widget_show_all(window);
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
     gtk_main();






     return 0;
}
