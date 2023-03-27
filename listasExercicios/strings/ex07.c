#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void adicionaColchetes (char *s) {
}

void adicionaCaractere (char *s, int ini) {
    int i, j;

    adicionaColchetes(s);

    for (i = strlen(s); i >= ini; i--) {
            
    }
}

int main () {
    char *string;
    int i;

    if (! (string = calloc(10000, sizeof(char))))
        return 1;

    fgets(string, 100, stdin);
    string[strcspn (string, "\n")] = 0;

    for (i = 0; i < strlen(string); i++) {
        if ((string[i] < 'a' || string[i] > 'z') ) {
        
        }
    }

    free(string);

    return 0;
}

