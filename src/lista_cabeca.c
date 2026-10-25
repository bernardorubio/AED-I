#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_cabeca.h"

typedef struct no{
    ITEM *item;
    struct no *prox;
}No;

struct lista_cabeca_{
    No * cabeca; // no cabeca, nao guarda item
    No * fim; // ultimo no (a propria cabeca se a lista estiver vazia)
    No * atual; // cursor de lista_cabeca_primeiro e lista_cabeca_proximo
};

LISTA_CABECA *lista_cabeca_criar(void){
    LISTA_CABECA * l = malloc(sizeof(LISTA_CABECA));
    if(l == NULL){
        return NULL;
    }
    // a cabeca eh um no normal, so que com item NULL
    l->cabeca = malloc(sizeof(No));
    if(l->cabeca == NULL){
        free(l);
        return NULL;
    }
    l->cabeca->item = NULL;
    l->cabeca->prox = NULL;
    l->fim = l->cabeca;
    l->atual = NULL;
    return l;
}

void lista_cabeca_apagar(LISTA_CABECA ** l, bool apagar_itens){
    if(l == NULL || *l == NULL){
        return;
    }
    No * atual = (*l)->cabeca->prox;
    while(atual != NULL){
        No * proximo = atual->prox;
        if(apagar_itens){
            item_apagar(&(atual->item));
        }
        free(atual);
        atual = proximo;
    }
    // a cabeca e liberada por ultimo
    free((*l)->cabeca);
    free(*l);
    *l = NULL;
}

bool lista_cabeca_inserir(LISTA_CABECA * l, ITEM * item){
    if(l == NULL || item == NULL){
        return false;
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    novo->prox = NULL;
    // com a lista vazia o fim eh a cabeca, entao nao precisa de caso especial
    l->fim->prox = novo;
    l->fim = novo;
    return true;
}

bool lista_cabeca_inserir_posicao(LISTA_CABECA * l, ITEM * item, int pos){
    if(l == NULL || item == NULL || pos < 0){
        return false;
    }
    // anterior comeca na cabeca, entao pos == 0 funciona igual as outras posicoes
    No * anterior = l->cabeca;
    for(int i = 0; i < pos && anterior != NULL; i++){
        anterior = anterior->prox;
    }
    if(anterior == NULL){
        return false;
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    novo->prox = anterior->prox;
    anterior->prox = novo;
    if(novo->prox == NULL){
        l->fim = novo;
    }
    return true;
}

ITEM *lista_cabeca_buscar(LISTA_CABECA * l, ITEM * modelo, int (*comparar)(ITEM *, ITEM *)){
    if(l == NULL || modelo == NULL || comparar == NULL){
        return NULL;
    }
    // comeca em cabeca->prox para comecar no primeiro no com item diferente de NULL
    No * atual = l->cabeca->prox;
    while(atual != NULL){
        if(comparar(atual->item, modelo) == 0){
            return atual->item;
        }
        atual = atual->prox;
    }
    return NULL;
}

ITEM *lista_cabeca_remover(LISTA_CABECA * l, int pos){
    if(l == NULL || pos < 0){
        return NULL;
    }
    // anterior comeca na cabeca, entao remover o primeiro item nao eh um caso especial
    No * anterior = l->cabeca;
    for(int i = 0; i < pos && anterior != NULL; i++){
        anterior = anterior->prox;
    }
    if(anterior == NULL || anterior->prox == NULL){
        return NULL;
    }
    No * removido = anterior->prox;
    anterior->prox = removido->prox;
    // se saiu o ultimo, o anterior vira o fim (pode ser a cabeca)
    if(removido == l->fim){
        l->fim = anterior;
    }
    ITEM * item = removido->item;
    free(removido);
    return item;
}

ITEM *lista_cabeca_primeiro(LISTA_CABECA * l){
    if(l == NULL){
        return NULL;
    }
    // pula a cabeca
    l->atual = l->cabeca->prox;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}

ITEM *lista_cabeca_proximo(LISTA_CABECA * l){
    if(l == NULL || l->atual == NULL){
        return NULL;
    }
    l->atual = l->atual->prox;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}