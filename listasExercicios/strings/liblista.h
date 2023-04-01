typedef  struct dado {
    char indice;
    int *chaves;
} dado_t;

/*
 * nodo_t eh uma estrutura interna que representa cada nodo da lista.
 */
typedef struct nodo_lista {
    dado_t *dado;
    struct nodo_lista *prox;
} nodo_lista_t;

typedef struct lista {
    nodo_lista_t *Primeiro;
} lista_t;

/* 
 * Cria uma lista vazia 
 */
lista_t *cria_lista ();

/* 
 * Destroi a lista e retorna NULL. 
 */ 
lista_t *destroi_lista (lista_t *l);

/* 
 * Adiciona um dado na primeira posicao da lista.
 * Uma nova copia do dado sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */ 
int adiciona_inicio_lista (lista_t *l, dado_t *dado);

/* 
 * Adiciona um dado de maneira ordenada na lista.
 * Uma nova copia do dado sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lista (lista_t *l, dado_t *dado);

/* 
 * Retorna e retira o primeiro dado da lista. 
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
dado_t *obtem_primeiro_lista (lista_t *l);

