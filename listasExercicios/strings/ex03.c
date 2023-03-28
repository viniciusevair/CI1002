#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char *string;
    int i;

    if (! (string = calloc(100, sizeof(char))))
        return 1;

    fgets(string, 100, stdin);
    string[strcspn (string, "\n")] = 0;

    for (i = strlen(string); i >= 0; i--)
        if (string[i] >= 'A' && string[i] <= 'Z')
            string[i] += 32;

    printf("%s\n", string);
    free(string);

    return 0;
}

