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

static void apagar_nos(NO_GEN *no){
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

static NO_GEN *criar_sublista(char *rotulo){
    if (rotulo == NULL)
    {
        return NULL;
    }

    NO_GEN *no = malloc(sizeof(NO_GEN));

    if (no == NULL)
    {
        return NULL;
    }

    no->eh_atomo = 0;

    no->info.sublista.rotulo = malloc(strlen(rotulo) + 1);

    if (no->info.sublista.rotulo == NULL)
    {
        free(no);
        return NULL;
    }

    strcpy(no->info.sublista.rotulo, rotulo);
    


    no->info.sublista.filhos = NULL;
    no->prox = NULL;

    return no;
    
    
}

static NO_GEN *criar_atomo(ITEM *item){
    if (item == NULL)
    {
        return NULL;
    }

    NO_GEN *no = malloc(sizeof(NO_GEN));

    if (no == NULL)
    {
        return NULL;
    }
    no->eh_atomo = 1;

    no->info.atomo = item;
    
    no->prox = NULL;

    return no;

    
}




