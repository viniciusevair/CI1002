#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "libAVL.h"
#include "libTratamento.h"
#include "libDecodifica.h"

/*
 * Funcao interna da biblioteca. Traduz o codigo para um caractere e o insere no
 * arquivo de saida. Retorna 1 em caso de sucesso e 0 caso nao encontre um
 * caractere para o codigo passado.
 */
int insereCaractereTraduzido(FILE *saida, struct tArvore *dados, int codigo) {
    wchar_t chave;

    if (codigo == -1)
        fprintf(saida, " ");
    else if (codigo == -2)
        fprintf(saida, "\n");
    else {
        if(! (chave = buscaNum(dados, codigo)))
            return 0;

        fprintf(saida, "%lc", chave);
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
        if(! (insereCaractereTraduzido(saida, dados, codigo)))
            return 0;
        fscanf(entrada, "%d", &codigo);
    }
    fprintf(saida, "\n");

    fclose(entrada);
    fclose(saida);

    return 1;
}
