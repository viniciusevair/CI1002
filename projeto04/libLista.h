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

struct list_t *make_list();

struct list_t *delete_list(struct list_t *l);

int add_list_head(struct list_t *l, struct file_header_t *dado);

int add_list_tail(struct list_t *l, struct file_header_t *dado);

int add_list_ordered(struct list_t *l, struct file_header_t *dado);

int remove_element(struct list_t *l, char *filename);

void read_list(struct list_t *l);

struct file_header_t *get_first_element(struct list_t *l);

#endif
