#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libLista.h"
#include "libArquivos.h"

struct list_t *make_list() {
    struct list_t *list;
        
    if(! (list = malloc(sizeof(struct list_t))))
        return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct list_t *delete_list(struct list_t *list) {
    while(list->head != NULL) {
        struct list_node_t *aux = list->head;
        list->head = list->head->next;
        free(aux->file);
        free(aux);
    }    
    
    free(list);    

    return NULL;
}

int is_empty(struct list_t *list) {
    if(list->head == NULL && list->tail == NULL)
        return 1;

    return 0;
}

void update_order(struct list_t *list, struct list_node_t *node) {
    if(node != NULL) {
        while (node->next != NULL) {
            node->next->file->order = node->file->order + 1;
            node = node->next;
        }
    }
}

int add_list_head(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new;
    if(! (new = malloc(sizeof(struct list_node_t))))
        return 0;
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        free(new);
        return 0;
    }

    memcpy(new->file, file_data, sizeof(struct file_header_t));
    new->file->order = 0;
    
    if(list->head == NULL) {    
        new->next = NULL;
        list->head = new;
        list->tail = new;

        return 1;
    }

    new->next = list->head;
    list->head = new;
    update_order(list, list->head);

    return 1;
}

int add_list_tail(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new;

    if(list->head == NULL) {    
        return add_list_head(list, file_data);
    }

    if(! (new = malloc(sizeof(struct list_node_t))))
        return 0;
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        free(new);
        return 0;
    }

    memcpy(new->file, file_data, sizeof(struct file_header_t));
    
    new->next = NULL;
    list->tail->next = new;
    new->file->order = list->tail->file->order + 1;
    list->tail = new;

    return 1;
}

int add_list_ordered(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new, *current;

    if(list->head == NULL) {    
        return add_list_head(list, file_data);
    }

    if(! (new = malloc(sizeof(struct list_node_t))))
        return 0;
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        free(new);
        return 0;
    }
    new->next = NULL;

    memcpy(new->file, file_data, sizeof(struct file_header_t));

    if(list->head->file->order > new->file->order) {
        new->next = list->head;
        list->head = new;
        update_order(list, list->head);
        
        return 1;
    }

    current = list->head;
    while(current->next != NULL && new->file->order > current->next->file->order)
        current = current->next;

    if(current == list->tail)
        list->tail = new;
    new->next = current->next;
    current->next = new;
    update_order(list, current);

    return 1;
}

int remove_element(struct list_t *list, char *filename) {
    struct list_node_t *current, *aux;

    if(strcmp(list->head->file->filename, filename) == 0) {
        aux = list->head;
        list->head = list->head->next;
        free(aux->file);
        free(aux);
        (list->head->file->order)--;
        update_order(list, list->head);
        
        return 1;
    }

    current = list->head;
    while(current->next != NULL && strcmp(current->next->file->filename, filename))
        current = current->next;

    if(strcmp(current->next->file->filename, filename) == 0) {
        aux = current->next;
        current->next = current->next->next;
        free(aux->file);
        free(aux);
        update_order(list, current);
        return 1;
    }

    return 0;
}

void read_list(struct list_t *list) {
    struct list_node_t *current = list->head;

    if(is_empty(list))
        printf("A lista está vazia.\n");

    while (current != NULL) {
        write_file_data(current->file);

        current = current->next;
    }
}

struct file_header_t *get_first_element(struct list_t *list) {
    struct list_node_t *temp_node;
    struct file_header_t *temp_data;

    if(list->head == NULL)
        return NULL;

    temp_node = list->head;
    list->head = list->head->next;
    if(list->head != NULL) {
        (list->head->file->order)--;
        update_order(list, list->head);
    }
    temp_node->next = NULL;

    temp_data = temp_node->file;
    free(temp_node);

    return temp_data;
}
