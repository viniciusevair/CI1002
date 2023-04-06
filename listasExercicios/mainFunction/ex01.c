#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int count;

    count = 0;
    for (int i = 0; i < argc; i++)
        if (argv[i][0] == '-')
            count++;

    return count;
}
