#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void trocar(long vetor[], long a, long b) {
    long aux;

    aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}

/*
 * Recebe um vetor indexado por [ini..fim].
 * Particiona o vetor em duas partes a partir do valor de vetor[fim].
 * Todos os valores menores ficam para a esquerda, e todos os valores maiores
 * ficam para a direita.
 * Retorna o indice em que o divisor das partes ficou.
 */
int particionar(long vetor[], int ini, int fim, int *numComp) {
    int indiceDivisor, i;

    indiceDivisor = ini;
    for(i = ini; i < fim; i++) {
        (*numComp)++;
        if(vetor[i] < vetor[fim]) {
            trocar(vetor, indiceDivisor, i);
            indiceDivisor++;
        }
    }

    trocar(vetor, indiceDivisor, fim);
    return indiceDivisor;        
}

/*
 * Recebe um vetor indexado por [0..tam-1].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int quickSortInterna(long vetor[], int ini, int fim) {
    int indiceDivisor, numComp;
    if(ini >= fim)
        return 0;

    numComp = 0;
    indiceDivisor = particionar(vetor, ini, fim, &numComp);
    numComp += quickSortInterna(vetor, ini, indiceDivisor - 1);
    numComp += quickSortInterna(vetor, indiceDivisor + 1, fim);

    return numComp;
}

int quickSort(long vetor[], int tam){
    return quickSortInterna(vetor, 0, tam-1);
}

long tamanhoArquivo() {
    struct stat st;
    stat("numeros.bin", &st);

    return st.st_size / sizeof(long);
}

int main(int argc, char *argv[]) {
    FILE *arq;
    int i, j, menor, maior, qtd, ret;
    long aux;
    long *value;

    srand(time(NULL));
    if (! (arq = fopen("numeros.bin", "r+"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    qtd = tamanhoArquivo();
    if (! (value = calloc(qtd, sizeof(long))))
        return 1;

    ret = fread(value, sizeof(long), qtd, arq);

    quickSort(value, ret);

    rewind(arq);
    fwrite(value, sizeof(long), ret, arq);

    if (ret)
        printf("Ordenou %d valores!\n", ret);
    else
        printf("Erro ao gravar!\n");

    fclose(arq);
    free(value);
    return 0;
}

