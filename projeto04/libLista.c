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
    return (list->head == NULL);
}

void update_list(struct list_t *list, struct list_node_t *node, int pointer_fix) {
    while (node->next != NULL) {
        node->next->file->order = node->file->order + 1;
        node->next->file->archive_position += pointer_fix;
        node = node->next;
    }
}

struct file_header_t *seek_element(struct list_t *list, char *filename) {
    struct list_node_t *current = list->head;

    if(is_empty(list))
        return NULL;

    while (current != NULL) {
        if(strcmp(current->file->filename, filename) == 0)
           return current->file;

        current = current->next;
    }

    return NULL;
}

time_t get_element_modif_time(struct list_t *list, char *filename) {
    struct file_header_t *aux;
    aux = seek_element(list, filename);

    if(aux != NULL)
        return aux->modif_date;

    return -1;
}

int is_element_present(struct list_t *list, char *filename) {
    if(seek_element(list, filename))
        return 1;
    return 0;
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

    new->file->archive_position = list->head->file->archive_position;
    new->next = list->head;
    list->head = new;
    update_list(list, list->head, list->head->file->size);

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
        update_list(list, list->head, list->head->file->size);
        
        return 1;
    }

    current = list->head;
    while(current->next != NULL && new->file->order > current->next->file->order)
        current = current->next;

    if(current == list->tail)
        list->tail = new;
    new->file->archive_position = current->file->archive_position;
    new->file->archive_position += current->file->size;
    new->next = current->next;
    current->next = new;
    update_list(list, current->next, current->next->file->size);

    return 1;
}

struct file_header_t *remove_element(struct list_t *list, char *filename) {
    struct list_node_t *current, *aux;
    struct file_header_t *temp_data;

    if(is_empty(list))
        return NULL;

    if(strcmp(list->head->file->filename, filename) == 0) {
        aux = list->head;
        temp_data = aux->file;
        list->head = list->head->next;
        if(list->head != NULL) {
            (list->head->file->order)--;
            list->head->file->archive_position -= temp_data->size;
            update_list(list, list->head, temp_data->size * (-1));
        }
        aux->next = NULL;
        free(aux);
        return temp_data;
    }

    current = list->head;
    while(current->next != NULL && strcmp(current->next->file->filename, filename))
        current = current->next;

    if(strcmp(current->next->file->filename, filename) == 0) {
        if(current->next == list->tail)
            list->tail = current;
        aux = current->next;
        temp_data = aux->file;
        current->next = current->next->next;
        current->next->file->archive_position -= temp_data->size;
        aux->next = NULL;
        free(aux);
        update_list(list, current->next, temp_data->size * (-1));
        return temp_data;
    }

    return NULL;
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
        update_list(list, list->head, 0);
    }
    temp_node->next = NULL;

    temp_data = temp_node->file;
    free(temp_node);
    temp_node = NULL;

    return temp_data;
}
