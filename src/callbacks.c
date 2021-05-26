#include <t9_keys.h>
#include <trie.h>

#include "app_data.h"
#include "graphics.h"

const char* values[] = {",.?",  "abc", "def",  "ghi", "jkl", "mno",
                        "pqrs", "tuv", "wxyz", "*+",  " ",   "#"};

/**
 * Runs the T9 algorithm. Returns the ordered list of words
 * TODO: This should be optimized. The permutations should be passed instead of
 * the input
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

void update_labels_view(AppData* data) {
    if (data->cur_node != NULL) {
        // TODO: Update labels and view
    } else {
        // TODO: Update labels with "Add to
        // Dictionary"
    }
}

void t9_cycle(AppData* data) {
    if (data->cur_node != NULL)
        data->cur_node = data->cur_node->next;
    else
        data->cur_node = data->t9_words;

    update_labels_view(data);
}

void on_click_t9(int button, AppData* data) {
    time_t curr_time = time(NULL);
    data->last_click_time = curr_time;

    size_t len = strlen(data->t9_buffer);
    if (button <= 8) {
        data->t9_buffer[len] = '1' + button;
        data->t9_buffer[len + 1] = '\0';
        free_t9_list(data->t9_words);
        data->t9_words = run_t9(data->trie, data->ht, data->t9_buffer);
        data->cur_node = data->t9_words;
        update_labels_view(data);
    } else if (button == 9) {
        t9_cycle(data);
    } else if (button == 10) {
        add_view_char(data->gr->view, ' ');
        data->t9_buffer[0] = '\0';
        free_t9_list(data->t9_words);
        data->t9_words = NULL;
        data->cur_node = NULL;
    }
}

void on_button_pressed(GtkButton* button, ButtonData* data) {
    if (data->app_data->t9_mode) {
        on_click_t9(data->button, data->app_data);
    } else {
        on_click_not_t9(data->button, data->app_data);
    }
}

void on_button_released(GtkButton* button, ButtonData* data) {
    int btn = data->button;
    AppData* app_data = data->app_data;

    time_t curr_time = time(NULL);
    double time_diff = difftime(curr_time, app_data->last_click_time);

    if (!app_data->t9_mode && time_diff > 0.5) {
        if (btn <= 8)
            switch_last_char(app_data->gr->view, '1' + btn);
        else if (btn == 9)
            add_view_char(app_data->gr->view, '*');
        else if (btn == 10)
            switch_last_char(app_data->gr->view, '0');
    }
}

void on_t9_switch(GObject* sw, GParamSpec* pspec, AppData* data) {
    data->t9_mode = gtk_switch_get_active(GTK_SWITCH(sw));
}

void on_delete_clicked(GtkButton* button, AppData* data) {
    remove_view_char(data->gr->view);

    size_t len = strlen(data->t9_buffer);
    if (data->t9_mode && len > 0) {
        data->t9_buffer[len - 1] = '\0';
    }
}