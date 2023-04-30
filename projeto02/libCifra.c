#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "libTratamento.h"
#ifndef AVLTREE
#include "libAVL.h"
#include "libSubAVL.h"
#endif /* ifndef AVLTREE */

void criaChaves(FILE *livro, struct tArvore *dados) {
    wchar_t letra;
    int pos;

    pos = 0;
    if((letra = pegaLetra(livro)) != L' ') {
        guardaDado(dados, letra, pos);
        pos++;
    }
    while (! feof(livro)) {
        if(iswspace(letra)) {
            letra = pegaLetra(livro);
            while (iswspace(letra))
                letra = pegaLetra(livro);
            if (letra != WEOF) {
                guardaDado(dados, letra, pos);
                pos++;
            }
        }

        letra = pegaLetra(livro);
    }
}

int extraiDadosLivro(char *b, struct tArvore *dados) {
    FILE *livro;

    if (! (livro = fopen(b, "r"))) {
        perror("Erro ao abrir arquivo");
        return 0;
    }
    criaChaves(livro, dados);

    fclose(livro);
    return 1;
}

int transformaArquivoChaves(char *c, struct tArvore *dados) {
    FILE *arquivo;
    wchar_t letra;
    int numero;

    if (! (arquivo = fopen(c, "r"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    letra = fgetc(arquivo);
    fgetc(arquivo);
    while (! feof(arquivo)) {
        while (fscanf(arquivo, "%d", &numero) > 0) {
            guardaDado(dados, letra, numero);
        }
        letra = fgetc(arquivo);
        fgetc(arquivo);
    }

    fclose(arquivo);
    return 1;
}
