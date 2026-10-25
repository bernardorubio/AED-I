#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

typedef struct no NO;
struct no {
    ITEM* item;
    NO* anterior;
};

struct pilha {
    NO* topo;
    NO* atual; // cursor de pilha_primeiro e pilha_proximo
    int tamanho;
};

PILHA* pilha_criar(void) {
    PILHA* pilha = (PILHA* ) malloc(sizeof (PILHA));
    if (pilha != NULL) {
        pilha->topo = NULL;
        pilha->atual = NULL;
        pilha->tamanho = 0;
    }
    return (pilha);
}

void pilha_apagar(PILHA** pilha, bool apagar_itens) {
    NO* paux;
    if ((pilha == NULL) || (*pilha == NULL)) {
        return;
    }
    while ((*pilha)->topo != NULL) {
        paux = (*pilha)->topo;
        (*pilha)->topo = (*pilha)->topo->anterior;
        // o item so eh apagado quando apagar_itens for true
        if (apagar_itens) {
            item_apagar(&paux->item);
        }
        paux->anterior = NULL;
        free(paux);
    }
    free(*pilha);
    *pilha = NULL;
}

bool pilha_vazia(PILHA *pilha) {
    if(pilha != NULL) 
        return (pilha->tamanho == 0);
    return false;
}

int pilha_tamanho(PILHA *pilha) {
    return ((pilha != NULL) ? pilha->tamanho : -1);
}

ITEM* pilha_topo(PILHA *pilha) {
    if ((pilha!=NULL)&& (!pilha_vazia(pilha)) ) {
        return (pilha->topo->item);
    }
    return (NULL);
}

bool pilha_empilhar(PILHA *pilha, ITEM* item) {
    if(pilha != NULL){
        NO *pnovo = (NO *) malloc(sizeof (NO));
        if (pnovo != NULL) {
            pnovo->item = item;
            pnovo->anterior = pilha->topo;
            pilha->topo = pnovo;
            pilha->tamanho++;
            return (true);
        }
    }
    return (false);
}

ITEM* pilha_desempilhar(PILHA* pilha) {
    if ( (pilha != NULL) && (!pilha_vazia(pilha)) ) {
        NO* pno = pilha->topo;
        ITEM* item = pilha->topo->item;
        pilha->topo = pilha->topo->anterior;
        pno->anterior=NULL; 
        free(pno); 
        pilha->tamanho--;
        return (item);
    }
    return (NULL);
}

ITEM* pilha_primeiro(PILHA *pilha) {
    if (pilha == NULL) {
        return (NULL);
    }
    pilha->atual = pilha->topo;
    if (pilha->atual == NULL) {
        return (NULL);
    }
    return (pilha->atual->item);
}

ITEM* pilha_proximo(PILHA *pilha) {
    if ((pilha == NULL) || (pilha->atual == NULL)) {
        return (NULL);
    }
    pilha->atual = pilha->atual->anterior;
    if (pilha->atual == NULL) {
        return (NULL);
    }
    return (pilha->atual->item);
}
