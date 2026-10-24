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

static NO_GEN *criar_sublista(const char *rotulo){
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

static NO_GEN *buscar_ou_criar(NO_GEN **lista, const char *chave){
    NO_GEN *atual = *lista;
    NO_GEN *anterior = NULL;

    while (atual != NULL)
    {
        if (!strcmp(atual->info.sublista.rotulo, chave))
        {
            return atual;
        }

        anterior = atual;
        atual = atual->prox;
        
    }

    NO_GEN *novo = criar_sublista(chave);

    if (novo == NULL)
    {
        return NULL;
    }
    

    if (anterior == NULL)
    {
        *lista = novo;
    }
    else{
        anterior->prox = novo;
    }

    return novo;
    
    
}

static void inserir_no_fim(NO_GEN **lista, NO_GEN *novo){
    if (*lista == NULL)
    {
        *lista = novo;
        return;
    }
    NO_GEN *ultimo = *lista;
    while (ultimo->prox != NULL)
    {
        ultimo = ultimo->prox;
    }

    ultimo->prox = novo;
    
    
}

bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item){
    if (lista == NULL || item == NULL)
    {
        return false;
    }

    const char *chave1 = item_get_texto(item, lista->coluna1);
    const char *chave2 = item_get_texto(item, lista->coluna2);

    if (chave1 == NULL || chave2 == NULL)
    {
        return false;
    }

    NO_GEN *grupo1 = buscar_ou_criar(&lista->inicio, chave1);
    if (grupo1 == NULL)
    {
        return false;
    }
    
    NO_GEN *grupo2 = buscar_ou_criar(&grupo1->info.sublista.filhos, chave2);

    if (grupo2 == NULL)
    {
        return false;
    }

    NO_GEN *atomo = criar_atomo(item);

    if (atomo == NULL)
    {
        return false;
    }
    
    inserir_no_fim(&grupo2->info.sublista.filhos, atomo);

    lista->tamanho++;
    
    return true;
}




