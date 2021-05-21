#include <gtk/gtk.h>
#include <int_hashtable.h>
#include <simple_input.h>
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

AppData *copy_data(AppData *data) {
     AppData* newdata = (AppData*)malloc(sizeof(AppData));
     newdata->gr = data->gr;
     newdata->trie = data->trie;
     newdata->ht = data->ht;
     newdata->currButton = data->currButton;
     newdata->lastButtonPressed = data->lastButtonPressed;
     newdata->lastClickTime = data->lastClickTime;
     return newdata;
}

void setup_callbacks(AppData* data) {
     for(int i=0; i<11;i++) {
          AppData *newData = copy_data(data);
          newData->currButton = (int *)malloc(sizeof(int));
          *newData->currButton = i;
          g_signal_connect(G_OBJECT(data->gr->buttons[i]), "clicked", G_CALLBACK(on_button_clicked), newData);
     }

    g_signal_connect(G_OBJECT(data->gr->buttons[12]), "state_set",
                     G_CALLBACK(on_switch), data);
    g_signal_connect(G_OBJECT(data->gr->buttons[13]), "clicked",
                     G_CALLBACK(on_delete_clicked), data);
    // TODO: Use the "pressed" and "released" signals (with time.h) to check
    // if button is being held for > 1 second. In that case, use the number,
    // not the values (specific callback for that).
}

int main(int argc, char* argv[]) {
    TrieNode* trie = load_trie();
    HashTable* ht = load_hashtable();

    gtk_init(&argc, &argv);
    Graphics* gr = graphics_init();
    AppData* data = (AppData*)malloc(sizeof(AppData));
    data->gr = gr;
    data->trie = trie;
    data->ht = ht;
    data->lastButtonPressed = (int *)malloc(sizeof(int));
    *data->lastButtonPressed = -1;
    data->lastClickTime = (int *)malloc(sizeof(int));
    *data->lastClickTime = 0;
    data->t9 = (int *)malloc(sizeof(int));
    *data->t9 = 0;
    setup_callbacks(data);
    gtk_main();

    return 0;
}
