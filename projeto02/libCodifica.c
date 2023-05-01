#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "libAVL.h"
#include "libTratamento.h"

int imprimeChaves(char *arqChaves, struct tArvore *dados) {
    FILE *cifra;

    if (! (cifra = fopen(arqChaves, "w"))) {
        perror("Erro ao abrir arquivo");
        return 0;
    }

    imprimeDadosEmArq(cifra, dados);

    fclose(cifra);
    return 1;
}

/*
 * Funcao auxiliar que transforma um caractere em algum codigo baseado no arquivo
 * de chaves.
 */
int codificaCaractere(wchar_t chave, int *codigo, struct tArvore *dados) {
    if (chave == L'\n')
        (*codigo) = -2;
    else if (iswspace(chave))
        (*codigo) = -1;
    else
        if(! (buscaDadoAleatorio(chave, dados, codigo)))
            return 0;

    return 1;
}

/* Funcao auxiliar para escrever o codigo no arquivo, evitando espacos
 * desnecessarios. */
void escreveCodigo(FILE *codificada, int *primeiroCaractere, int codigo) {
    if (*primeiroCaractere) {
        fprintf(codificada, "%d", codigo);
        (*primeiroCaractere) = 0;
    }
    else
        fprintf(codificada, " %d", codigo);
}

int codificaMsg(char *arqEntrada, char *arqSaida, struct tArvore *dados) {
    FILE *original, *codificada;
    wchar_t chave;
    int codigo, primeiroCaractere;

    if (! (original = fopen(arqEntrada, "r"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    if (! (codificada = fopen(arqSaida, "w"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    chave = pegaCaractere(original);
    primeiroCaractere = 1;
    while (! feof(original)) {
        if(! (codificaCaractere(chave, &codigo, dados))) {
            fprintf(stderr, "Codificacao de mensagem abortada.\n");
            fprintf(stderr, "Nao foi possivel encontrar a chave do caractere \"%lc\".\n", chave);
            return 0;
        }

        chave = pegaCaractere(original);
        if (chave == EOF)
            break;

        escreveCodigo(codificada, &primeiroCaractere, codigo);
    }
    fprintf(codificada, "\n");

    fclose(original);
    fclose(codificada);

    return 1;
}
