/*
 * Programa elaborado pelo aluno Vinicius Evair da Silva
 * para o projeto 02 da disciplina Programacao II (CI1002).
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "libArquivos.h"
#include "libVina.h"

enum modo_t {NOP, INSERIR, ATUALIZAR, MOVER, EXTRAIR, REMOVER, LISTAR, AJUDA};

void erro_entrada(char *argv[]) {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "%s [-i | -a | -m <target> | -x | -r | -c | -h]", argv[0]);
    fprintf(stderr, " <archive> [membro1 membro2 ...]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    enum modo_t modo = NOP;
    FILE *arq;
    int opt;
    int archive_arg_position = 2;
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
                archive_arg_position = 3;
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
                help_utility(argv[0]);
                return 0;
            default:
                erro_entrada(argv);
        }		
    }

    //TIRAR DA MAIN DEPOIS ===================================================
    filename = strdup(argv[archive_arg_position]);

    arq = open_archiver(filename);
    if (modo == INSERIR) {
        insert_operation(arq, argv, argc);
    } else if (modo == LISTAR) {
        list_files(arq);
    } else if (modo == EXTRAIR) {
        extract_operation(arq, argv, argc);
    } else if (modo == REMOVER) {
        remove_operation(arq, argv, argc);
    } else if (modo == ATUALIZAR) {
        update_operation(arq, argv, argc);
    } else if (modo == MOVER) {
        move_operation(arq, argv, argc, target);
    }

    fclose(arq);
    free(filename);
    return 0;
}
