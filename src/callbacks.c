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

char* values[] = {",.?",  "abc", "def",  "ghi", "jkl", "mno",
                  "pqrs", "tuv", "wxyz", "*+",  " ",   "#"};

void on_click_not_t9(int button, AppData* data) {
    time_t curr_time = time(NULL);
    double time_diff = difftime(curr_time, data->last_click_time);

    data->last_click_time = curr_time;

    int char_index = 0;

    if (data->last_button_pressed == button && time_diff <= 1) {
        char_index = (data->last_char_index + 1) % strlen(values[button]);
        switch_last_char(data->gr->view, values[button][char_index]);
    } else {
        add_view_char(data->gr->view, values[button][0]);
    }

    data->last_button_pressed = button;
    data->last_char_index = char_index;
}

void on_click_t9(int button, AppData* data) {
    // TODO
}

void on_button_clicked(GtkButton* button, ButtonData* data) {
    if (data->app_data->t9_mode) {
        on_click_t9(data->button, data->app_data);
    } else {
        on_click_not_t9(data->button, data->app_data);
    }
}

void on_t9_switch(GObject* sw, GParamSpec* pspec, AppData* data) {
    data->t9_mode = gtk_switch_get_active(GTK_SWITCH(sw));
}

void on_delete_clicked(GtkButton* button, AppData* data) {
    remove_view_char(data->gr->view);
}