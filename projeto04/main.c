/*
 * Programa elaborado pelo aluno Vinicius Evair da Silva
 * para o projeto 02 da disciplina Programacao II (CI1002).
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

enum modo_t {NOP, INSERIR, ATUALIZAR, MOVER, EXTRAIR, REMOVER, LISTAR, AJUDA};

void erroEntrada(char *argv[]) {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "%s [-i | -a | -m <target> | -x | -r | -c | -h]", argv[0]);
    fprintf(stderr, " <archive> [membro1 membro2 ...]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    enum modo_t modo = NOP;
    FILE *arq, *memb;
    char reg = 'b';
    int opt;
    int archivePosition = 2;
    char *target;
    char *filename;

    while ((opt = getopt(argc, argv, "iam:xrch")) != -1) {
        switch (opt) {
            case 'i':
                modo = INSERIR;
                break;
            case 'a':
                modo = ATUALIZAR;
                break;
            case 'm':
                modo = MOVER;
                archivePosition = 3;
                target = strdup(optarg);
                break;
            case 'x':
                modo = EXTRAIR;
                break;
            case 'r':
                modo = REMOVER;
                break;
            case 'c':
                modo = LISTAR;
                break;
            case 'h':
                modo = AJUDA;
                break;
            default:
                erroEntrada(argv);
        }		
    }

    //TIRAR DA MAIN DEPOIS ===================================================
    filename = strdup(argv[archivePosition]);

    //testando insert
    arq = fopen(filename, "w+");
    fwrite(&reg, sizeof(char), 1, arq);

    fclose(arq);
    free(filename);
    return 0;
}
