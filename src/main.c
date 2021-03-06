#include <gtk/gtk.h>
#include <int_hashtable.h>
#include <stdio.h>
#include <string.h>
#include <stringutils.h>
#include <t9_keys.h>
#include <trie.h>
#include <unistd.h>

#include "app_data.h"
#include "callbacks.h"
#include "graphics.h"

int file_exists(char* filename) { return access(filename, F_OK) == 0; }

/**
 * Loads the trie from file. If not available, creates a new one from the
 * dictionary
 **/
TrieNode* load_trie() {
    if (!file_exists("data/trie.bin")) {
        FILE* dict = fopen("data/lusiadas.txt", "r");
        TrieNode* result = create_trie_node();
        trie_load_dict(result, dict);
        fclose(dict);
        return result;
    } else {
        FILE* fp = fopen("data/trie.bin", "rb");
        TrieNode* trie = trie_load(fp);
        fclose(fp);
        return trie;
    }
}

/**
 * Loads the hashtable from file. If not available, creates a new one from the
 * dictionary
 **/
HashTable* load_hashtable() {
    if (!file_exists("data/hashtable.bin")) {
        FILE* dict = fopen("data/lusiadas.txt", "r");
        HashTable* result = hashtable_create();

        char buffer[64];
        while (fscanf(dict, "%s", buffer) != EOF) {
            str_normalize(buffer);
            if (str_scan(buffer, 64)) {
                int curr_val = hashtable_get(result, buffer);
                if (curr_val == -1) curr_val = 0;
                hashtable_put(result, buffer, curr_val + 1);
            }
            buffer[0] = '\0';
        }
        fclose(dict);

        return result;
    } else {
        FILE* fp = fopen("data/hashtable.bin", "rb");
        HashTable* result = hashtable_load(fp);
        fclose(fp);

        return result;
    }
}

void save_trie(TrieNode* trie) {
    FILE* fp = fopen("data/trie.bin", "wb");
    trie_save(trie, fp);
    fclose(fp);
}

void save_hashtable(HashTable* ht) {
    FILE* fp = fopen("data/hashtable.bin", "wb");
    hashtable_save(ht, fp);
    fclose(fp);
}

void on_quit(GtkWidget* window, AppData* data) {
    save_trie(data->trie);
    save_hashtable(data->ht);
    gtk_main_quit();
}

void setup_callbacks(AppData* data) {
    for (int i = 0; i < 12; i++) {
        ButtonData* bdata = (ButtonData*)malloc(sizeof(ButtonData));
        bdata->button = i;
        bdata->app_data = data;
        if (i < 9) {
            g_signal_connect(G_OBJECT(data->gr->buttons[i]), "pressed",
                             G_CALLBACK(on_button_pressed), bdata);
        }
        g_signal_connect(G_OBJECT(data->gr->buttons[i]), "released",
                         G_CALLBACK(on_button_released), bdata);
    }

    g_signal_connect(G_OBJECT(data->gr->buttons[9]), "pressed",
                     G_CALLBACK(on_plus_clicked), data);
    g_signal_connect(G_OBJECT(data->gr->buttons[10]), "pressed",
                     G_CALLBACK(on_zero_clicked), data);
    g_signal_connect(G_OBJECT(data->gr->buttons[11]), "pressed",
                     G_CALLBACK(on_hash_clicked), data);

    g_signal_connect(G_OBJECT(data->gr->buttons[12]), "notify::active",
                     G_CALLBACK(on_t9_switch), data);
    g_signal_connect(G_OBJECT(data->gr->buttons[13]), "clicked",
                     G_CALLBACK(on_delete_clicked), data);

    g_signal_connect(G_OBJECT(data->gr->window), "destroy", G_CALLBACK(on_quit),
                     data);
}

void initialize_data(AppData* data, Graphics* gr, TrieNode* trie,
                     HashTable* ht) {
    data->gr = gr;
    data->trie = trie;
    data->ht = ht;
    data->last_button_pressed = -1;
    data->last_click_time = 0;
    data->t9_mode = 0;
    data->t9_buffer = (char*)malloc(sizeof(char) * 30);
    data->t9_buffer[0] = '\0';
    data->cur_node = NULL;
    data->t9_words = NULL;
    data->adding_word = 0;
    data->saved_text = NULL;
}

int main(int argc, char* argv[]) {
    TrieNode* trie = load_trie();
    HashTable* ht = load_hashtable();

    gtk_init(&argc, &argv);
    Graphics* gr = graphics_init();
    AppData* data = (AppData*)malloc(sizeof(AppData));
    initialize_data(data, gr, trie, ht);
    setup_callbacks(data);
    gtk_main();

    return 0;
}
