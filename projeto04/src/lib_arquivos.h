/*
 * Biblioteca elaborada pelo aluno Vinicius Evair da Silva
 * para o projeto 04 da disciplina Programacao II (CI1002).
 */

#ifndef __ARQUIVOS
#define __ARQUIVOS

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

struct member_metadata_t {
    char filename[FILENAME_MAX];
    size_t order;
    uid_t user_id;
    gid_t group_id;
    mode_t mode;
    size_t size;
    time_t modif_date;
    size_t archive_position;
};

/*
 * Abre um arquivo em modo de leitura e escrita para ser manipulado. O caso de
 * falha ao abrir o arquivo indica que ele ainda não existe e portanto, a função
 * tenta criá-lo.
 */
FILE *open_archiver(char *filename);

/*
 * Cria um arquivo em disco com o nome filename e o abre para escrita.
 */
FILE *make_member(char *filename);

// Abre o arquivo filename em modo de leitura.
FILE *open_member(char *filename);

/*
 * Recebe como parâmetro o nome de um arquivo.
 * Guarda os metadados do arquivo em uma struct e a retorna em caso de sucesso,
 * ou retorna NULL em caso de falha. A responsabilidade por
 * desalocar a memória é de quem chamou a função.
 */
struct member_metadata_t *get_data(char *filename);

/*
 * Corrige os caminhos de arquivo para que todos tenham como endereço raíz o
 * diretório corrente. Por uma questão puramente estética, arquivos cujo caminho
 * final são o próprio diretório corrente e que foram passados de maneira não
 * absoluta (por ex.: ./arquivo_pequeno) são representados apenas com o seu
 * nome, sem indicador de caminho (por ex.: arquivo_pequeno).
 */
char *relativize_filepath(char *filename);

void print_file_data(struct member_metadata_t *file);

#endif
