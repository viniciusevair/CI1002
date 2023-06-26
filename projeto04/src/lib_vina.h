/*
 * Biblioteca elaborada pelo aluno Vinicius Evair da Silva
 * para o projeto 04 da disciplina Programacao II (CI1002).
 */

#ifndef __VINA
#define __VINA

#include <stdio.h>

/*
 * Cria o arquivo caso este ainda não exista.
 * Chama a função de inserção do membro para cada membro passado
 * como argumento na linha de comando. Retorna 1 em caso de sucesso e 0 caso não
 * consiga montar a lista encadeada para ler/inserir os membros.
 */
int insert_operation(FILE *archiver, char **argv, int members_quantity);

/*
 * Verifica se o arquivo existe. Em caso negativo, chama a operação de inserção
 * e procede como se o usuário tivesse usado a opção -i como argumento. Em caso
 * positivo, chama a função de atualização do membro para cada membro passado
 * como argumento na linha de comando. Retorna 1 em caso de sucesso e 0 caso não
 * consiga montar a lista encadeada para ler/inserir os membros.
 */
int update_operation(FILE *archiver, char **argv, int members_quantity);

/*
 * Move todos os membros para a posição depois do membro target. A ordem final é
 * de acordo com a ordem passada na linha de comando e não de acordo com a ordem
 * anteriormente guardada na lista. I. e., se os membros 1, 2, 3 e 4 existem
 * nessa ordem, "-m 3 archive 2 1" deixará a posição final como 3, 2, 1 e 4.
 * Retorna 1 em caso de sucesso e 0 caso não consiga montar a lista encadeada
 * para leitura/escrita ou caso o membro target não exista.
 */
int move_operation(FILE *archiver, char **argv, int members_quantity, char *target);

/*
 * Extrai do arquivo os membros especificados na linha de comando. Se nenhum
 * membro for especificado, extrai todos os membros do arquivo.
 * Não faz nenhuma modificação no arquivo.
 */
int extract_operation(FILE *archiver, char **argv, int members_quantity);

/*
 * Remove os membros especificados na linha de comando. Retorna 1 em caso de
 * sucesso e 0 em caso de não conseguir abrir a lista de membros para
 * escrita/leitura ou não for especificado nenhum membro a ser removido.
 */
int remove_operation(FILE *archiver, char **argv, int members_quantity);

/*
 * Lista os membros presentes no arquivo. A saida é exatamente da mesma forma
 * que a do tar: MODE USER/GROUP SIZE MODIF.DATE FILENAME
 */
void list_operation(FILE *archiver);

// Imprime o manual do vina++ na tela.
void help_utility(char *call);

#endif
