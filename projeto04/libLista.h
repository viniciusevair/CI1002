#ifndef __LISTA
#define __LISTA

#include "libArquivos.h"

struct list_node_t {
    struct file_header_t *file;
    struct list_node_t *next;
};

struct list_t {
    struct list_node_t *head, *tail;
};

struct list_t *make_list();

struct list_t *delete_list(struct list_t *l);

struct file_header_t *seek_element(struct list_t *list, char *filename);

time_t get_element_modif_time(struct list_t *list, char *filename);

int is_element_present(struct list_t *list, char *filename);

int add_list_head(struct list_t *l, struct file_header_t *dado);

int add_list_tail(struct list_t *l, struct file_header_t *dado);

int add_list_ordered(struct list_t *l, struct file_header_t *dado);

struct file_header_t *remove_element(struct list_t *l, char *filename);

void read_list(struct list_t *l);

struct file_header_t *get_first_element(struct list_t *l);

#endif
