/*
 * Biblioteca implementada pelo aluno Vinicius Evair da Silva para o projeto 02
 * da disciplina Programacao II (CI1002).
 * Nesta biblioteca encontram-se as funcoes que lidam com o gerenciamento da
 * estrutura de dados em que as chaves sao guardadas. Utiliza da biblioteca
 * interna libSubAVL para gerenciar os dados de posicoes de cada chave.
 */

#include <wchar.h>

/*
 * Estrutura interna para gerenciamento dos dados guardados.
 * Cada no da arvore guarda uma outra arvore de posicoes de palavras que iniciam
 * com o caractere chave do no.
 */
struct tNo {
    wchar_t chave;
    int equilibrio; /* Controle para o balanceamento da arvore */
    struct tNo *esq, *dir, *pai;
    struct tNumArvore *subTree;
};

/* Estrutura para acesso dos dados pelo usuario */
struct tArvore {
    struct tNo *raiz;
};

/*
 * Cria e aloca espaco para uma arvore vazia. A responsabilidade por desalocar a
 * memoria utilizada e de quem chamou a funcao.
 */
struct tArvore *criaArvore();

/*
 * Desaloca recursivamente a memoria de todos os nos e em seguida a da arvore.
 * sempre retorna NULL.
 */
struct tArvore *destroiArvore(struct tArvore *tree);

/*
 * Funcao para guardar uma posicao na sub-arvore de uma chave. Se o no ainda nao
 * estiver presente na arvore, faz a inicializacao do no e tambem de sua
 * subarvore.
 */
void guardaDado(struct tArvore *tree, wchar_t chave, int pos);

/*
 * Faz a busca em toda a estrutura pelo valor da posicao passado. Devolve a
 * chave do no que guarda a posicao.
 */
wchar_t buscaNum(struct tArvore *tree, int pos);

/*
 * Guarda todos os dados da estrutura em um arquivo. A responsabilidade pela
 * abertura e fechamento do arquivo de maneira correta e de quem chamou a
 * funcao.
 */
void imprimeDadosEmArq(FILE *arq, struct tArvore *tree);

/*
 * Busca e retorna uma posicao aleatoria entre as que estao guardadas no no
 * passado como argumento para a funcao.
 */
int buscaDadoAleatorio(wchar_t chave, struct tArvore *tree, int *pos);

/* 
 * Funcao de debugging.
 * Imprime para stdout em ordem crescente os valores guardados na estrutura.
 */
void imprimeDados(struct tArvore *tree);
