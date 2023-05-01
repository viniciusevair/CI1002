/*
 * Biblioteca implementada pelo aluno Vinicius Evair da Silva para o projeto 02
 * da disciplina Programacao II (CI1002).
 * Biblioteca com as funcoes para codificar uma mensagem em texto. A mensagem de
 * entrada nao precisa necessariamente estar em ASCII, mas a codificacao
 * removera todos os acentos das vogais e cedilhas.
 */

/* 
 * Guarda os dados da arvore em um arquivo de texto. O formato de cada linha da
 * saida e 
 * chave: *dados separados por espaco*.
 */
int imprimeChaves(char *arqChaves, struct tArvore *dados);

/* 
 * Recebe como entrada um arquivo com uma mensagem de texto e devolve em um
 * arquivo de saida a mensagem criptografada pela cifra de Beale. Por questoes
 * de compatibilidade, transforma toda vogal acentuada em vogal normal para
 * manter o padrao ASCII.
 */
int codificaMsg(char *arqEntrada, char *arqSaida, struct tArvore *dados);
