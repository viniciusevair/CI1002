/*
 * Programa elaborado pelo aluno Vinicius Evair da Silva
 * para o projeto 02 da disciplina Programacao II (CI1002).
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

enum modo_t { NOP, INSERT, APPEND, MOVE, EXTRACT, REMOVE, LIST, HELP };

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
                modo = INSERT;
                break;
            case 'a':
                modo = APPEND;
                break;
            case 'm':
                modo = MOVE;
                archivePosition = 3;
                target = strdup(optarg);
                break;
            case 'x':
                modo = EXTRACT;
                break;
            case 'r':
                modo = REMOVE;
                break;
            case 'c':
                modo = LIST;
                break;
            case 'h':
                modo = HELP;
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
