#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char *s;
    int i;

    if (! (s = calloc(100, sizeof(char))))
        return 1;

    fgets(s, 100, stdin);
    s[strcspn (s, "\n")] = 0;

    for (i = strlen(s); i >= 0; i--)
        printf("%c", s[i]);
    printf("\n");

    free(s);

    return 0;
}
