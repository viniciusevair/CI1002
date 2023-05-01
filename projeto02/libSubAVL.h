/*
 * Biblioteca implementada pelo aluno Vinicius Evair da Silva para o projeto 02
 * da disciplina Programacao II (CI1002).
 * Biblioteca interna da libAVL.h. Contem todas as funcoes que lidam com o
 * gerenciamento dos dados de posicoes.
 */

#include <stdio.h>

struct tNumNo {
    int pos;
    int equilibrio;
    struct tNumNo *esq, *dir, *pai;
};

struct tNumArvore {
    struct tNumNo *raiz;
    struct tNumNo *ponteiro;
};

/*
 * Cria e aloca espaco para uma arvore vazia. A responsabilidade por desalocar a
 * memoria utilizada e de quem chamou a funcao.
 */
struct tNumArvore *criaNumArvore();

/*
 * Desaloca recursivamente a memoria de todos os nos e em seguida a da arvore.
 * sempre retorna NULL.
 */
struct tNumNo *destroiNumArvore(struct tNumArvore *tree);

/* Adiciona uma nova chave na arvore */
struct tNumNo *adicionaPos(struct tNumArvore *tree, struct tNumNo *no, int pos, int *controle);

/* Busca e retorna um ponteiro para o no que contem o valor pos */
struct tNumNo *buscaPos(struct tNumNo *no, int pos);

/* Retorna aleatoriamente o valor da chave algum no da arvore. */
int posAleatoria(struct tNumArvore *tree);

/* 
 * Funcao de debugging.
 * Imprime para stdout em ordem decrescente os valores guardados na estrutura.
 */
void imprimeInverso(struct tNumNo *no);

/*
 * Guarda todos os dados da estrutura em um arquivo. A responsabilidade pela
 * abertura e fechamento do arquivo de maneira correta e de quem chamou a
 * funcao.
 */
void imprimeInversoEmArq(FILE *arq, struct tNumNo *no);
