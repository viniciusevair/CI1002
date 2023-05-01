/*
 * Programa elaborado pelo aluno Vinicius Evair da Silva
 * para o projeto 02 da disciplina Programacao II (CI1002).
 */

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <getopt.h>
#include <time.h>

#include "libAVL.h"
#include "libSubAVL.h"
#include "libCodifica.h"
#include "libDecodifica.h"
#include "libCifra.h"

/*
 * Estrutura utilizada para facilitar a passagem de parametros e leitura das
 * funcoes encode, decode e getopt.
 */
struct tEntrada {
    char *livro;
    char *msgOriginal;
    char *msgCodificada;
    char *msgDecodificada;
    char *arqvChaves;
    int encode;
    int decode;
};

/* Mensagem final de todos os erros de entrada */
void erroEntrada(char *argv[]) {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "%s -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves\n", argv[0]);
    fprintf(stderr, "%s -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada\n", argv[0]);
    fprintf(stderr, "%s -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* Evitar o uso das flags -d e -e simultaneamente. */
void conflitoEntrada(char *argv[]) {
    fprintf(stderr, "Conflito de instrucoes. Escolha entre codificar ou decodificar.\n");
    erroEntrada(argv);
}

/* Funcao para tratar a codificacao da mensagem. */
int encode(struct tEntrada entrada, struct tArvore *dados, char *argv[]) {
    if (entrada.livro == NULL || entrada.msgCodificada == NULL) {
        fprintf(stderr, "Quantidade de parametros insuficiente.\n");
        erroEntrada(argv);
    }
    if(! (extraiDadosLivro(entrada.livro, dados)))
        return 0;
    if(! (codificaMsg(entrada.msgOriginal, entrada.msgCodificada, dados)))
        return 0;
    if (entrada.arqvChaves)
        imprimeChaves(entrada.arqvChaves, dados);

    return 1;
}

/*
 * Funcao para tratar a decodificacao da mensagem. Caso o usuario opte por usar
 * um livro cifra, mas ainda assim passe como argumento a flag -c, 
 * o algoritmo optará pelo uso do livro.
 */
int decode(struct tEntrada entrada, struct tArvore *dados) {
    if (entrada.livro) {
        extraiDadosLivro(entrada.livro, dados);
    }
    else if (entrada.arqvChaves) {
        if(! (transformaArquivoChaves(entrada.arqvChaves, dados)))
            return 0;
    }
    if(! (decodificaMensagem(entrada.msgCodificada, entrada.msgDecodificada, dados))) {
        fprintf(stderr, "Decodificacao abortada.\n");
        fprintf(stderr, "Codigo da chave nao foi encontrado no arquivo de chaves.\n");
        return 0;
    }

    return 1;
}

/* Funcao simples para tratamento das flags de entrada. */
void analisaEntradas(struct tEntrada *entrada, int argc, char *argv[]) {
    int option;

    if (argc < 8) {
        fprintf(stderr, "Quantidade de parametros insuficiente.\n");
        erroEntrada(argv);
    }

    opterr = 0;
    while ((option = getopt (argc, argv, "b:c:dei:m:o:")) != -1)
        switch (option) {
            case 'b':
                entrada->livro = optarg;
                break;
            case 'c':
                entrada->arqvChaves = optarg;
                break;
            case 'd':
                entrada->decode = 1;
                if (entrada->encode) 
                    conflitoEntrada(argv);
                break;
            case 'e':
                entrada->encode = 1;
                if (entrada->decode) 
                    conflitoEntrada(argv);
                break;
            case 'i':
                entrada->msgCodificada = optarg;
                break;
            case 'm':
                entrada->msgOriginal = optarg;
                break;
            case 'o':
                if (entrada->encode)
                    entrada->msgCodificada = optarg;
                else if (entrada->decode)
                    entrada->msgDecodificada = optarg; 
                else {
                    fprintf(stderr, "A flag -o depende das flags -d ou -e antes.\n");
                    erroEntrada(argv);
                }
                break;
            default:
                fprintf(stderr, "Parametros de entrada incorretos.\n");
                erroEntrada(argv);
        }
}

/* Marca inicialmente todas as entradas como vazio/zero */
void inicializaEntradas(struct tEntrada *entrada) {
    entrada->livro = NULL;
    entrada->arqvChaves = NULL;
    entrada->msgOriginal = NULL;
    entrada->msgCodificada = NULL;
    entrada->msgDecodificada = NULL;
    entrada->encode = 0;
    entrada->decode = 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    struct tArvore *dados;
    struct tEntrada entrada;

    inicializaEntradas(&entrada);
    analisaEntradas(&entrada, argc, argv);

    dados = criaArvore();
    if (entrada.encode) {
        if(! (encode(entrada, dados, argv)))
            return 1;
    }
    else if (entrada.decode) {
        if(! (decode(entrada, dados)))
            return 1;
    }

    dados = destroiArvore(dados);
    return 0;
}
