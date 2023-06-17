#ifndef __LISTA
#define __LISTA

#include "libArquivos.h"

struct list_node_t {
    struct file_header_t *file;
    struct list_node_t *prev, *next;
};

struct list_t {
    struct list_node_t *head, *tail;
};

struct list_t *cria_lista();

struct list_t *destroi_lista(struct list_t *l);

int adiciona_inicio_lista(struct list_t *l, struct file_header_t *dado);

int adiciona_fim_lista(struct list_t *l, struct file_header_t *dado);

int adiciona_ordem_lista(struct list_t *l, struct file_header_t *dado);

int remove_arquivo(struct list_t *l, char *filename);

void ler_elementos_lista(struct list_t *l);

struct file_header_t *obtem_primeiro_lista(struct list_t *l);

#endif
