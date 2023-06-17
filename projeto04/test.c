#include <stdio.h>
#include "libLista.h"
#include "libArquivos.h"

int main(int argc, char *argv[]) {
    struct lista_t *lista = cria_lista();

    struct file_header_t *file0 = pega_dados(argv[2]);
    struct file_header_t *file1 = pega_dados(argv[3]);
    struct file_header_t *file2 = pega_dados(argv[4]);
    struct file_header_t *file3 = pega_dados(argv[5]);
    struct file_header_t *file4 = pega_dados(argv[6]);
    struct file_header_t *file5 = pega_dados(argv[7]);

    adiciona_inicio_lista(lista, file0);
    adiciona_fim_lista(lista, file1);
    adiciona_ordem_lista(lista, file2);
    adiciona_fim_lista(lista, file3);
    adiciona_fim_lista(lista, file4);
    adiciona_fim_lista(lista, file5);

    printf("Elementos da lista:\n");
    ler_elementos_lista(lista);

    struct file_header_t *primeiro = obtem_primeiro_lista(lista);
    if (primeiro != NULL) {
        printf("Primeiro elemento removido: %s\n", primeiro->filename);
    }

    printf("Elementos atualizados da lista:\n");
    ler_elementos_lista(lista);

    printf("Removendo um elemento qualquer.\n");
    remove_arquivo(lista, file4->filename);

    printf("Elementos atualizados da lista:\n");
    ler_elementos_lista(lista);

    destroi_lista(lista);

    return 0;
}
