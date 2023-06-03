#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

void leRegistro (FILE *fonte, int argc, int flag_t) {
    FILE *arq;
    int i, reg_tam, j;
    char *registro, *reg_nome;

    fread(&reg_tam, sizeof(unsigned int), 1, fonte);
    if (! (registro = calloc(reg_tam + 1, sizeof(char)))) {
        perror("Erro ao alocar memoria");
        exit(1);
    }

    fread(registro, sizeof(char), reg_tam, fonte);

    if (! (reg_nome = calloc(reg_tam + 1, sizeof(char)))) {
        perror("Erro ao alocar memoria");
        exit(1);
    }
    strcpy(reg_nome, registro);
    reg_nome[strcspn(reg_nome, "\n")] = 0;

    j = 0;
    for (i = strlen(reg_nome) + 1; i < reg_tam; i++) {
        registro[j] = registro[i];
        j++;
    }
    j++;
    registro[j] = 0;

    if (argc == 1) {
        if (! (arq = fopen(reg_nome, "w"))) {
            perror("Erro ao abrir arquivo");
            exit(1);
        }

        fprintf(arq, "%s", registro);
        fclose(arq);
    }
    else if (flag_t) {
        printf("%s %lu\n", reg_nome, strlen(registro));
    }

    free(registro);
    free(reg_nome);
}

int main(int argc, char *argv[]) {
    FILE *fonte;
    int qtd, i, option, flag_t;

    if (! (fonte = fopen("bkp.bin", "r+"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fread(&qtd, sizeof(unsigned int), 1, fonte);

    if (argc > 1) {
        opterr = 0;
        while ((option = getopt(argc, argv, "t")) != -1) {
            switch (option) {
                case 't':
                    flag_t = 1;
                    break;
                default: 
                    fprintf (stderr, "Uso: %s [-t]\nA flag -t é opcional\n", argv[0]);
                    exit (1);
            }

        }
    }

    for (i = 0; i < qtd; i++)
        leRegistro(fonte, argc, flag_t);

    fclose(fonte);
    return 0;
}
