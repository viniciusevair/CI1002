#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include "lib_arquivos.h"
#include "lib_lista.h"
#include "lib_vina.h"

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
    if(list == NULL)
        return NULL;

    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t temp;

    while(fread(&temp, sizeof(struct file_header_t), 1, archive) == 1)
        add_list_tail(list, &temp);

    return list;
}

/*
 * Recebe um arquivo, uma lista encadeada e um ponteiro para uma posição no
 * arquivo. Grava no arquivo a partir da posição recebida as informações
 * presentes na lista encadeada. Libera a memória associada a cada membro dentro
 * da lista no processo, deixando-a vazia.
 */
void store_list(FILE *archive, struct list_t *list, size_t archive_pointer) {
    fseek(archive, archive_pointer, SEEK_SET);
    struct file_header_t *file_data;

    while((file_data = get_first_element(list))) {
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

    while(file_size > 0) {
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
    while(remaining_bytes > 0) {
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

    // Calcula o tamanho do segmento a ser deslocado.
    remaining_bytes = end_point - start_point;

    /*
     * Copia os dados a partir do start_point até o end_point em blocos de
     * BUFFER_SIZE(1024) bytes, de trás pra frente, para o start_point +
     * shift_size, realizando o deslocamento de shift_size para a direita e
     * abrindo assim espaços para serem escritos sem sobrescrever informações.
     */
    while(remaining_bytes > 0) {
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

/*
 * Adiciona um membro ao fim do arquivo. Retorna 1 em caso de sucesso e 0 em
 * caso de não conseguir abrir o membro em stream para ser lido.
 */
int add_file_end(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data;
    char *new_filepath;
    FILE *member;

    file_data = get_data(filename);
    if(file_data->mode & S_IFDIR) {
        fprintf(stderr, "O arquivo \"%s\" é um diretório, o que não é ", filename);
        fprintf(stderr, "suportado pelo arquivador.\n");
        return 0;
    }
    new_filepath = relativize_filepath(filename);
    strcpy(file_data->filename, new_filepath);

    member = open_member(filename);
    if(member == NULL)
        return 0;

    copy_file_content(member, archive, file_data->size);

    /*
     * Guarda o ponto inicial de escrita nos metadados do membro e em seguida
     * atualiza a posição do ponteiro do arquivo.
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
 * Atualiza o membro filename dentro do arquivo para sua versão mais recente.
 * Basicamente, troca o membro dentro do arquivo pelo atual membro de mesmo nome
 * que está presente em disco. Retorna 1 em caso de sucesso e 0 em caso de não
 * conseguir abrir o membro em stream para leitura.
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

    // Abre o espaço apropriado para o membro novo dentro do arquivo.
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

// Remove um membro de dentro do arquivo. Retorna o novo fim do arquivo.
size_t remove_file(FILE *archive, struct list_t *list, struct file_header_t *file) {
    size_t read_point, new_eof;

    fseek(archive, file->archive_position + file->size, SEEK_SET);
    read_point = ftell(archive);

    new_eof = shift_bytes_left(archive, read_point, file->size);
    remove_element(list, file->filename);

    return new_eof;
}

/* -------- Update -------- */

/*
 * Verifica a existência do membro dentro do arquivo e, em caso positivo,
 * compara as datas de modificação. Substitui o membro presente no arquivo pelo
 * novo membro caso o novo membro tenha uma data de modificação maior (i. e.,
 * mais recente). Se o membro não existir, simplesmente o insere no arquivo.
 * Retorna 1 em caso de sucesso e 0 em casos de erros.
 */
int update_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    struct file_header_t *file_data;
    time_t old_mdate;

    /*
     * A função retornar tempo negativo implica que o membro não está presente
     * no arquivo, e portanto, será normalmente inserido ao fim do arquivo.
     */
    if((old_mdate = get_element_modif_time(list, filename)) < 0)
        return add_file_end(archive, list, filename, archive_pointer);

    file_data = get_data(filename);
    if(old_mdate < file_data->modif_date)
        return change_file_present(archive, list, filename, archive_pointer);

    return 0;
}

/*
 * Verifica se o arquivo existe. Em caso negativo, chama a operação de inserção
 * e procede como se o usuário tivesse usado a opção -i como argumento. Em caso
 * positivo, chama a função de atualização do membro para cada membro passado
 * como argumento na linha de comando. Retorna 1 em caso de sucesso e 0 caso não
 * consiga montar a lista encadeada para ler/inserir os membros.
 */
int update_operation(FILE *archive, char **argv, int argument_count) {
    struct list_t *list;
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        if(list == NULL)
            return 0;
        fseek(archive, archive_pointer, SEEK_SET);
    } else {
        return insert_operation(archive, argv, argument_count);
    }

    for(int i = ARGUMENT_OFFSET; i < argument_count; i++)
        if(! update_file(archive, list, argv[i], &archive_pointer)) {
            fprintf(stderr, "Não foi possível atualizar o arquivo \"%s\". ", argv[i]);
            fprintf(stderr, "Data de modificação menor que a presente em \"%s\".\n", argv[2]);
        }

    fseek(archive, 0, SEEK_SET);
    fwrite(&archive_pointer, sizeof(size_t), 1, archive);

    store_list(archive, list, archive_pointer);
    list = delete_list(list);

    return 1;
}

/* -------- Insert -------- */

/*
 * Insere um membro no arquivo. Se um membro de mesmo nome já estiver presente
 * dentro do arquivo, apenas o substitui (i. e., guarda na mesma posição que o
 * anterior). Caso não exista, insere ao fim do arquivo o novo membro. Retorna 1
 * em caso de sucesso e 0 caso não consiga montar a lista encadeada para ler os
 * membros.
 */
int insert_file(FILE *archive, struct list_t *list, char *filename, size_t *archive_pointer) {
    if(is_element_present(list, filename))
        return change_file_present(archive, list, filename, archive_pointer);

    return add_file_end(archive, list, filename, archive_pointer);
}

/*
 * Cria o arquivo caso este ainda não exista.
 * Chama a função de inserção do membro para cada membro passado
 * como argumento na linha de comando. Retorna 1 em caso de sucesso e 0 caso não
 * consiga montar a lista encadeada para ler/inserir os membros.
 */
int insert_operation(FILE *archive, char **argv, int argument_count) {
    struct list_t *list;
    size_t archive_pointer;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        if(list == NULL)
            return 0;
        fseek(archive, archive_pointer, SEEK_SET);
    } else {
        list = make_list();
        if(list == NULL)
            return 0;
        archive_pointer = sizeof(size_t);
        fwrite(&archive_pointer, sizeof(size_t), 1, archive);
    }

    for(int i = ARGUMENT_OFFSET; i < argument_count; i++)
        if(! insert_file(archive, list, argv[i], &archive_pointer))
            fprintf(stderr, "Não foi possível incluir o arquivo \"%s\"\n", argv[i]);

    fseek(archive, 0, SEEK_SET);
    fwrite(&archive_pointer, sizeof(size_t), 1, archive);

    store_list(archive, list, archive_pointer);
    list = delete_list(list);

    return 1;
}

/* -------- Move -------- */

size_t get_target_end(struct list_t *list, struct file_header_t *target_data) {
    return target_data->archive_position + target_data->size;
}

/*
 * Move um membro para a posição logo após o move_point e modifica sua ordem de
 * acordo com a nova posição. Retorna 0 caso o membro não esteja presente no
 * arquivo e 1 em caso de sucesso.
 */
int move_file(FILE *archive, struct list_t *list, char *filename, size_t move_point, size_t order) {
    struct file_header_t *file_data = seek_element(list, filename);
    size_t write_point, read_point, remaining_bytes, read_size, bytes_read;
    char buffer[BUFFER_SIZE];

    if(file_data == NULL)
        return 0;

    read_point = file_data->archive_position;
    write_point = move_point;

    /*
     * Ajusta a futura ordem no arquivo, a posição de leitura e a posição antiga
     * dentro do arquivo caso o membro esteja para frente do target, pois ele
     * será afetado pelo shift.
     */
    if(read_point > write_point) {
        file_data->archive_position += file_data->size;
        read_point += file_data->size;
        order++;
    }
    shift_bytes_right(archive, write_point, file_data->size);

    remaining_bytes = file_data->size;
    // Move o membro em blocos de tamanho BUFFER_SIZE.
    while(remaining_bytes > 0) {
        read_size = min(remaining_bytes, BUFFER_SIZE);

        fseek(archive, read_point, SEEK_SET);
        bytes_read = fread(buffer, sizeof(char), read_size, archive);

        fseek(archive, write_point, SEEK_SET);
        fwrite(buffer, sizeof(char), bytes_read, archive);

        read_point += bytes_read;
        write_point += bytes_read;
        remaining_bytes -= bytes_read;
    }

    // Tira o membro da antiga posição da lista.
    remove_file(archive, list, file_data);

    // Atualiza os metadados e guarda novamente no lugar novo na lista.
    file_data->archive_position = move_point;
    file_data->order = order;
    add_list_ordered(list, file_data);

    return 1;
}

/*
 * Move todos os membros para a posição depois do membro target. A ordem final é
 * de acordo com a ordem passada na linha de comando e não de acordo com a ordem
 * anteriormente guardada na lista. I. e., se os membros 1, 2, 3 e 4 existem
 * nessa ordem, "-m 3 archive 2 1" deixará a posição final como 3, 2, 1 e 4.
 * Retorna 1 em caso de sucesso e 0 caso não consiga montar a lista encadeada
 * para leitura/escrita ou caso o membro target não exista.
 */
int move_operation(FILE *archive, char **argv, int argument_count, char *target) {
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
    if(target_data == NULL) {
        fprintf(stderr, "Não foi possível encontrar o target \"%s\".\n", target);
        list = delete_list(list);
        return 0;
    }
        
    move_point = get_target_end(list, target_data);
    order = target_data->order;

    for(int i = ARGUMENT_OFFSET + 1; i < argument_count; i++) {
        filename = relativize_filepath(argv[i]);
        if(strcmp(filename, target_filename) == 0)
            continue;
        if(! move_file(archive, list, filename, move_point, order))
            fprintf(stderr, "Não foi possível mover o arquivo %s\n", argv[i]);

        /*
         * Atualiza as informações para que o membro recém movido seja o novo
         * target.
         */
        target_data = seek_element(list, filename);
        move_point = get_target_end(list, target_data);
        order = target_data->order;
    }

    store_list(archive, list, archive_pointer);

    list = delete_list(list);
    return 1;
}

/* -------- Extract -------- */

/*
 * Extrai um membro de dentro do arquivo. Utiliza a struct com os metadados para
 * não ter que caminhar pela lista duas vezes (verificando e depois extraindo).
 * Restaura as permissões e data de modificação originais do arquivo.
 */
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

/*
 * Extrai todos os membros presentes na lista encadeada. Libera a memória
 * associada a cada membro dentro da lista no processo, deixando-a vazia.
 */
void extract_all(FILE *archive, struct list_t *list, size_t archive_pointer) {
    struct file_header_t *temp;

    while((temp = get_first_element(list))) {
        extract_file(archive, temp);
        free(temp);
    }
}

/*
 * Extrai do arquivo os membros especificados na linha de comando. Se nenhum
 * membro for especificado, extrai todos os membros do arquivo.
 * Não faz nenhuma modificação no arquivo.
 */
int extract_operation(FILE *archive, char **argv, int argument_count) {
    struct list_t *list;
    size_t archive_pointer;
    char *filename;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);

        if(argument_count == 3) {
            extract_all(archive, list, archive_pointer);
        } else if(argument_count > 3) {
            for(int i = ARGUMENT_OFFSET; i < argument_count; i++) {
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

/*
 * Remove os membros especificados na linha de comando. Retorna 1 em caso de
 * sucesso e 0 em caso de não conseguir abrir a lista de membros para
 * escrita/leitura ou não for especificado nenhum membro a ser removido.
 */
int remove_operation(FILE *archive, char **argv, int argument_count) {
    size_t archive_pointer;
    char *filename;

    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        struct list_t *list;
        list = load_list(archive, archive_pointer);
        if(list == NULL)
            return 0;

        if(argument_count == 3) {
            fprintf(stderr, "Não foi especificado nenhum arquivo para a exclusão.\n");
            list = delete_list(list);
            return 0;
        } else if(argument_count > 3) {
            /*
             * Remove a lista do fim do arquivo para evitar ficar movendo bytes
             * que seriam sobrescritos ou truncados de qualquer forma.
             */
            ftruncate(fileno(archive), archive_pointer);

            for(int i = ARGUMENT_OFFSET; i < argument_count; i++) {
                filename = relativize_filepath(argv[i]);
                struct file_header_t *file = seek_element(list, filename);
                if(file != NULL) {
                    remove_file(archive, list, file);
                    free(file);
                } else {
                    fprintf(stderr, "O arquivo \"%s\" não foi encontrado em \"%s\".\n", argv[i], argv[2]);
                }

                free(filename);
            }
        }

        fseek(archive, 0, SEEK_SET);
        fwrite(&archive_pointer, sizeof(size_t), 1, archive);

        ftruncate(fileno(archive), archive_pointer);
        fseek(archive, 0, SEEK_END);
        store_list(archive, list, archive_pointer);

        list = delete_list(list);

        return 1;
    }

    return 0;
}

/* -------- List -------- */

/*
 * Lista os membros presentes no arquivo. A saida é exatamente da mesma forma
 * que a do tar: MODE USER/GROUP SIZE MODIF.DATE FILENAME
 */
void list_operation(FILE *archive) {
    struct list_t *list;
    size_t archive_pointer;
    if(fread(&archive_pointer, sizeof(size_t), 1, archive)) {
        list = load_list(archive, archive_pointer);
        read_list(list);
    } else {
        fprintf(stderr, "Arquivo não encontrado para leitura.\n");
    }
}

/* -------- Help -------- */

// Imprime o manual do vina++ na tela.
void help_utility(char *call) {
    printf("Uso:\n");
    printf("%s [-i | -a | -m <target> | -x | -r | -c | -h]", call);
    printf(" <archive> [membro1 membro2 ...]\n");
    printf("\n");
    printf("Opções:\n");
    printf("-i \t\tInsere/acrescenta os membros ao <archive>. Se o membro já ");
    printf("exista no archive ele será substituído. Membros novos são ");
    printf("adicionados ao fim do arquivo.\n");
    printf("-a \t\tMesmo comportamento de -i, porém só substitui em caso do ");
    printf("membro possuir uma data de modificação maior que a já presente ");
    printf("no arquivo.\n");
    printf("-m <target> \tMove os membros especificados para imediatamente ");
    printf("depois de <target> no archive.\n");
    printf("-x \t\tExtrai os membros indicados do <archive>. Caso nenhum ");
    printf("membro seja especificado, extrai todos os membros.\n");
    printf("-r \t\tRemove os membros indicados do <archive>.\n");
    printf("-c \t\tImprime os metadados dos membros do <archive> na tela na ");
    printf("seguinte ordem: MODE USER/GROUP SIZE MODIF.DATE FILENAME\n");
    printf("-h \t\tImprime esta mensagem de ajuda na tela.\n");
}
