
//gcc -o graphics graphics.c `pkg-config --libs --cflags gtk+-3.0` && ./graphics


#include <gtk/gtk.h>
#include "graphics.h"

void onClickPrint(GtkApplication* app, gpointer user_data) {
     printf("Heelo: %c\n", *(char *)user_data );
}

void addChar() {

}


GtkWidget *createWindow() {
     GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
     gtk_window_set_title(GTK_WINDOW(window), "Center");
     gtk_window_set_default_size(GTK_WINDOW(window), 250, 400);
     gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
     gtk_container_set_border_width(GTK_CONTAINER(window), 20);
     return window;
}

GtkWidget *createVerticalBox() {
     GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
     gtk_widget_set_halign(vbox,GTK_ALIGN_CENTER);
     return vbox;
}

GtkWidget *createView() {
     GtkWidget *view = gtk_text_view_new ();
     gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
     gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);
     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);
     return view;
}


GtkWidget *createLabel() {
     GtkWidget *label = gtk_label_new(NULL);
     return label;
}

GtkWidget *createMiddleButtons(GtkWidget *buttons[]) {
     GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
     GtkWidget *button = gtk_button_new_with_label("cycle");
     gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, TRUE, 0);
     buttons[12] = button;
     button = gtk_button_new_with_label("delete");
     gtk_box_pack_end(GTK_BOX(vbox), button, FALSE,TRUE, 0);
     buttons[13] = button;
     return vbox;
}

GtkWidget *createButtonGrid(gchar *values[], GtkWidget *buttons[]) {
     GtkWidget *grid = gtk_grid_new();
     GtkWidget *button;
     gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
     gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

     for (int pos=0,i=0; i < 8; i+=2) {
          for (int j=0; j < 12; j+=4) {
               button = gtk_button_new_with_label(values[pos]);
               buttons[pos]=button;
               g_signal_connect(button, "pressed", G_CALLBACK(onClickPrint), (void*)&(values[pos][0]));     //onClick event
               gtk_grid_attach(GTK_GRID(grid), button, j, i, 4, 2);
               pos++;
          }
     }
     return grid;
}

//SIZE LIMIT: 26
void labelSetText(GtkWidget *label, char *text) {

     if(strlen(text)>26) {
          text = "Err: buffer to long";
     }
     gtk_label_set_text(GTK_LABEL(label),text);
}

void setViewText(GtkWidget *view, char *text) {
     GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
     gtk_text_buffer_set_text (buffer, text, -1);
}

void addViewChar(GtkWidget *view, char ch) {
     GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
     GtkTextIter iter;
     gtk_text_buffer_get_end_iter(buffer, &iter);
     char str[1];
     str[0]=ch;
     gtk_text_buffer_insert(buffer,&iter,str,1 );
}

void backspaceView(GtkWidget *view) {
     GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
     GtkTextIter iter;
     gtk_text_buffer_get_end_iter(buffer, &iter);
     gtk_text_buffer_backspace(buffer,&iter,1,1 );
}


Graphics graphics_init() {
     GtkWidget *window;
     GtkWidget *vbox;
     GtkWidget *vbox2;
     GtkWidget *grid;
     GtkWidget *view;
     GtkWidget *label;
     GtkWidget *buttons[14];
     gchar *values[12] = {
          "1\n,.?", "2\nabc", "3\ndef",
          "4\nghi", "5\njkl", "6\nmno",
          "7\npqrs", "8\ntuv", "9\nwxyz",
          "* +", "0_", "#"
     };



     //--------------------------Window----------------------------------
     window = createWindow();

     vbox = createVerticalBox();
     gtk_container_add(GTK_CONTAINER(window), vbox);



     //--------------------------Text View----------------------------------


     view = createView();
     gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);



     //--------------------------Label----------------------------------
     label = createLabel();
     gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);


     //--------------------------Middle buttons----------------------------------

     vbox2 = createMiddleButtons(buttons);
     gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE,FALSE, 8);


     //--------------------------Grid----------------------------------

     grid = createButtonGrid(values,buttons);
     gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE,FALSE, 20);



     //--------------------------End----------------------------------
     gtk_widget_show_all(window);
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


     //--------------------------Preping Struct----------------------------------
     Graphics gr;
     gr.view = view;
     gr.label = label;
     for(int i=0; i<14;i++) {
          gr.buttons[i] = buttons[i];
     }


     //--------------------------Testing Buffer Functions----------------------------------
     setViewText(view, "uwot?! D:<");
     addViewChar(view,'\n');
     addViewChar(view,'U');
     addViewChar(view,'F');
     backspaceView(view);
     labelSetText(label, "ola alo hey");

     // char str[1] = "a";
     // for(int i=0; i<14;i++) {
     //      gtk_button_set_label(GTK_BUTTON(gr.buttons[i]),str);
     //      str[0]++;
     // }


     return gr;
}
