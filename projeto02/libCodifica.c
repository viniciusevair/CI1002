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

void imprimeChaves(char *c, struct tArvore *dados) {
    FILE *cifra;

    if (! (cifra = fopen(c, "w"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    imprimeDadosEmArq(cifra, dados);

    fclose(cifra);
}

int codificaLetra(wchar_t letra, int *codigo, struct tArvore *dados) {
    if (letra == L'\n')
        (*codigo) = -2;
    else if (iswspace(letra))
        (*codigo) = -1;
    else
        if(! (buscaDadoAleatorio(letra, dados, codigo)))
            return 0;

    return 1;
}

void escreveCodigo(FILE *codificada, int *primeiraLetra, int codigo) {
    if (*primeiraLetra) {
        fprintf(codificada, "%d", codigo);
        (*primeiraLetra) = 0;
    }
    else
        fprintf(codificada, " %d", codigo);
}

int codificaMsg(char *arqEntrada, char *arqSaida, struct tArvore *dados) {
    FILE *original, *codificada;
    wchar_t letra;
    int codigo, primeiraLetra;

    if (! (original = fopen(arqEntrada, "r"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    if (! (codificada = fopen(arqSaida, "w"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    letra = pegaLetra(original);
    primeiraLetra = 1;
    while (! feof(original)) {
        if(! (codificaLetra(letra, &codigo, dados))) {
            fprintf(stderr, "Codificacao de mensagem abortada.\n");
            fprintf(stderr, "Nao foi possivel encontrar a chave desta letra.");
            return 0;
        }

        letra = pegaLetra(original);
        if (letra == WEOF)
            break;

        escreveCodigo(codificada, &primeiraLetra, codigo);
    }
    fprintf(codificada, "\n");

    fclose(original);
    fclose(codificada);

    return 1;
}
