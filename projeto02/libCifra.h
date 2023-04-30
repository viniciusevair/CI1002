/*
 * Guarda a posicao de cada palavra do texto em uma estrutura de dados do tipo
 * arvore AVL, cuja chave de cada no e a letra inicial da palavra.
 */
int extraiDadosLivro(char *arqLivro, struct tArvore *dados);

/*
 * Transforma um arquivo de chaves em formato de texto para uma estrutura de
 * dados do tipo arvore binaria AVL, com chaves no tipo wchar_t e dados int,
 * tambem guardados em formato de arvore AVL.
 */
int transformaArquivoChaves(char *arqChaves, struct tArvore *dados);
