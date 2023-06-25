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
    char buffer[BUFFER_SIZE];
    size_t read_point, write_point, remaining_bytes, bytes_read, end_point;
    size_t read_size;

    read_point = shift_point;
    write_point = read_point - shift_size;

    // Calcula o tamanho do segmento que vai ser deslocado.
    fseek(archive, 0, SEEK_END);
    end_point = ftell(archive);
    remaining_bytes = end_point - read_point;

    /*
     * Copia os dados a partir do read_point até o fim do arquivo em blocos de
     * BUFFER_SIZE(1024) bytes para o write_point, realizando o deslocamento de
     * shift_size para a esquerda.
     */
    while (remaining_bytes > 0) {
        read_size = min(remaining_bytes, BUFFER_SIZE);

        fseek(archive, read_point, SEEK_SET);
        bytes_read = fread(buffer, sizeof(char), read_size, archive);

        fseek(archive, write_point, SEEK_SET);
        fwrite(buffer, sizeof(char), bytes_read, archive);

        read_point += bytes_read;
        write_point += bytes_read;
        remaining_bytes -= bytes_read;
    }

    // Trunca o arquivo jogando fora os bytes ao final que não foram sobrescritos.
    ftruncate(fileno(archive), ftell(archive));

    return ftell(archive);
}

/*
 * Desloca todos os bytes do arquivo a partir do ponto shift_point uma
 * quantidade de shift_size bytes para a direita. Retorna um size_t indicando a
 * posição em que a escrita terminou no archive.
 */
size_t shift_bytes_right(FILE *archive, size_t shift_point, size_t shift_size) {
    char buffer[BUFFER_SIZE];
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
     * abrindo assim espaços para serem escritos sem sobrescrever informações.
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

// Adiciona um membro ao fim do arquivador.
int add_file_end(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data;
    char *new_filepath;
    FILE *member;

    file_data = get_data(filename);
    new_filepath = relativize_filepath(filename);
    strcpy(file_data->filename, new_filepath);

    member = open_member(filename);
    if(member == NULL)
        return 0;

    copy_file_content(member, archive, file_data->size);

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

/*
 * Atualiza o membro "filename" dentro do arquivador para sua versão
 * mais recente. Basicamente, troca o membro dentro do arquivador pelo atual
 * membro de mesmo nome que está presente em disco.
 */
int change_file_present(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *new_data, *old_data;
    char *new_filepath;
    FILE *member;
    int size_diff;
    size_t file_end;

    new_data = get_data(filename);
    new_filepath = relativize_filepath(filename);
    strcpy(new_data->filename, new_filepath);

    old_data = remove_element(list, new_data->filename);
    size_diff = new_data->size - old_data->size;
    file_end = old_data->archive_position + old_data->size;

    // Abre o espaço apropriado para o membro novo dentro do arquivador.
    if(size_diff > 0)
        shift_bytes_right(archive, old_data->archive_position, size_diff);
    else if(size_diff < 0)
        shift_bytes_left(archive, file_end, -size_diff);

    member = open_member(filename);
    if(member == NULL)
        return 0;

    fseek(archive, old_data->archive_position, SEEK_SET);
    copy_file_content(member, archive, new_data->size);

    new_data->order = old_data->order;
    *archive_pointer += size_diff;
    add_list_ordered(list, new_data);

    free(new_data);
    free(old_data);
    free(new_filepath);
    fclose(member);

    return 1;
}

size_t remove_file(FILE *archive, struct list_t *list, struct file_header_t *file) {
    size_t read_point, new_eof;

    fseek(archive, file->archive_position + file->size, SEEK_SET);
    read_point = ftell(archive);

    new_eof = shift_bytes_left(archive, read_point, file->size);
    remove_element(list, file->filename);

    return new_eof;
}

/* -------- Update -------- */

int update_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data;
    time_t old_mdate;

    /*
     * A função retornar tempo negativo implica que o membro não está presente
     * no arquivador, e portanto, será normalmente inserido ao fim do arquivo.
     */
    if((old_mdate = get_element_modif_time(list, filename)) < 0)
        return add_file_end(archive, list, filename, archive_pointer);

    file_data = get_data(filename);
    if(old_mdate < file_data->modif_date)
        return change_file_present(archive, list, filename, archive_pointer);

    return 0;
}

void update_operation(FILE *archive, char **argv, int members_quantity) {
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
        if(! update_file(archive, list, argv[i], &archive_pointer)) {
            fprintf(stderr, "Não foi possível atualizar o arquivo \"%s\". ", argv[i]);
            fprintf(stderr, "Data de modificação menor que a presente em \"%s\".\n", argv[2]);
        }

    fseek(archive, 0, SEEK_SET);
    fwrite(&archive_pointer, sizeof(size_t), 1, archive);

    store_list(archive, list, archive_pointer);
    list = delete_list(list);
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
    if(is_element_present(list, filename))
        return change_file_present(archive, list, filename, archive_pointer);

    return add_file_end(archive, list, filename, archive_pointer);
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
size_t get_target_end(struct list_t *list, struct file_header_t *target_data) {
    return target_data->archive_position + target_data->size;
}

int move_file(FILE *archive, struct list_t *list, char *filename, size_t move_point, size_t order) {
    printf("Testezera:\n");
    read_list(list);
    struct file_header_t *file_data = seek_element(list, filename);
    size_t write_point, read_point, remaining_bytes, read_size, bytes_read;
    char buffer[BUFFER_SIZE];

    // file_data null implica que o arquivo não existe no arquivador. 
    if(file_data == NULL)
        return 0;

    read_point = file_data->archive_position;
    write_point = move_point;

    /*
     * Ajusta a posição de leitura e a posição antiga dentro do arquivador caso
     * estes estejam para frente do target, pois serão afetados pelo shift.
     * Também aumenta o contador de ordem, pois se o arquivo estiver pra frente
     * do target, ele fica na próxima posição, mas se for antes do target, ele
     * vai tomar a posição do target (que vai receber -1 na ordem).
     */
    if(read_point > write_point) {
        file_data->archive_position += file_data->size;
        read_point += file_data->size;
        order++;
    }
    shift_bytes_right(archive, write_point, file_data->size);

    remaining_bytes = file_data->size;
    while (remaining_bytes > 0) {
        read_size = min(remaining_bytes, BUFFER_SIZE);

        fseek(archive, read_point, SEEK_SET);
        printf("ftell do read: %ld\n", ftell(archive));
        bytes_read = fread(buffer, sizeof(char), read_size, archive);
        printf("buffer text: %s\n", buffer);

        fseek(archive, write_point, SEEK_SET);
        fwrite(buffer, sizeof(char), bytes_read, archive);

        read_point += bytes_read;
        write_point += bytes_read;
        remaining_bytes -= bytes_read;
    }

    remove_file(archive, list, file_data);

    file_data->archive_position = move_point;
    file_data->order = order;
    add_list_ordered(list, file_data);

    return 1;
}

int move_operation(FILE *archive, char **argv, int members_quantity, char *target) {
    struct file_header_t *target_data;
    struct list_t *list;
    size_t archive_pointer, move_point, order;
    char *filename, *target_filename;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        fseek(archive, archive_pointer, SEEK_SET);
    } else {
        return 0;
    }

    target_filename = relativize_filepath(target);
    target_data = seek_element(list, target_filename);
    move_point = get_target_end(list, target_data);
    order = target_data->order;

    for (int i = ARGUMENT_OFFSET + 1; i < members_quantity; i++) {
        filename = relativize_filepath(argv[i]);
        if(strcmp(filename, target_filename) == 0)
            continue;
        if(! move_file(archive, list, filename, move_point, order))
            fprintf(stderr, "Não foi possível mover o arquivo %s\n", argv[i]);

        target_data = seek_element(list, filename);
        move_point = get_target_end(list, target_data);
        order = target_data->order;
    }

    store_list(archive, list, archive_pointer);

    list = delete_list(list);
    return 1;
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
    char *filename;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);

        if(members_quantity == 3) {
            extract_all(archive, list, archive_pointer);
        } else if (members_quantity > 3) {
            for (int i = ARGUMENT_OFFSET; i < members_quantity; i++) {
                filename = relativize_filepath(argv[i]);
                struct file_header_t *file_data = seek_element(list, filename);
                if(file_data != NULL)
                    extract_file(archive, file_data);
                else
                    fprintf(stderr, "Não foi possível extrair o arquivo \"%s\".\n", argv[i]);
                free(filename);
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
                    remove_file(archive, list, file);
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
    struct list_t *list;
    size_t archive_pointer;
    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        read_list(list);
    }
}

/* -------- Help -------- */
void help_utility(char *call) {
    printf("Uso:\n");
    printf("%s [-i | -a | -m <target> | -x | -r | -c | -h]", call);
    printf(" <archive> [membro1 membro2 ...]\n");
}
