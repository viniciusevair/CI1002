#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "libTratamento.h"
#ifndef AVLTREE
#include "libAVL.h"
#endif /* ifndef AVLTREE */

/*
 * Funcao interna da biblioteca. Traduz o codigo para uma letra e a insere no
 * arquivo de saida. Retorna 1 em caso de sucesso e 0 caso nao encontre uma
 * chave com a letra para o codigo.
 */
int insereLetraTraduzida(FILE *saida, struct tArvore *dados, int codigo) {
    wchar_t letra;

    if (codigo == -1)
        fprintf(saida, " ");
    else if (codigo == -2)
        fprintf(saida, "\n");
    else {
        if(! (letra = buscaNum(dados, codigo)))
            return 0;

        fprintf(saida, "%lc", letra);
    }

    return 1;
}

int decodificaMensagem(char *inpt, char *outpt, struct tArvore *dados) {
    FILE *entrada, *saida;
    int codigo;

    if (! (entrada = fopen(inpt, "r"))) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }
    if (! (saida = fopen(outpt, "w"))) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    fscanf(entrada, "%d", &codigo);
    while (! feof(entrada)) {
        if(! (insereLetraTraduzida(saida, dados, codigo)))
            return 0;
        fscanf(entrada, "%d", &codigo);
    }
    fprintf(saida, "\n");

    fclose(entrada);
    fclose(saida);

    return 1;
}
