#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#include "libAVL.h"
#include "libTratamento.h"


/*
 * Percorre o arquivo de texto, caractere por caractere, buscando o inicio de
 * cada palavra para guardar na arvore de dados. Identifica como inicial de uma
 * palavra o primeiro caractere que nao seja espaco apos identificar um espaco.
 * Considera como espaco todos os caracteres que validam a funcao iswspace, como
 * espacos, tabs e \n.
 */
int criaChaves(FILE *livro, struct tArvore *dados) {
    wchar_t chave;
    int pos;

    pos = 0;
    if((chave = pegaCaractere(livro)) != L' ') {
        guardaDado(dados, chave, pos);
        pos++;
    }
    while (! feof(livro)) {
        if(iswspace(chave)) {
            chave = pegaCaractere(livro);
            while (iswspace(chave))
                if(! (chave = pegaCaractere(livro)))
                    return 0;
            if (chave != EOF) {
                guardaDado(dados, chave, pos);
                pos++;
            }
        }

        if(! (chave = pegaCaractere(livro)))
            return 0;
    }

    return 1;
}

int extraiDadosLivro(char *arqLivro, struct tArvore *dados) {
    FILE *livro;

    if (! (livro = fopen(arqLivro, "r"))) {
        perror("Erro ao abrir arquivo");
        return 0;
    }
    if(! (criaChaves(livro, dados))) {
        fprintf(stderr, "Erro durante a leitura de dados do Livro Cifra");
        return 0;
    }

    fclose(livro);
    return 1;
}

int transformaArquivoChaves(char *arqChaves, struct tArvore *dados) {
    FILE *arquivo;
    wchar_t chave;
    int codigo;

    if (! (arquivo = fopen(arqChaves, "r"))) {
        perror("Erro ao abrir arquivo");
        return 0;
    }

    chave = fgetc(arquivo);
    fgetc(arquivo);
    while (! feof(arquivo)) {
        while (fscanf(arquivo, "%d", &codigo) > 0) {
            guardaDado(dados, chave, codigo);
        }
        chave = fgetc(arquivo);
        fgetc(arquivo);
    }

    fclose(arquivo);
    return 1;
}
