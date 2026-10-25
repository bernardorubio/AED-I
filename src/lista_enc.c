#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_enc.h"

typedef struct no{
    ITEM *item;
    struct no *prox;
}No;

struct lista_enc_{
    No * inicio;
    No * fim;
    No * atual; // cursor de lista_enc_primeiro e lista_enc_proximo
};

LISTA_ENC *lista_enc_criar(void){
    LISTA_ENC * l = malloc(sizeof(LISTA_ENC));
    if(l == NULL){
        return NULL;
    }
    l->inicio = NULL;
    l->fim = NULL;
    l->atual = NULL;
    return l;
}

void lista_enc_apagar(LISTA_ENC ** l, bool apagar_itens){
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

bool lista_enc_inserir(LISTA_ENC * l, ITEM * item){
    if(l == NULL || item == NULL){
        return false;
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    novo->prox = NULL;
    if(l->inicio == NULL){
        l->inicio = novo;
    }
    else{
        l->fim->prox = novo;
    }
    l->fim = novo;
    return true;
}

bool lista_enc_inserir_posicao(LISTA_ENC * l, ITEM * item, int pos){
    if(l == NULL || item == NULL || pos < 0){
        return false;
    }
    // anterior e o no que fica antes da nova posicao (continua NULL quando pos == 0)
    No * anterior = NULL;
    if(pos > 0){
        anterior = l->inicio;
        for(int i = 0; i < pos - 1 && anterior != NULL; i++){
            anterior = anterior->prox;
        }
        if(anterior == NULL){
            return false;
        }
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    if(pos == 0){
        novo->prox = l->inicio;
        l->inicio = novo;
    }
    else{
        novo->prox = anterior->prox;
        anterior->prox = novo;
    }
    // se o novo no ficou sem proximo, ele passou a ser o ultimo
    if(novo->prox == NULL){
        l->fim = novo;
    }
    return true;
}

ITEM *lista_enc_primeiro(LISTA_ENC * l){
    if(l == NULL){
        return NULL;
    }
    l->atual = l->inicio;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}

ITEM *lista_enc_proximo(LISTA_ENC * l){
    if(l == NULL || l->atual == NULL){
        return NULL;
    }
    l->atual = l->atual->prox;
    if(l->atual == NULL){
        return NULL;
    }
    return l->atual->item;
}
