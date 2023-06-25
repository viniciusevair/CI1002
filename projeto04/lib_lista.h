#ifndef __LISTA
#define __LISTA

#include "lib_arquivos.h"

struct list_node_t {
    struct file_header_t *file;
    struct list_node_t *next;
};

struct list_t {
    struct list_node_t *head, *tail;
};

/*
 * Cria e aloca espaço para uma lista encadeada. Retorna a lista criada em caso
 * de sucesso e NULL em caso de falha.
 */
struct list_t *make_list();

/*
 * Libera toda a memória associada aos nodos da lista e em seguida a memória da
 * lista. Sempre retorna NULL para aterrar o ponteiro da lista que chamou a
 * função.
 */
struct list_t *delete_list(struct list_t *l);

/*
 * Busca um elemento dentro da lista. Retorna o ponteiro para o dado guardado
 * pelo nodo caso o elemento exista na lista, e NULL caso não exista ou a lista
 * esteja vazia.
 */
struct file_header_t *seek_element(struct list_t *list, char *filename);

/*
 * Retorna a data de modificação dos metadados de um elemento da lista. Caso o
 * elemento não exista na lista, retorna -1.
 */
time_t get_element_modif_time(struct list_t *list, char *filename);

/*
 * Adiciona um nodo ao início da lista. O dado guardado é uma cópia de
 * file_data. O dado order da cópia guardada é modificado para refletir a nova
 * posição do dado. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_head(struct list_t *l, struct file_header_t *file_data);

/*
 * Adiciona um nodo ao fim da lista. O dado guardado é uma cópia de
 * file_data. O dado order da cópia guardada é modificado para refletir a nova
 * posição do dado. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_tail(struct list_t *l, struct file_header_t *file_data);

/*
 * Adiciona um nodo em ordem na lista. Para tal, é necessário que a variável
 * order de file_data seja previamente definida. O dado guardado é uma cópia de
 * file_data. Retorna 1 em caso de sucesso e 0 caso não consiga alocar
 * memória para o dado.
 */
int add_list_ordered(struct list_t *l, struct file_header_t *file_data);

/*
 * Remove um elemento especificado da lista. Retorna a struct guardada pelo nodo
 * e libera apenas a memória referente ao nodo. A memória da struct passa a ser
 * responsabilidade de quem chamou a função.
 */
struct file_header_t *remove_element(struct list_t *l, char *filename);

// Caminha por toda a lista imprimindo os dados de cada nodo.
void read_list(struct list_t *l);

/*
 * Remove o primeiro elemento da lista. Retorna a struct guardada pelo nodo
 * e libera apenas a memória referente ao nodo. A memória da struct passa a ser
 * responsabilidade de quem chamou a função.
 */
struct file_header_t *get_first_element(struct list_t *l);

#endif
