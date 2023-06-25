#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_lista.h"
#include "lib_arquivos.h"

/*
 * Cria e aloca espaço para uma lista encadeada. Retorna a lista criada em caso
 * de sucesso e NULL em caso de falha.
 */
struct list_t *make_list() {
    struct list_t *list;
        
    if(! (list = malloc(sizeof(struct list_t)))) {
        fprintf(stderr, "Não foi possível alocar memória para a lista encadeada.\n");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

/*
 * Libera toda a memória associada aos nodos da lista e em seguida a memória da
 * lista. Sempre retorna NULL para aterrar o ponteiro da lista que chamou a
 * função.
 */
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

/*
 * Corrige a ordem de todos os nodos após uma inserção/remoção ser feita na
 * lista. Também atualiza a posição inicial dos membros dentro do arquivo.
 */
void update_list(struct list_t *list, struct list_node_t *node, int pointer_fix) {
    while(node->next != NULL) {
        node->next->file->order = node->file->order + 1;
        node->next->file->archive_position += pointer_fix;
        node = node->next;
    }
}

/*
 * Busca um elemento dentro da lista. Retorna o ponteiro para o dado guardado
 * pelo nodo caso o elemento exista na lista, e NULL caso não exista ou a lista
 * esteja vazia.
 */
struct file_header_t *seek_element(struct list_t *list, char *filename) {
    struct list_node_t *current = list->head;
    /* 
     * Relativiza o filename para uso pela função de inserção/atualização, que
     * ainda não guardaram o nome relativo por não terem acessado o arquivo em
     * disco.
     */
    char *relative_filename = relativize_filepath(filename);

    if(is_empty(list))
        return NULL;

    while(current != NULL) {
        if(strcmp(current->file->filename, relative_filename) == 0)
           return current->file;

        current = current->next;
    }

    return NULL;
}

/*
 * Retorna a data de modificação dos metadados de um elemento da lista. Caso o
 * elemento não exista na lista, retorna -1.
 */
time_t get_element_modif_time(struct list_t *list, char *filename) {
    struct file_header_t *aux;
    aux = seek_element(list, filename);

    if(aux != NULL)
        return aux->modif_date;

    return -1;
}

/*
 * Adiciona um nodo ao início da lista. O dado guardado é uma cópia de
 * file_data. O dado order da cópia guardada é modificado para refletir a nova
 * posição do dado. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_head(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new;

    if(! (new = malloc(sizeof(struct list_node_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 0;
    }
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
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

/*
 * Adiciona um nodo ao fim da lista. O dado guardado é uma cópia de
 * file_data. O dado order da cópia guardada é modificado para refletir a nova
 * posição do dado. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_tail(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new;

    if(list->head == NULL) {    
        return add_list_head(list, file_data);
    }

    if(! (new = malloc(sizeof(struct list_node_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 0;
    }
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
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

/*
 * Adiciona um nodo em ordem na lista. Para tal, é necessário que a variável
 * order de file_data seja previamente definida. O dado guardado é uma cópia de
 * file_data. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_ordered(struct list_t *list, struct file_header_t *file_data) {
    struct list_node_t *new, *current;

    if(list->head == NULL) {    
        return add_list_head(list, file_data);
    }

    if(! (new = malloc(sizeof(struct list_node_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 0;
    }
    if(! (new->file = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        free(new);
        return 0;
    }
    new->next = NULL;

    memcpy(new->file, file_data, sizeof(struct file_header_t));

    if(list->head->file->order >= new->file->order) {
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

/*
 * Remove um elemento especificado da lista. Retorna a struct guardada pelo nodo
 * e libera apenas a memória referente ao nodo. A memória da struct passa a ser
 * responsabilidade de quem chamou a função.
 */
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
        if(current->next != NULL) {
            (current->next->file->order)--;
            current->next->file->archive_position -= temp_data->size;
            update_list(list, current->next, temp_data->size * (-1));
        }
        aux->next = NULL;
        free(aux);
        return temp_data;
    }

    return NULL;
}


// Caminha por toda a lista imprimindo os dados de cada nodo.
void read_list(struct list_t *list) {
    struct list_node_t *current = list->head;

    if(is_empty(list))
        printf("A lista está vazia.\n");

    while(current != NULL) {
        print_file_data(current->file);

        current = current->next;
    }
}

/*
 * Remove o primeiro elemento da lista. Retorna a struct guardada pelo nodo
 * e libera apenas a memória referente ao nodo. A memória da struct passa a ser
 * responsabilidade de quem chamou a função.
 */
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
