int compara(const void *a, const void *b) {
    int x = *(int *)a;
    int y = *(int *)b;

    if(x < y)
        return -1;
    if(x > y)
        return 1;

    return 0;
}

int max(int a, int b) {
    if(a > b)
        return a;
    return b;
}

int min(int a, int b) {
    if(a < b)
        return a;
    return b;
}

