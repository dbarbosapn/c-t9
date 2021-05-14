
//gcc -o graphics graphics.c `pkg-config --libs --cflags gtk+-3.0` && ./graphics


#include <gtk/gtk.h>


int main(int argc, char *argv[]) {
     GtkWidget *window;
     GtkWidget *fixed;
     GtkWidget *btn1;
     GtkWidget *btn2;
     GtkWidget *btn3;
     GtkWidget *vbox;
     GtkWidget *vbox2;
     GtkWidget *one;
     GtkWidget *two;
     GtkWidget *three;
     GtkWidget *button;
     gtk_init(&argc, &argv);

     gchar *values[12] = {
          "1", "2 abc", "3 def",
          "4 ghi", "5 jkl", "6 mno",
          "7 pqrs", "8 tuv", "9 wxyz",
          "* +", "0 __", "#"
     };

     window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(window), "Center");
     gtk_window_set_default_size(GTK_WINDOW(window), 250, 350);
     gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
     gtk_container_set_border_width(GTK_CONTAINER(window), 20);

     vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
     gtk_container_add(GTK_CONTAINER(window), vbox);




     GtkWidget *view;
     GtkTextBuffer *buffer;

     view = gtk_text_view_new ();

     buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

     gtk_text_buffer_set_text (buffer, "nothin", -1);
     // gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
     // gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);
     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);

     gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);



     vbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
     button = gtk_button_new_with_label("cycle");
     gtk_box_pack_start(GTK_BOX(vbox2), button, FALSE, TRUE, 0);
     button = gtk_button_new_with_label("delete");
     gtk_box_pack_end(GTK_BOX(vbox2), button, FALSE,TRUE, 0);

     gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE,FALSE, 8);



     GtkWidget *grid;
     grid = gtk_grid_new();
     gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

     for (int pos=0,i=0; i < 8; i+=2) {
          for (int j=0; j < 12; j+=4) {
               button = gtk_button_new_with_label(values[pos]);
               gtk_grid_attach(GTK_GRID(grid), button, j, i, 4, 2);
               pos++;
          }
     }
     gtk_widget_set_halign(vbox,GTK_ALIGN_CENTER);
     gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE,FALSE, 20);





     gtk_widget_show_all(window);
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

     gtk_main();

     return 0;
}
