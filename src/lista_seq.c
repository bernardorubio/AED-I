#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_seq.h"

struct lista_seq_{
    ITEM ** itens;
    int tamanho;
    int capacidade;
    int atual; // indice do cursor de lista_seq_primeiro e lista_seq_proximo
};

// dobra a capacidade do vetor (comeca com 16); false se o realloc falhar
static bool aumentar(LISTA_SEQ * l){
    int nova_capacidade;
    if(l->capacidade == 0){
        nova_capacidade = 16;
    }
    else{
        nova_capacidade = 2 * l->capacidade;
    }
    ITEM ** novos = realloc(l->itens, nova_capacidade * sizeof(ITEM *));
    if(novos == NULL){
        return false;
    }
    l->itens = novos;
    l->capacidade = nova_capacidade;
    return true;
}

LISTA_SEQ *lista_seq_criar(void){
    LISTA_SEQ * l = malloc(sizeof(LISTA_SEQ));
    if(l == NULL){
        return NULL;
    }
    l->itens = NULL;
    l->tamanho = 0;
    l->capacidade = 0;
    l->atual = 0;
    return l;
}

void lista_seq_apagar(LISTA_SEQ ** l, bool apagar_itens){
    if(l == NULL || *l == NULL){
        return;
    }
    if(apagar_itens){
        for(int i = 0; i < (*l)->tamanho; i++){
            item_apagar(&((*l)->itens[i]));
        }
    }
    free((*l)->itens);
    free(*l);
    *l = NULL;
}

bool lista_seq_inserir(LISTA_SEQ * l, ITEM * item){
    if(l == NULL || item == NULL){
        return false;
    }
    // ve se o vetor esta cheio, se estiver ele chama a aumentar
    // so da false se o realloc da funcao aumentar falhar
    if(l->tamanho == l->capacidade && !aumentar(l)){
        return false;
    }
    l->itens[l->tamanho] = item;
    l->tamanho++;
    return true;
}

bool lista_seq_inserir_posicao(LISTA_SEQ * l, ITEM * item, int pos){
    if(l == NULL || item == NULL || pos < 0 || pos > l->tamanho){
        return false;
    }
    if(l->tamanho == l->capacidade && !aumentar(l)){
        return false;
    }
    // abre espaco para o novo item deslocando os seguintes uma posicao para a direita
    for(int j = l->tamanho; j > pos; j--){
        l->itens[j] = l->itens[j-1];
    }
    l->itens[pos] = item;
    l->tamanho++;
    return true;
}

ITEM *lista_seq_primeiro(LISTA_SEQ * l){
    if(l == NULL){
        return NULL;
    }
    l->atual = 0;
    if(l->atual >= l->tamanho){
        return NULL;
    }
    return l->itens[l->atual];
}

ITEM *lista_seq_proximo(LISTA_SEQ * l){
    if(l == NULL || l->atual >= l->tamanho){
        return NULL;
    }
    l->atual++;
    if(l->atual >= l->tamanho){
        return NULL;
    }
    return l->itens[l->atual];
}
