#include <stdio.h>
#include <stdlib.h>
#include "libArquivos.h"
#include "libLista.h"
#include "libVina.h"



void insert_file(FILE *archiver, char *filename) {
    struct file_header_t *file_data = get_data(filename);
    FILE *member = open_member(filename);
    size_t position;
    //get a way to read size always
    fread(void *, unsigned long, unsigned long, FILE *)
    fwrite(&file_data->size, sizeof(size_t), 1, archiver);
    fwrite(member, sizeof(char), file_data->size, archiver);
    fwrite(file_data, sizeof(struct file_header_t), 1, archiver);
    fclose(member);
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
    printf("TESTE: %zu\n", directory_position);
    fseek(archiver, directory_position, SEEK_CUR);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archiver) == 1) {
        write_file_data(&temp);
    }
}
//void list_files(FILE *archiver) {
//    size_t directory_position;
//    fread(&directory_position, sizeof(size_t), 1, archiver);
//    printf("TESTE: %zu\n", directory_position);
//    fseek(archiver, directory_position, SEEK_SET);
//    while (! feof(archiver)) {
//        struct file_header_t temp;
//        fread(&temp, sizeof(struct file_header_t), 1, archiver);
//        write_file_data(&temp);
//    }
//}

void help_utility(char *call) {
    printf("Uso:\n");
    printf("%s [-i | -a | -m <target> | -x | -r | -c | -h]", call);
    printf(" <archive> [membro1 membro2 ...]\n");
}
