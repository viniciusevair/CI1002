#include <stdio.h>
#include "libLista.h"
#include "libArquivos.h"
#include "libVina.h"

int main(int argc, char *argv[]) {
    FILE *arq;
    struct list_t *lista = make_list();
    read_list(lista);

    struct file_header_t *file0 = get_data(argv[2]);
    struct file_header_t *file1 = get_data(argv[3]);
    struct file_header_t *file2 = get_data(argv[4]);
    struct file_header_t *file3 = get_data(argv[5]);
    struct file_header_t *file4 = get_data(argv[6]);
    struct file_header_t *file5 = get_data(argv[7]);

    add_list_head(lista, file0);
    add_list_tail(lista, file1);
    add_list_ordered(lista, file2);
    add_list_tail(lista, file3);
    add_list_tail(lista, file4);
    add_list_tail(lista, file5);

    arq = open_archiver(argv[1]);
    struct list_t *alternativa = make_list();
    //escreve
    for (int i = 0; i < 6; i++) {
        insert_file(arq, argv[i+2]);
    }
    //lê
    printf("Lendo direto do arquivo:\n");
    rewind(arq);
    while (! feof(arq)) {
        struct file_header_t temp;
        fread(&temp, sizeof(struct file_header_t), 1, arq);
        write_file_data(&temp);
    }

    delete_list(lista);
    fclose(arq);

    return 0;
}
