#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINEMAX 1024

/*
 *  Lê um caractere e, caso seja um algarismo numérico, o adiciona ao fim do
 *  número. Isso é, multiplica o valor atual por 10 e soma o novo valor.
 */
void acrescentaTextoNum (char c, int *num) {
    int aux;
    if (c >= '0' && c <= '9') {
        aux = (int)c - 48;
        (*num) *= 10;
        (*num) += aux;
    }
}

/* As coordenadas são dadas em uma única linha, separada por espaço. */
void lerCoordenada (char *s, int *lin, int *col) {
    int i, aux;

    (*lin) = 0;
    (*col) = 0;
    for (i = 0; s[i] != ' '; i++)
        acrescentaTextoNum(s[i], lin);
    for (; i < strlen(s); i++)
        acrescentaTextoNum(s[i], col);
}

void lerMapa (FILE *srce, char **mapa, char *s, int lin, int col) {
    int i;

    i = 0;
    while (! feof(srce)) {
        for (int j = 0; j < col; j++)
            mapa[i][j] = fgetc(srce);

        i++;
        fgetc(srce);
    }
}

void escreverMapa (char **mapa, int lin, int col) {
    printf("%d %d\n", lin, col);
    for (int i = 0; i < lin; i++) {
        for (int j = 0; j < col; j++)
            printf("%c", mapa[i][j]);
        printf("\n");
    }
}

int main () {
    FILE *srce;
    char *s;
    char **mapa;
    int linhaCoord, colunaCoord;

    if (! (s = calloc(LINEMAX, sizeof(char)))) {
        perror ("Erro ao alocar memória.");
        exit(1);
    }
    if (! (mapa = calloc(100, sizeof(char*)))) {
        perror("Erro ao alocar memória.");
        exit(1);
    }
    for (int i = 0; i < 100; i++)
        if (! (mapa[i] = calloc(100, sizeof(char)))) {
            perror("Erro ao alocar memória.");
            exit(1);
        }
    if (! (srce = fopen("mapa.txt", "r"))) {
        perror("Erro ao abrir o arquivo.");
        exit (1);
    }

    /* 
     *  O bloco a seguir faz a leitura do arquivo de texto contendo coordenadas
     *  na primeira linha e um mapa para o jogo nas demais linhas.
     */
    fgets(s, LINEMAX, srce);
    lerCoordenada(s, &linhaCoord, &colunaCoord);
    lerMapa(srce, mapa, s, linhaCoord, colunaCoord);

    escreverMapa(mapa, linhaCoord, colunaCoord);

    /*  Libera toda a memória usada. */
    for (int i = 0; i < 100; i++)
        free(mapa[i]);
    free(mapa);
    free(s);
    fclose(srce);
    return 0;
}
