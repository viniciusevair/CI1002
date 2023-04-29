#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#ifndef AVLTREE
#include "libAVL.h"
#include "libSubAVL.h"
#endif /* ifndef AVLTREE */

//void imprimeChaves(char *c, struct tArvore *dados) {
//    FILE *cifra;
//    wchar_t chave;
//}

wchar_t pegaLetra(FILE *livro) {
    wchar_t letra;

    letra = fgetwc(livro);
    letra = towlower(letra);

    return letra;
}

void criaChaves(FILE *livro, struct tArvore *dados) {
    wchar_t letra;
    int pos;

    pos = 0;
    if((letra = pegaLetra(livro)) != L' ') {
        guardaDado(dados, letra, pos);
        pos++;
    }
    while (! feof(livro)) {
        if(letra == L' ' || letra == L'\n') {
            letra = pegaLetra(livro);
            while (letra == L' ' || letra == L'\n')
                letra = fgetwc(livro);
            if (letra != -1) {
                guardaDado(dados, letra, pos);
                pos++;
            }
        }

        letra = pegaLetra(livro);
    }
}

int codifica(char *b, char *m, char *o, struct tArvore *dados) {
    FILE *livro, *msgOrgnl, *msgCrpt, *arqChaves;

    if (! (livro = fopen(b, "r"))) {
        perror("Erro ao abrir arquivo");
        return 0;
    }
    criaChaves(livro, dados);
    imprimeDados(dados);

    fclose(livro);
    return 1;
}
