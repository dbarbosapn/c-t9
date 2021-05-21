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

double get_curr_time() {
     clock_t c = clock();
     return ((double) c / CLOCKS_PER_SEC)*10;
}


char *values[] = {",.?",  "abc", "def",  "ghi", "jkl", "mno",
"pqrs", "tuv", "wxyz", "*+",   " ",   "#"};

void on_click_not_t9(AppData *data) {
     double currTime = get_curr_time();
     int currButton = *data->currButton;
     float timeDiff = currTime - *data->lastClickTime;
     *data->lastClickTime = currTime;
     printf("%f\n", timeDiff);
     if((*data->lastButtonPressed == currButton) &&
     (timeDiff) < 2 ) { //time works in mysterious ways
          switch_last_char(data->gr->view, values[currButton]);
     }
     else {
          add_view_char(data->gr->view, values[currButton][0]);
     }
     *data->lastButtonPressed = currButton;

}

void on_click_t9(AppData *data) {
     //TODO
}

//crash a verificar
void on_button_clicked(GtkButton* button, AppData *data) {
     // if(*data->t9) {
          on_click_t9(data);
          // }
     // else {
          on_click_not_t9(data);
     // }
}

//gboolean state, dá crash
void on_switch(GtkSwitch * sw, int state, AppData *data) {
     gtk_switch_set_state(GTK_SWITCH(sw), state);
     int s = state;
     *data->t9 = s;
}

void on_delete_clicked(GtkButton* button, AppData *data) {
     remove_view_char(data->gr->view);
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
