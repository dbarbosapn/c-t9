#include "graphics.h"

#define MAX_LENGTH_LABEL 20

GtkWidget *create_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LABP - T9");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 400);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    return window;
}

GtkWidget *create_vertical_box() {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    return vbox;
}

GtkWidget *create_view() {
    GtkWidget *view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);
    return view;
}

GtkWidget *create_label() {
    GtkWidget *label = gtk_label_new(NULL);
    return label;
}

GtkWidget *create_phone_buttons(GtkWidget *buttons[]) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

    GtkWidget *sw = gtk_switch_new();
    gtk_switch_set_active(GTK_SWITCH(sw), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), sw, FALSE, TRUE, 0);
    buttons[12] = sw;

    GtkWidget *button = gtk_button_new_with_label("DEL");
    gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, TRUE, 0);
    buttons[13] = button;
    return vbox;
}

GtkWidget *create_button_grid(gchar *numbers[], gchar *values[],
                              GtkWidget *buttons[]) {
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *button;
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    for (int pos = 0, i = 0; i < 8; i += 2) {
        for (int j = 0; j < 12; j += 4) {
            button = gtk_button_new();
            GtkWidget *label1 = gtk_label_new(numbers[pos]);
            if (strlen(values[pos]) > 0) {
                GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
                GtkWidget *label2 = gtk_label_new(values[pos]);
                gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, TRUE, 0);
                gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, TRUE, 0);
                gtk_container_add(GTK_CONTAINER(button), vbox);
            } else {
                gtk_container_add(GTK_CONTAINER(button), label1);
            }

            buttons[pos] = button;
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 4, 2);
            pos++;
        }
    }
    return grid;
}

void set_label_text(GtkWidget *label, char *text) {
    if (strlen(text) > MAX_LENGTH_LABEL) {
        text = "Error: text too long";
    }
    gtk_label_set_text(GTK_LABEL(label), text);
}

void set_view_text(GtkWidget *view, char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, text, -1);
}

gchar *get_view_text(GtkWidget *view) {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

void switch_last_char(GtkWidget *view, char ch) {
    remove_view_char(view);
    add_view_char(view, ch);
}

void add_view_char(GtkWidget *view, char ch) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    char str[1];
    str[0] = ch;
    gtk_text_buffer_insert(buffer, &iter, str, 1);
}

void remove_view_char(GtkWidget *view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_backspace(buffer, &iter, 1, 1);
}

void remove_view_last_word(GtkWidget *view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_iter_backward_char(&iter);
    char ch = gtk_text_iter_get_char(&iter);
    while (ch != ' ' && ch != 0) {
        gtk_text_iter_forward_char(&iter);
        gtk_text_buffer_backspace(buffer, &iter, 1, 1);
        gtk_text_iter_backward_char(&iter);
        ch = gtk_text_iter_get_char(&iter);
    }
}

void fill_label(GtkWidget *label, Node *curr) {
    char str[33];
    char *word;
    size_t size, totalSize = 0;
    int i = 0;

    str[i] = '<';
    str[i + 1] = 'u';
    str[i + 2] = '>';
    i = 3;
    int first = 1;

    while (curr != NULL) {
        word = (char *)curr->value;
        size = strlen(word);
        totalSize += size + 2;

        if (totalSize > MAX_LENGTH_LABEL) {
            break;
        } else {
            for (int j = 0; j < size; j++, i++) {
                str[i] = word[j];
            }

            if (first) {
                str[i] = '<';
                str[i + 1] = '/';
                str[i + 2] = 'u';
                str[i + 3] = '>';
                i += 4;
                first = 0;
            }

            str[i] = ' ';
            str[i + 1] = ' ';
            str[i + 2] = '\0';
            i += 2;
        }
        curr = curr->next;
    }
    gtk_label_set_markup(GTK_LABEL(label), str);
}

void add_view_word(GtkWidget *view, char *word) {
    if (word == NULL) return;
    size_t size = strlen(word);
    for (int i = 0; i < size; i++) {
        add_view_char(view, word[i]);
    }
}

Graphics *graphics_init() {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *vbox2;
    GtkWidget *grid;
    GtkWidget *view;
    GtkWidget *label;
    GtkWidget *buttons[14];
    gchar *numbers[12] = {"1", "2", "3", "4", "5", "6",
                          "7", "8", "9", "*", "0", "#"};
    gchar *values[12] = {",.?",  "abc", "def",  "ghi", "jkl", "mno",
                         "pqrs", "tuv", "wxyz", "+",   "_",   ""};

    //-------------------------- Window ----------------------------------
    window = create_window();

    vbox = create_vertical_box();
    gtk_container_add(GTK_CONTAINER(window), vbox);

    //-------------------------- Text View ----------------------------------

    view = create_view();
    gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);

    //-------------------------- Label ----------------------------------
    label = create_label();
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    //-------------------------- Phone Buttons --------------------------

    vbox2 = create_phone_buttons(buttons);
    gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE, FALSE, 8);

    //-------------------------- Grid ----------------------------------

    grid = create_button_grid(numbers, values, buttons);
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 20);

    //-------------------------- End ----------------------------------
    gtk_widget_show_all(window);

    //-------------------------- Return Struct -----------------------
    Graphics *gr = (Graphics *)malloc(sizeof(Graphics));
    gr->window = window;
    gr->view = view;
    gr->label = label;
    for (int i = 0; i < 14; i++) {
        gr->buttons[i] = buttons[i];
    }

    return gr;
}
