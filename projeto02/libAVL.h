#include <wchar.h>

struct tNo {
    wchar_t chave;
    int equilibrio;
    struct tNo *esq, *dir, *pai;
    struct tNumArvore *subTree;
};

struct tArvore {
    struct tNo *raiz;
    struct tNo *ponteiro;
};

/* Cria uma árvore vazia. */
struct tArvore *criaArvore();

/* Destroi uma árvore e retorna NULL. */
struct tNo *destroiArvore(struct tArvore *tree);

struct tNo *adicionaChave(struct tArvore *tree, struct tNo *no, wchar_t chave, int *controle, int pos);

void guardaDado(struct tArvore *tree, wchar_t chave, int pos);

/* Imprime os valores guardados na estrutura */
void imprimeDados(struct tArvore *tree);

wchar_t chaveNodo(struct tNo *no);

void incrementaIterador(struct tArvore *tree);

void inicializaIterador(struct tArvore *tree);
