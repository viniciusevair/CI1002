/*
 * Programa elaborado pelo aluno Vinicius Evair da Silva
 * para o projeto 04 da disciplina Programacao II (CI1002).
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "lib_arquivos.h"
#include "lib_vina.h"

void erro_entrada(char *argv[]) {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "%s [-i | -a | -m <target> | -x | -r | -c | -h]", argv[0]);
    fprintf(stderr, " <archive> [membro1 membro2 ...]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    FILE *archive;
    int opt;
    int archive_arg_position = 2;
    char *target;
    char *filename;

    while((opt = getopt(argc, argv, "iam:xrch")) != -1) {
        switch(opt) {
            case 'i':
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                insert_operation(archive, argv, argc);
                break;
            case 'a':
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                update_operation(archive, argv, argc);
                break;
            case 'm':
                archive_arg_position = 3;
                target = strdup(optarg);
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                move_operation(archive, argv, argc, target);
                break;
            case 'x':
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                extract_operation(archive, argv, argc);
                break;
            case 'r':
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                remove_operation(archive, argv, argc);
                break;
            case 'c':
                filename = strdup(argv[archive_arg_position]);
                archive = open_archiver(filename);
                list_operation(archive);
                break;
            case 'h':
                help_utility(argv[0]);
                return 0;
            default:
                erro_entrada(argv);
        }		
    }

    fclose(archive);
    free(filename);
    return 0;
}
