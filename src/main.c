#include <stdio.h>
#include <gtk/gtk.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
    // TODO: If there are no data files (with the hash table and the trie
    // saved), we will load the data from lusiadas.txt

    gtk_init(&argc, &argv);
    Graphics gr = graphics_init();
    gtk_main();
    return 0;
}
