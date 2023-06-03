#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { NOP, ENCODE, DECODE } Modo_t;

#define BUFFER_SIZE 10

// PROTÓTIPOS

// Função que comprime 'fIn', gravando o resultado da compressão em 'fOut'
void Encode (char *fIn, char *fOut);

// Função que descomprime 'fIn', gravando o resultado da descompressão em 'fOut'
void Decode (char *fIn, char *fOut);

// Função  ser  usada  no  programa para  chamar  Encode()  ou  Decode()
void rle (void (func) (char*, char*), char *input, char *output);


// PROGRAMA PRINCIPAL
int main(int argc, char *argv[])
{

    int opt;
    char *input = NULL;
    char *output = NULL;
    Modo_t modo = NOP;

    while ((opt = getopt(argc, argv, "dei:o:")) != -1) {
        switch (opt) {
            case 'i':
                input = strdup(optarg);
                break;
            case 'o':
                output = strdup(optarg);
                break;
            case 'e':
                modo |= ENCODE;
                break;
            case 'd':
                modo |= DECODE;
                break;
            default:
                fprintf(stderr, "Forma de uso: ./rle [-e | -d] -i <arq_in> -o <arq_out>\n");
                exit(1);
        }		
    }

    // Erro se:
    //  * não forneceu os dois arquivos
    //  * não forneceu uma das opções '-e' ou '-d',
    //  * forneceu as opções '-e' e '-d' na mesma linha de comando
    if ( ! input || ! output || modo == NOP || modo & (modo-1) ) {
        fprintf(stderr, "Forma de uso: ./rle [-e | -d] -i <arq_in> -o <arq_out>\n");
        exit(2);
    }

    /* COMPLEMENTE O CÓDIGO PARA DESENVOLVER O PROGRAMA PRINCIPAL A PARTIR DESTE PONTO */
    if(modo == ENCODE) {
        rle(Encode, input, output);
    } else if (modo == DECODE) {
        rle(Decode, input, output);
    }

    return 0 ;
}


/* DEFINA ABAIXO O CÓDIGO DAS FUNÇÕES Encode(), Decode() e rle()  */

// Função ser usada no programa principal para chamar Encode() ou Decode()
void rle (void (func) (char*, char*), char *input, char *output)
{
    func(input, output);
}

void escreveArquivoAux(FILE *saida, char *buffer, char *reg1, char *reg2, int *aux, int *count) {
    buffer[*aux] = *reg1;
    sprintf(&buffer[*aux + 1], "%d", *count);
    *reg1 = *reg2;
    *count = 1;
    (*aux) += 2;

    if(*aux == BUFFER_SIZE) {
        fwrite(buffer, sizeof(char), BUFFER_SIZE, saida);
        *aux = 0;
    }
}

// Função que comprime 'fIn', gravando o resultado da compressão em 'fOut'
void Encode (char *fIn, char *fOut)
{
    FILE *arq, *saida;
    char *buffer;
    char reg1, reg2;
    int count = 0;
    int aux = 0;

    arq = fopen (fIn, "r");
    if (! arq ) {
        perror ("Erro ao abrir arquivo x");
        exit (1);
    }
    saida = fopen (fOut, "w");
    if (! arq ) {
        perror ("Erro ao abrir arquivo x");
        exit (1);
    }

    if(! (buffer = calloc(BUFFER_SIZE, sizeof(char))))
        exit (1);

    fread(&reg1, sizeof(char), 1, arq);
    count++;

    while (! feof(arq)) {
        fread(&reg2, sizeof(char), 1, arq);
        if (reg2 == reg1) {
            if(count < BUFFER_SIZE - 1) {
                count++;
            } else {
                escreveArquivoAux(saida, buffer, &reg1, &reg2, &aux, &count);
            }
        } else {
            escreveArquivoAux(saida, buffer, &reg1, &reg2, &aux, &count);
        }
    }
    fwrite(buffer, sizeof(char), aux, saida);

    free(buffer);
    fclose(arq);
    fclose(saida);
}



// Função que descomprime 'fIn', gravando o resultado da descompressão em 'fOut'
void Decode (char *fIn, char *fOut)
{
    FILE *arq, *saida;
    char *buffer;
    char reg;
    int aux = 0;
    int count;
    int resto;

    arq = fopen (fIn, "r");
    if (! arq ) {
        perror ("Erro ao abrir arquivo x");
        exit (1);
    }
    saida = fopen (fOut, "w");
    if (! arq ) {
        perror ("Erro ao abrir arquivo x");
        exit (1);
    }

    if(! (buffer = calloc(BUFFER_SIZE, sizeof(char))))
        exit (1);

    fread(buffer, sizeof(char), BUFFER_SIZE, arq);
    while (! feof(arq)) {
        while (aux < BUFFER_SIZE) {
            reg = buffer[aux];
            count = atoi(&buffer[aux + 1]);
            for ( ; count > 0; count --) {
                fwrite(&reg, sizeof(char), 1, saida);
            }
            aux +=2;
        }

        aux = 0;
        resto = fread(buffer, sizeof(char), BUFFER_SIZE, arq);
    }
    while (aux < resto) {
        reg = buffer[aux];
        count = atoi(&buffer[aux + 1]);
        for ( ; count > 0; count --) {
            fwrite(&reg, sizeof(char), 1, saida);
        }
        aux +=2;
    }

    free(buffer);
    fclose(arq);
    fclose(saida);
}
