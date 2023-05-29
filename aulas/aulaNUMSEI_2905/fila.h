typedef struct Elemento_t {
    void valor;
    struct Elemento_t *prox;
} Elemento_t;

typedef struct Fila_t {
    struct Elemento_t *head;
    struct Elemento_t *tail;
    int tam, maxTam;
} Fila_t;

Fila_t *cria_fila(unsigned int limite_fila);

int insere (Elemento_t *o, Fila_t *f);

int retira (Elemento_t **o, Fila_t *f);

int ehVazia (Fila_t *f);

int ehCheia (Fila_t *f);
