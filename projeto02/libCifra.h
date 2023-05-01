/*
 * Biblioteca implementada pelo aluno Vinicius Evair da Silva para o projeto 02
 * da disciplina Programacao II (CI1002).
 * Biblioteca com as funcoes que guardam a entrada na estrutura de dados.
 */


/*
 * Guarda a posicao de cada palavra do texto em uma estrutura de dados do tipo
 * arvore AVL, cuja chave de cada no e o caractere inicial da palavra.
 */
int extraiDadosLivro(char *arqLivro, struct tArvore *dados);

/*
 * Transforma um arquivo de chaves em formato de texto para uma estrutura de
 * dados do tipo arvore binaria AVL, com chaves no tipo wchar_t e dados int,
 * tambem guardados em formato de arvore AVL.
 */
int transformaArquivoChaves(char *arqChaves, struct tArvore *dados);
