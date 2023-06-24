#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

// Recebe dois valores inteiros positivos. Devolve o menor deles.
size_t min(size_t x, size_t y) {
    return (x <= y) ? x : BUFFER_SIZE;
}

/*
 * Recebe um arquivo e um ponteiro para uma posição dentro deste arquivo.
 * Retorna os dados presentes a partir desta posição na forma de uma lista
 * encadeada de struct file_header_t.
 */
struct list_t* load_list(FILE *archive, size_t archive_pointer) {
    struct list_t *list = make_list();
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t temp;
    
    while (fread(&temp, sizeof(struct file_header_t), 1, archive) == 1)
        add_list_tail(list, &temp);

    return list;
}

/*
 * Recebe um arquivo, uma lista encadeada e um ponteiro para uma posição no
 * arquivo. Grava no arquivo a partir da posição recebida as informações
 * presentes na lista encadeada.
 */
void store_list(FILE *archive, struct list_t *list, size_t archive_pointer) {
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t *file_data;
    
    while ((file_data = get_first_element(list))) {
        fwrite(file_data, sizeof(struct file_header_t), 1, archive);
        free(file_data);
    }
}

/*
 * Escreve file_size bytes do arquivo input no arquivo output.
 * Assume que os ponteiros em ambos os arquivos já foram devidamente
 * posicionados nos pontos de leitura e escrita.
 */
void copy_file_content(FILE *input, FILE *output, size_t file_size) {
    char buffer[BUFFER_SIZE];
    size_t read_size, bytes_read;

    while (file_size > 0) {
        read_size = min(file_size, BUFFER_SIZE);
        bytes_read = fread(buffer, sizeof(char), read_size, input);
        fwrite(buffer, sizeof(char), bytes_read, output);
        file_size -= bytes_read;
    }
}

/*
 * Desloca todos os bytes do arquivo a partir do ponto shift_point uma
 * quantidade de shift_size bytes para a esquerda.
 * Retorna a posição do ponteiro ao fim do shift.
 */
size_t shift_bytes_left(FILE *archive, size_t shift_point, size_t shift_size) {
    char buffer[BUFFER_SIZE + 1];
    size_t read_point, write_point, remaining_bytes, bytes_read, end_point;

    read_point = shift_point;
    write_point = read_point - shift_size;

    // Calcula o tamanho do segmento que vai ser deslocado.
    fseek(archive, 0, SEEK_END);
    end_point = ftell(archive);
    remaining_bytes = end_point - read_point;

    /*
     * Copia os dados a partir do read_point até o fim do arquivo em blocos de
     * BUFFER_SIZE(1024) bytes para o write_point, realizando o deslocamento de
     * shift_size para a ESQUERDA.
     */
    while (remaining_bytes > 0) {
        fseek(archive, read_point, SEEK_SET);
        bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, archive);

        fseek(archive, write_point, SEEK_SET);
        fwrite(buffer, sizeof(char), bytes_read, archive);

        read_point += bytes_read;
        write_point += bytes_read;
        remaining_bytes -= bytes_read;
    }

    ftruncate(fileno(archive), ftell(archive));

    return ftell(archive);
}

/*
 * Desloca todos os bytes do arquivo a partir do ponto shift_point uma
 * quantidade de shift_size bytes para a direita.
 */
size_t shift_bytes_right(FILE *archive, size_t shift_point, size_t shift_size) {
    char buffer[BUFFER_SIZE + 1];
    size_t remaining_bytes, bytes_read, read_size, start_point, end_point;
    size_t read_point, write_point;

    // Acrescenta espaços suficientes ao fim do arquivo.
    fseek(archive, 0, SEEK_END);
    end_point = ftell(archive);
    start_point = shift_point;
    ftruncate(fileno(archive), end_point + shift_size);

    // Calcula o tamanho do segmento a ser deslocado.
    remaining_bytes = end_point - start_point;

    /*
     * Copia os dados a partir do start_point até o end_point em blocos de
     * BUFFER_SIZE(1024) bytes, de trás pra frente, para o start_point +
     * shift_size, realizando o deslocamento de shift_size para a direita e
     * abrindo assim espaços para serem sobrescritos de maneira segura.
     */
    while (remaining_bytes > 0) {
        read_size = min(remaining_bytes, BUFFER_SIZE);
        read_point = start_point + remaining_bytes - read_size;
        write_point = read_point + shift_size;

        fseek(archive, read_point, SEEK_SET);
        bytes_read = fread(buffer, sizeof(char), read_size, archive);

        fseek(archive, write_point, SEEK_SET);
        fwrite(buffer, sizeof(char), bytes_read, archive);

        remaining_bytes -= bytes_read;
    }

    return ftell(archive);
}


/* -------- Update -------- */
//int update_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
//}

void update_operation(FILE *archive, char **argv, int members_quantity) {
}

/* -------- Insert -------- */

/*
 * Recebe um arquivo aberto em stream (archive), um ponteiro para uma posição neste arquivo,
 * uma lista encadeada e o nome de um arquivo (filename).
 * Se o arquivo existir, guarda seus metadados na lista encadeada e o binário no
 * arquivo aberto em stream (archive).
 * Vale notar que 
 */
int insert_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data = get_data(filename);
    char *new_filepath = relativize_filepath(filename);
    strcpy(file_data->filename, new_filepath);
    if(is_element_present(list, file_data->filename)) {
        free(file_data);
        free(new_filepath);
        return 0;
        //arrumar pro valor de retorno ser o update file
        //return update_file(archive, list, filename, archive_pointer);
    }

    FILE *member = open_member(filename);
    if(member == NULL)
        return 0;

    size_t file_size = file_data->size;

    copy_file_content(member, archive, file_size);

    /*
     * Guarda o ponto inicial de escrita nos metadados do arquivo e em seguida
     * atualiza a posição do ponteiro do arquivador.
     */
    file_data->archive_position = *archive_pointer;
    (*archive_pointer) += file_data->size;

    add_list_tail(list, file_data);

    free(file_data);
    free(new_filepath);
    fclose(member);

    return 1;
}

void insert_operation(FILE *archive, char **argv, int members_quantity) {
    struct list_t *list;
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        fseek(archive, archive_pointer, SEEK_SET);
    } else {
        list = make_list();
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
void move_operation(FILE *archive, char **argv, int members_quantity) {
}

/* -------- Extract -------- */
void extract_file(FILE *archive, struct file_header_t *file_data) {
    FILE *member = make_member(file_data->filename);
    size_t file_size = file_data->size;
    struct utimbuf original_time;

    /*
     * A função make_member já tratou o erro. Basta agora retornar para avançar
     * para a próxima iteração do laço (i. e., próximo arquivo a se extrair).
     */
    if(member == NULL)
        return;

    fseek(archive, file_data->archive_position, SEEK_SET);
    copy_file_content(archive, member, file_size);

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

int extract_operation(FILE *archive, char **argv, int members_quantity) {
    struct list_t *list;
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);

        if(members_quantity == 3) {
            extract_all(archive, list, archive_pointer);
        } else if (members_quantity > 3) {
            for (int i = ARGUMENT_OFFSET; i < members_quantity; i++) {
                struct file_header_t *file = seek_element(list, argv[i]);
                if(file != NULL)
                    extract_file(archive, file);
                else
                    fprintf(stderr, "Não foi possível extrair o arquivo \"%s\".\n", argv[i]);
            }
        }

        list = delete_list(list);
    } else {
        fprintf(stderr, "Não foi possível fazer a leitura do arquivo %s.\n", argv[2]);
        return 0;
    }

    return 1;
}

/* -------- Remove -------- */
size_t delete_file(FILE *archive, struct list_t *list, struct file_header_t *file) {
    size_t read_point, new_eof;

    fseek(archive, file->archive_position + file->size, SEEK_SET);
    read_point = ftell(archive);

    new_eof = shift_bytes_left(archive, read_point, file->size);
    remove_element(list, file->filename);

    return new_eof;
}

void remove_operation(FILE *archive, char **argv, int members_quantity) {
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        struct list_t *list;
        list = load_list(archive, archive_pointer);
        ftruncate(fileno(archive), archive_pointer);

        if(members_quantity == 3) {
            fprintf(stderr, "Não foi especificado nenhum arquivo para a exclusão.\n");
        } else if (members_quantity > 3) {
            for (int i = ARGUMENT_OFFSET; i < members_quantity; i++) {
                struct file_header_t *file = seek_element(list, argv[i]);
                if(file != NULL) {
                    delete_file(archive, list, file);
                    free(file);
                }
                else
                    fprintf(stderr, "O arquivo %s não foi encontrado em %s.\n", argv[i], argv[2]);
            }
        }

        fseek(archive, 0, SEEK_SET);
        fwrite(&archive_pointer, sizeof(size_t), 1, archive);
        ftruncate(fileno(archive), archive_pointer);
        fseek(archive, 0, SEEK_END);
        store_list(archive, list, archive_pointer);
        list = delete_list(list);
    } else {
        fprintf(stderr, "Não foi possível fazer a leitura do arquivo %s.\n", argv[2]);
    }
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
