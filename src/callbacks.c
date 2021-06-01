#include <t9_keys.h>
#include <trie.h>

#include "app_data.h"
#include "graphics.h"

const char* values[] = {",.?",  "abc", "def",  "ghi", "jkl", "mno",
                        "pqrs", "tuv", "wxyz", "*+",  " ",   "#"};

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
        fill_label(data->gr->label, data->cur_node);
    } else {
        gtk_label_set_markup(GTK_LABEL(data->gr->label),
                             "<u>Add to dictionary</u>");
    }
}

char* get_semi_word(Node* curr, size_t size) {
    if (curr == NULL) return NULL;
    if (curr->value == NULL) return NULL;
    char* curr_word = curr->value;
    char* word = (char*)malloc(sizeof(char) * size + 1);
    for (int i = 0; i < size; i++) {
        word[i] = curr_word[i];
    }
    word[size] = '\0';
    return word;
}

void update_view(AppData* data) {
    if (data->t9_words == NULL || data->cur_node == NULL) return;

    size_t size = strlen(data->t9_buffer);

    if (size > 0) remove_view_last_word(data->gr->view);

    char* word = get_semi_word(data->cur_node, size);

    add_view_word(data->gr->view, word);
    free(word);
}

void t9_cycle(AppData* data) {
    if (data->cur_node == NULL) {
        data->cur_node = data->t9_words;
    } else {
        data->cur_node = data->cur_node->next;
    }
    update_labels_view(data);
    update_view(data);
}

void t9_update(AppData* data) {
    free_t9_list(data->t9_words);
    data->t9_words = run_t9(data->trie, data->ht, data->t9_buffer);
    data->cur_node = data->t9_words;
    update_labels_view(data);
    update_view(data);
}

void reset_t9(AppData* data) {
    data->t9_buffer[0] = '\0';
    set_label_text(data->gr->label, "");
    free_t9_list(data->t9_words);
    data->t9_words = NULL;
    data->cur_node = NULL;
}

void t9_select(AppData* data) {
    if (data->cur_node == NULL && strlen(data->t9_buffer) != 0) {
        data->adding_word = 1;
        data->t9_mode = 0;
        remove_view_last_word(data->gr->view);
        data->saved_text = get_view_text(data->gr->view);
        set_view_text(data->gr->view, "");
        set_label_text(data->gr->label, "Press # when done");
    }
    if (data->cur_node == NULL || data->cur_node->value == NULL) return;
    remove_view_last_word(data->gr->view);
    char* word = data->cur_node->value;
    add_view_word(data->gr->view, word);
    hashtable_put(data->ht, word, hashtable_get(data->ht, word) + 1);
    add_view_char(data->gr->view, ' ');

    reset_t9(data);
}

void on_click_t9(int button, AppData* data) {
    time_t curr_time = time(NULL);
    data->last_click_time = curr_time;

    size_t len = strlen(data->t9_buffer);
    if (button <= 8) {
        data->t9_buffer[len] = '1' + button;
        data->t9_buffer[len + 1] = '\0';
        t9_update(data);
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

    if (!app_data->t9_mode && !app_data->adding_word && time_diff > 0.5) {
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
    if (!data->t9_mode) {
        set_label_text(data->gr->label, "");
        reset_t9(data);
    }
}

void on_delete_clicked(GtkButton* button, AppData* data) {
    remove_view_char(data->gr->view);

    size_t len = strlen(data->t9_buffer);
    if (data->t9_mode && len > 1) {
        data->t9_buffer[len - 1] = '\0';
        t9_update(data);
    } else if (data->t9_mode) {
        reset_t9(data);
    }
}

void on_plus_clicked(GtkButton* button, AppData* data) {
    if (data->t9_mode) {
        t9_cycle(data);
    } else if (!data->adding_word) {
        on_click_not_t9(9, data);
    }
}

void on_zero_clicked(GtkButton* button, AppData* data) {
    if (data->t9_mode && data->cur_node != NULL) {
        t9_select(data);
    } else if (!data->adding_word) {
        on_click_not_t9(10, data);
    }
}

void on_hash_clicked(GtkButton* button, AppData* data) {
    if (data->t9_mode) {
        t9_select(data);
    } else if (data->adding_word) {
        data->adding_word = 0;
        data->t9_mode = 1;
        reset_t9(data);
        char* new_word = get_view_text(data->gr->view);
        insert_word(data->trie, new_word);
        hashtable_put(data->ht, new_word, 1);
        set_view_text(data->gr->view, data->saved_text);
        add_view_word(data->gr->view, new_word);
        add_view_char(data->gr->view, ' ');
        set_label_text(data->gr->label, "");
        free(new_word);
        free(data->saved_text);
        data->saved_text = NULL;
    } else {
        on_click_not_t9(11, data);
    }
}
