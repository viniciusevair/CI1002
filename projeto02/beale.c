#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include "libAVL.h"
#include "libSubAVL.h"
#include "libCodifica.h"
#include "libDecodifica.h"
#include "libCifra.h"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    srand(time(NULL));

    struct tArvore *dados;
    int option;
    int flag_d = 0, flag_e = 0;
    char *value_b = NULL, *value_c = NULL, *value_i = NULL;
    char *value_m = NULL, *value_o = NULL;

    opterr = 0;
    while ((option = getopt (argc, argv, "b:c:dei:m:o:")) != -1)
        switch (option) {
            case 'b':
                value_b = optarg;
                break;
            case 'c':
                value_c = optarg;
                break;
            case 'd':
                if (flag_e == 0)
                    flag_d = 1;
                break;
            case 'e':
                if (flag_d == 0)
                    flag_e = 1;
                break;
            case 'i':
                value_i = optarg;
                break;
            case 'm':
                value_m = optarg;
                break;
            case 'o':
                value_o = optarg;
                break;
            default:
                fprintf(stderr, "Usa direito ai, irmão\n");
                exit(1);
        }

    dados = criaArvore();

    if (flag_d == flag_e) {
        fprintf(stderr, "Usa direito ai, irmão\n");
        exit(1);
    }

    if (flag_e) {
        if (value_b == NULL || value_o == NULL) {
            fprintf(stderr, "Usa direito ai, irmão\n");
            exit(1);
        }
        extraiDadosLivro(value_b, dados);
        codificaMsg(value_m, value_o, dados);
        if (value_c != NULL)
            imprimeChaves(value_c, dados);
    }
    if (flag_d) {
        if (value_b)
            extraiDadosLivro(value_b, dados);
        else if (value_c) {
            transformaArquivoChaves(value_c, dados);
        }
    }

    return 0;
}
