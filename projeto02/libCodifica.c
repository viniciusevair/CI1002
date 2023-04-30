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

    if (! (cifra = fopen(c, "w, css=UTF-8"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    imprimeDadosEmArq(cifra, dados);

    fclose(cifra);
}

void codificaMsg(char *m, char *o, struct tArvore *dados) {
    FILE *original, *codificada;
    wchar_t letra;
    int codigo, primeiraLetra;

    if (! (original = fopen(m, "r"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    if (! (codificada = fopen(o, "w"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    letra = pegaLetra(original);
    primeiraLetra = 1;
    while (! feof(original)) {
        if (letra == L'\n')
            codigo = -2;
        else if (iswspace(letra))
            codigo = -1;
        else
            codigo = buscaDadoAleatorio(letra, dados);

        letra = pegaLetra(original);
        if (letra == WEOF)
            break;

        if (primeiraLetra) {
            fprintf(codificada, "%d", codigo);
            primeiraLetra = 0;
        }
        else
            fprintf(codificada, " %d", codigo);
    }
    fprintf(codificada, "\n");

    fclose(original);
    fclose(codificada);
}
