#include <stdlib.h>
#include <string.h>

#include "lista_gen.h"



typedef struct no_gen{
    int eh_atomo;
    union {
        ITEM *atomo;
        struct sublista{
            char *rotulo;
            struct no_gen *filhos;
        }sublista;
    }info;
    struct no_gen *prox;
}NO_GEN;

struct lista_gen{
    NO_GEN *inicio;
    int coluna1;
    int coluna2;
    int tamanho;
};

LISTA_GEN *lista_gen_criar(int coluna1, int coluna2){
    if (coluna1 < 0 || coluna2 < 0)
    {
        return NULL;
    }
    LISTA_GEN *lista = (LISTA_GEN*)malloc(sizeof(LISTA_GEN));

    if (lista != NULL)
    {
        lista->inicio = NULL;
        lista->tamanho = 0;
        lista->coluna1 = coluna1;
        lista->coluna2 = coluna2;
    }

    return lista;
    
    
}

int lista_gen_tamanho(LISTA_GEN *lista){
    if (lista == NULL || lista->tamanho < 0)
    {
        return LISTA_GEN_ERRO;
    }

    return lista->tamanho;
    
}

void apagar_nos(NO_GEN *no){
    while (no != NULL)
    {
        NO_GEN *proximo = no->prox;
        if (no->eh_atomo == 0)
        {
            apagar_nos(no->info.sublista.filhos);
            free(no->info.sublista.rotulo);
        }
        free(no);
        no = proximo;

    }

    
    
}

void lista_gen_apagar(LISTA_GEN **lista){
    if (lista == NULL || *lista == NULL)
    {
        return;

    }

    apagar_nos((*lista)->inicio);
    free(*lista);
    *lista = NULL;
    
}




