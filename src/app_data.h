#ifndef _APP_DATA
#define _APP_DATA

#include <trie.h>

#include "graphics.h"

typedef struct {
    Graphics* gr;
    HashTable* ht;
    TrieNode* trie;
    int curr_button;
    int last_button_pressed;
    int last_char_index;
    time_t last_click_time;
    gboolean t9_mode;
    gboolean adding_to_dict;
    char *prev_text;

    char* t9_buffer;
    Node* t9_words;
    Node* cur_node;
} AppData;

typedef struct {
    int button;
    AppData* app_data;
} ButtonData;

#endif
