#include <t9_keys.h>
#include <trie.h>

#include "app_data.h"
#include "graphics.h"

/**
 * Runs the T9 algorithm. Returns the ordered list of words
 **/
Node* run_t9(TrieNode* trie, HashTable* ht, char* input) {
    Node* permutations = get_permutations(input);
    Node* curr = permutations;

    Node* result = NULL;

    while (curr != NULL) {
        char* prefix = curr->value;

        Node* new = prefix_search(trie, prefix, ht, result);

        if (new != NULL) result = new;

        free(prefix);
        Node* prev = curr;
        curr = curr->next;
        free(prev);
    }

    return result;
}

void free_t9_list(Node* list) {
    Node* curr = list;
    while (curr != NULL) {
        free(curr->value);

        Node* prev = curr;
        curr = curr->next;
        free(prev);
    }
}

// TODO: This is just for testing. We should keep track of the characters added,
// not read the view each time (e.g. on_delete_char and on_added_char)
void update_label(AppData* data) {
    char* text = get_view_text(data->gr->view);

    if (strlen(text) == 0) {
        set_label_text(data->gr->label, "");
        return;
    }

    Node* list = run_t9(data->trie, data->ht, text);
    int counter = 3;
    Node* curr = list;
    char buffer[26] = "";
    while (curr != NULL && counter > 0) {
        strcat(buffer, curr->value);
        counter--;
        if (counter > 0) strcat(buffer, ",");
        curr = curr->next;
    }
    free_t9_list(list);
    set_label_text(data->gr->label, buffer);
}

void on_button_1_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '1');
    update_label(data);
}

void on_button_2_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '2');
    update_label(data);
}

void on_button_3_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '3');
    update_label(data);
}

void on_button_4_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '4');
    update_label(data);
}

void on_button_5_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '5');
    update_label(data);
}

void on_button_6_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '6');
    update_label(data);
}

void on_button_7_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '7');
    update_label(data);
}

void on_button_8_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '8');
    update_label(data);
}

void on_button_9_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '9');
    update_label(data);
}

void on_button_l_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), 'l');
    update_label(data);
}

void on_button_0_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), '0');
    update_label(data);
}

void on_button_r_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    add_view_char(GTK_WIDGET(data->gr->view), 'r');
    update_label(data);
}

void on_button_delete_clicked(GtkButton* button, gpointer param) {
    AppData* data = (AppData*)param;
    remove_view_char(GTK_WIDGET(data->gr->view));
    update_label(data);
}