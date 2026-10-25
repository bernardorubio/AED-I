#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

typedef struct no{
    ITEM *item;
    struct no *prox;
}No;

struct fila{
    No * inicio;
    No * fim;
    No * atual; // cursor de fila_primeiro e fila_proximo
    int tamanho;
};

FILA * fila_criar(void){
    FILA * f = malloc(sizeof(FILA));
    if(f == NULL){
        return NULL;
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->atual = NULL;
    f->tamanho = 0;
    return(f);
}

bool fila_inserir(FILA * f, ITEM * item){
    if(f == NULL){
        return false;
    }
    No * novo = malloc(sizeof(No));
    if(novo == NULL){
        return false;
    }
    novo->item = item;
    novo->prox = NULL;
    if(f->tamanho == 0){
        f-> inicio = novo;
    }
    else{
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->tamanho++;
    
    return true;
}

ITEM * fila_remover(FILA * f){
    if(f == NULL || f->tamanho == 0){
        return NULL;
    }
    No * removido = f->inicio;
    if(removido == NULL){
        return NULL;
    }
    f->inicio = removido->prox;
    if(f-> inicio == NULL){
        f->fim = NULL;
    }
    ITEM * item = removido->item;
    free(removido);
    f->tamanho--;
    return(item);
}

void fila_apagar(FILA **f, bool apagar_itens){
    if(f == NULL || *f == NULL){
        return;
    }
    No * atual = (*f)->inicio;
    for(int i = 0; i < (*f)->tamanho; i++){
        No * proximo = atual->prox;
        // o item so eh apagado quando apagar_itens eh true qua acontece na base; o result so libera os nos
        if(apagar_itens){
            item_apagar(&(atual->item));
        }
        free(atual);
        atual = proximo;
    }
    free(*f);
    *f = NULL;
}

bool fila_vazia(FILA *f){
    if(f == NULL || f->inicio == NULL){
        return true;
    }
    else{
        return false;
    }
}

int fila_tamanho(FILA *f){
    if(f == NULL){
        return -1;
    }
    return f->tamanho;
}

ITEM * fila_primeiro(FILA *f){
    if(f == NULL){
        return NULL;
    }
    f->atual = f->inicio;
    if(f->atual == NULL){
        return NULL;
    }
    return(f->atual->item);
}

ITEM * fila_proximo(FILA *f){
    if(f == NULL || f->atual == NULL){
        return NULL;
    }
    f->atual = f->atual->prox;
    if(f->atual == NULL){
        return NULL;
    }
    return(f->atual->item);
}
