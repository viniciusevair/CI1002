#include <stdio.h>
#include <stdlib.h>
#include "libArquivos.h"
#include "libLista.h"
#include "libVina.h"

#define INSERT_OFFSET 3

struct list_t* load_list(FILE *archiver, struct list_t *list, size_t directory_position) {
    fseek(archiver, directory_position, SEEK_CUR);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archiver) == 1)
        add_list_tail(list, &temp);

    return list;
}

struct list_t* store_list(FILE *archiver, struct list_t *list, size_t directory_position) {
    fseek(archiver, directory_position, SEEK_CUR);
    struct file_header_t *temp;
    
    while ((temp = get_first_element(list))) {
        fwrite(temp, sizeof(struct file_header_t), 1, archiver);
        free(temp);
    }

    return list;
}

void insert_file(FILE *archiver, struct list_t *list, char *filename, size_t *directory_position) {
    struct file_header_t *file_data = get_data(filename);
    FILE *member = open_member(filename);

    file_data->archive_position = *directory_position;
    *directory_position += file_data->size;
    fwrite(member, sizeof(char), file_data->size, archiver);
    fwrite(file_data, sizeof(struct file_header_t), 1, archiver);
    fclose(member);

    add_list_tail(list, file_data);
}

void insert_operation(FILE *archiver, char **argv, int members_quantity) {
    struct list_t *list = make_list();
    size_t directory_position;

    if(fread(&directory_position, sizeof(size_t), 1, archiver)) {
        load_list(archiver, list, directory_position);
        fseek(archiver, directory_position, SEEK_CUR);
    } else {
        directory_position = sizeof(size_t);
        fwrite(&directory_position, sizeof(size_t), 1, archiver);
    }

    for (int i = INSERT_OFFSET; i < members_quantity; i++)
        if(! is_element_present(list, argv[i]))
            insert_file(archiver, list, argv[i], &directory_position);
    fseek(archiver, 0, SEEK_SET);
    fwrite(&directory_position, sizeof(size_t), 1, archiver);
    store_list(archiver, list, directory_position);
}

//void update_file(FILE *archiver, struct list_t *directory, char *filename) {
//}
//
//void move_file(FILE *archiver, struct list_t *directory, char *filename) {
//}
//
//void extract_file(FILE *archiver, struct list_t *directory, char *filename) {
//}
//
//void remove_file(FILE *archiver, struct list_t *directory, char *filename) {
//}

void list_files(FILE *archiver) {
    size_t directory_position;
    fread(&directory_position, sizeof(size_t), 1, archiver);
    printf("Pequeno teste: %lu\n", sizeof(size_t));
    fseek(archiver, directory_position, SEEK_CUR);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archiver) == 1) {
        write_file_data(&temp);
        printf("Binary position: %zu\n", temp.archive_position);
    }
}

void help_utility(char *call) {
    printf("Uso:\n");
    printf("%s [-i | -a | -m <target> | -x | -r | -c | -h]", call);
    printf(" <archive> [membro1 membro2 ...]\n");
}
