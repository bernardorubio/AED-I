#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_ord.h"

typedef struct no{
    ITEM *item;
    struct no *prox;
}No;

struct lista_ord_{
    No * inicio;
    No * atual; // cursor de lista_ord_primeiro e lista_ord_proximo
    int (*comparar)(ITEM *, ITEM *); // regra de ordem escolhida por quem cria a lista
};

LISTA_ORD *lista_ord_criar(int (*comparar)(ITEM *, ITEM *)){
    if(comparar == NULL){
        return NULL;
    }
    LISTA_ORD * l = malloc(sizeof(LISTA_ORD));
    if(l == NULL){
        return NULL;
    }
    l->inicio = NULL;
    l->atual = NULL;
    l->comparar = comparar;
    return l;
}

void lista_ord_apagar(LISTA_ORD ** l, bool apagar_itens){
    if(l == NULL || *l == NULL){
        return;
    }
    No * atual = (*l)->inicio;
    while(atual != NULL){
        No * proximo = atual->prox;
        if(apagar_itens){
            item_apagar(&(atual->item));
        }
        free(atual);
        atual = proximo;
    }
    free(*l);
    *l = NULL;
}

bool lista_ord_inserir(LISTA_ORD * l, ITEM * item){
    if(l == NULL || item == NULL){
        return false;
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    // anterior fica no ultimo no que vem antes do item novo (ou empata com ele); NULL quando o novo vai para o inicio
    No * anterior = NULL;
    if(l->inicio != NULL && l->comparar(l->inicio->item, item) <= 0){ // usa a funcao dada pra comparar; para quando for a posicao que deve ser colocado
        anterior = l->inicio;
        while(anterior->prox != NULL && l->comparar(anterior->prox->item, item) <= 0){// menor igual que foi definido no .h
            anterior = anterior->prox;
        }
    }
    if(anterior == NULL){
        novo->prox = l->inicio;
        l->inicio = novo;
    }
    else{
        novo->prox = anterior->prox;
        anterior->prox = novo;
    }
    return true;
}

ITEM *lista_ord_primeiro(LISTA_ORD * l){
    if(l == NULL){
        return NULL;
    }
    l->atual = l->inicio;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}

ITEM *lista_ord_proximo(LISTA_ORD * l){
    if(l == NULL || l->atual == NULL){
        return NULL;
    }
    l->atual = l->atual->prox;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}
