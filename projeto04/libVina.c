#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include "libArquivos.h"
#include "libLista.h"
#include "libVina.h"

#define ARGUMENT_OFFSET 3
#define BUFFER_SIZE 1024

/* -------- Utilities -------- */
struct list_t* load_list(FILE *archive, struct list_t *list, size_t archive_pointer) {
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archive) == 1)
        add_list_tail(list, &temp);

    return list;
}

struct list_t* store_list(FILE *archive, struct list_t *list, size_t archive_pointer) {
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t *file_data;
    
    while ((file_data = get_first_element(list))) {
        fwrite(file_data, sizeof(struct file_header_t), 1, archive);
        free(file_data);
    }

    return list;
}

void process_file(FILE *input, FILE *output, size_t file_size) {
    size_t buffer_size, read_size, bytes_read;
    buffer_size = BUFFER_SIZE;
    char buffer[buffer_size];

    while (file_size > 0) {
        read_size = (file_size < buffer_size)? file_size : buffer_size;
        bytes_read = fread(buffer, sizeof(char), read_size, input);
        fwrite(buffer, sizeof(char), bytes_read, output);
        file_size -= bytes_read;
    }
}

/* -------- Update -------- */
//int update_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
//}

void update_operation(FILE *archive, char **argv, int members_quantity) {
}

/* -------- Insert -------- */
int insert_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data = get_data(filename);
    strcpy(file_data->filename, relativize_filepath(filename));
    if(is_element_present(list, file_data->filename))
        return 0;
        //return update_file(archive, list, filename, archive_pointer);

    FILE *member = open_member(filename);
    if(member == NULL)
        return 0;

    size_t file_size = file_data->size;

    process_file(member, archive, file_size);
    file_data->archive_position = *archive_pointer;

    (*archive_pointer) += file_data->size;
    add_list_tail(list, file_data);
    fclose(member);

    return 1;
}

void insert_operation(FILE *archive, char **argv, int members_quantity) {
    struct list_t *list = make_list();
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        load_list(archive, list, archive_pointer);
        fseek(archive, archive_pointer, SEEK_SET);
    } else {
        archive_pointer = sizeof(size_t);
        fwrite(&archive_pointer, sizeof(size_t), 1, archive);
    }

    for (int i = ARGUMENT_OFFSET; i < members_quantity; i++)
        if(! insert_file(archive, list, argv[i], &archive_pointer))
            fprintf(stderr, "Não foi possível incluir o arquivo %s\n", argv[i]);

    fseek(archive, 0, SEEK_SET);
    fwrite(&archive_pointer, sizeof(size_t), 1, archive);

    store_list(archive, list, archive_pointer);
    list = delete_list(list);
}

/* -------- Move -------- */
void move_file(FILE *archive, char **argv, int members_quantity) {
}

/* -------- Extract -------- */
void extract_file(FILE *archive, struct file_header_t *file_data) {
    FILE *member = make_member(file_data->filename);
    size_t file_size = file_data->size;
    struct utimbuf original_time;

    if(member == NULL)
        return;

    fseek(archive, file_data->archive_position, SEEK_SET);
    process_file(archive, member, file_size);

    fclose(member);

    original_time.modtime = file_data->modif_date;
    original_time.actime = time(NULL);
    chmod(file_data->filename, file_data->mode);
    utime(file_data->filename, &original_time);
}

void extract_all(FILE *archive, struct list_t *list, size_t archive_pointer) {
    struct file_header_t *temp;
    
    while ((temp = get_first_element(list))) {
        extract_file(archive, temp);
        free(temp);
    }
}

void extract_operation(FILE *archive, char **argv, int members_quantity) {
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        struct list_t *list = make_list();
        load_list(archive, list, archive_pointer);

        if(members_quantity == 3) {
            extract_all(archive, list, archive_pointer);
        } else if (members_quantity > 3) {
            for (int i = ARGUMENT_OFFSET; i < members_quantity; i++) {
                struct file_header_t *file = seek_element(list, argv[i]);
                if(file != NULL)
                    extract_file(archive, file);
                else
                    fprintf(stderr, "Não foi possível extrair o arquivo %s.\n", argv[i]);
            }
        }

        list = delete_list(list);
    } else {
        fprintf(stderr, "Não foi possível fazer a leitura do arquivo %s.\n", argv[2]);
    }
}

/* -------- Remove -------- */
void remove_operation(FILE *archive, char **argv, int members_quantity) {
}

/* -------- List -------- */
void list_files(FILE *archive) {
    size_t archive_pointer;
    fread(&archive_pointer, sizeof(size_t), 1, archive);
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archive) == 1)
        write_file_data(&temp);
}

/* -------- Help -------- */
void help_utility(char *call) {
    printf("Uso:\n");
    printf("%s [-i | -a | -m <target> | -x | -r | -c | -h]", call);
    printf(" <archive> [membro1 membro2 ...]\n");
}
