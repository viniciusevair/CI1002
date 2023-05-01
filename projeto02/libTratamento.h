/*
 * Biblioteca implementada pelo aluno Vinicius Evair da Silva para o projeto 02
 * da disciplina Programacao II (CI1002).
 * Pequena biblioteca interna das bibliotecas de codificacao e criacao de cifra
 * para tratar da extracao de dados dos arquivos de texto.
 */

#include <stdio.h>

/*
 * Le um caractere do arquivo arq. Se necessario, tambem muda o caractere para
 * letra minuscula e remove acentuacoes.
 */
wchar_t pegaCaractere(FILE *arq);
