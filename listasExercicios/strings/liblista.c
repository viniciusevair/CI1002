#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblista.h"

lista_t *cria_lista () {
    lista_t *l;
        
    if (! (l = malloc (sizeof (lista_t))))
        return NULL;

    l->Primeiro = NULL;

    return l;
}

lista_t *destroi_lista (lista_t *l) {
    while (l->Primeiro != NULL) {
        free (obtem_primeiro_lista (l));
    }    
    
    free (l);    

    return NULL;
}

int adiciona_inicio_lista (lista_t *l, dado_t *dado) {
    nodo_lista_t *novo;
    if (! (novo = malloc (sizeof (nodo_lista_t))))
        return 0;
    if (! (novo->dado = malloc (sizeof (dado_t)))) {
        free (novo);
        return 0;
    }

    memcpy (novo->dado, dado, sizeof (dado_t));
    
    if (l->Primeiro == NULL) {    
        novo->prox = NULL;
        l->Primeiro = novo;

        return 1;
    }

    novo->prox = l->Primeiro;
    l->Primeiro = novo;

    return 1;
}

int adiciona_ordem_lista (lista_t *l, dado_t *dado) {
    nodo_lista_t *novo, *atual;

    if (l->Primeiro == NULL) {    
        return adiciona_inicio_lista(l, dado);
    }

    if (! (novo = malloc (sizeof (nodo_lista_t))))
        return 0;
    if (! (novo->dado = malloc (sizeof (dado_t)))) {
        free (novo);
        return 0;
    }
    novo->prox = NULL;

    memcpy (novo->dado, dado, sizeof (dado_t));

    if (l->Primeiro->dado->indice > novo->dado->indice) {
        novo->prox = l->Primeiro;
        l->Primeiro = novo;
        
        return 1;
    }

    atual = l->Primeiro;
    while (atual->prox != NULL && novo->dado->indice > atual->prox->dado->indice)
        atual = atual->prox;

    novo->prox = atual->prox;
    atual->prox = novo;

    return 1;
}

dado_t *obtem_primeiro_lista (lista_t *l) {
    nodo_lista_t *nodo_temp;
    dado_t *dado_temp;

    if (l->Primeiro == NULL)
        return NULL;

    nodo_temp = l->Primeiro;
    l->Primeiro = l->Primeiro->prox;
    nodo_temp->prox = NULL;

    dado_temp = nodo_temp->dado;
    free (nodo_temp);

    return dado_temp;
}
