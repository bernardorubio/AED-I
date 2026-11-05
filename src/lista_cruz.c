#include "item.h"
#include "lista_cruz.h"

#include <stdlib.h>
#include <string.h>

typedef struct no_cruz
{
    ITEM *item;
    struct no_cruz *prox_linha;
    struct no_cruz *prox_coluna;

}NO_CRUZ;

typedef struct cabeca
{
    char *rotulo;
    NO_CRUZ *primeiro;
    struct cabeca *prox;
}CABECA;

struct lista_cruz{
    CABECA *inicio_colunas;
    CABECA *inicio_linhas;

    int campo_linha;
    int campo_coluna;

    int tamanho;

    CABECA *atual_cabeca;
    NO_CRUZ *atual;
    
};


LISTA_CRUZ *lista_cruz_criar(int campo_linha, int campo_coluna){
    if (campo_linha < 0 || campo_coluna < 0 )
    {
        return NULL;

    }

    LISTA_CRUZ *lista = (LISTA_CRUZ *)malloc(sizeof(LISTA_CRUZ));


    if (lista != NULL)
    {
        lista->campo_linha = campo_linha;
        lista->campo_coluna = campo_coluna;
        lista->tamanho = 0;
        lista->atual_cabeca = NULL;
        lista->atual = NULL;
        lista->inicio_colunas = NULL;
        lista->inicio_linhas = NULL;
    }
    

    return lista;
    
}

static CABECA *criar_cabeca(const char *rotulo){
    if (rotulo == NULL)
    {
        return NULL;
    }

    CABECA *cabeca = malloc(sizeof(CABECA));

    if (cabeca == NULL)
    {
        return NULL;
    }

    cabeca->rotulo = malloc(strlen(rotulo) + 1);

    if (cabeca->rotulo == NULL)
    {
        free(cabeca);
        return NULL;
    }

    strcpy(cabeca->rotulo, rotulo);

    cabeca->prox = NULL;
    cabeca->primeiro = NULL;


    return cabeca;
}

static NO_CRUZ *criar_no(ITEM *item){
    if (item == NULL)
    {
        return NULL;
    }

    NO_CRUZ *no = malloc(sizeof(NO_CRUZ));

    if (no == NULL)
    {
        return NULL;
    }
    no->item = item;

    no->prox_coluna = NULL;
    no->prox_linha = NULL;

    return no;
}

bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item){
    if (lista == NULL || item == NULL)
    {
        return false;
    }
    
    
    const char *rotulo_coluna = item_get_texto(item, lista->campo_coluna);
    const char *rotulo_linha = item_get_texto(item, lista->campo_linha);

    if (rotulo_coluna == NULL || rotulo_linha == NULL)
    {
        return false;
    }

    CABECA *cursor_linha = lista->inicio_linhas;
    CABECA *anterior_linha= NULL;



    while (cursor_linha != NULL && strcmp(rotulo_linha,cursor_linha->rotulo ) != 0)
    {
        anterior_linha = cursor_linha;
        cursor_linha = cursor_linha->prox;
    }

    if (cursor_linha == NULL)
    {
        CABECA *nova_cabeca_linha = criar_cabeca(rotulo_linha);
        if (nova_cabeca_linha == NULL)
        {
            return false;
        }

        if (anterior_linha == NULL)
        {
            lista->inicio_linhas = nova_cabeca_linha;
        }
        else{
            anterior_linha->prox = nova_cabeca_linha;
        }

        cursor_linha = nova_cabeca_linha;
        
       
        
    }

    CABECA *cursor_coluna = lista->inicio_colunas;
    CABECA *anterior_coluna = NULL;


    while (cursor_coluna != NULL && strcmp(rotulo_coluna, cursor_coluna->rotulo))
    {
        anterior_coluna = cursor_coluna;
        cursor_coluna = cursor_coluna->prox;
    }

    if (cursor_coluna == NULL)
    {
        CABECA *nova_cabeca_coluna = criar_cabeca(rotulo_coluna);
        if (nova_cabeca_coluna == NULL)
        {
            return false;
        }

        if (anterior_coluna == NULL)
        {
            lista->inicio_colunas = nova_cabeca_coluna;
        }
        else{
            anterior_coluna->prox = nova_cabeca_coluna;
        }

        cursor_coluna = nova_cabeca_coluna;
        
        
    }

    NO_CRUZ *no = criar_no(item);

    if (no == NULL)
    {
        return false;
    }

    if (cursor_linha->primeiro == NULL)
    {
        cursor_linha->primeiro = no;
    }

    else{

        NO_CRUZ *cursor_linha_no = cursor_linha->primeiro;

        while (cursor_linha_no->prox_linha != NULL)
        {

        cursor_linha_no = cursor_linha_no->prox_linha;

        }

        cursor_linha_no->prox_linha = no;

    }

    if (cursor_coluna->primeiro == NULL)
    {
        cursor_coluna->primeiro = no;
    }

    else{
        NO_CRUZ *cursor_coluna_no = cursor_coluna->primeiro;

        while (cursor_coluna_no->prox_coluna != NULL)
        {

            cursor_coluna_no = cursor_coluna_no->prox_coluna;

        }
        
        cursor_coluna_no->prox_coluna = no;
    }

    lista->tamanho++;

    return true;



}

int lista_cruz_tamanho(LISTA_CRUZ *lista){
    if (lista == NULL)
    {
        return ERRO_LISTA_CRUZ;
    }

    return lista->tamanho;
    
}

static void apagar_nos(NO_CRUZ *no, bool apagar_itens){
    if (no == NULL)
    {
        return;
    }

    while (no != NULL)
    {
        
        NO_CRUZ *proximo_linha = no->prox_linha;

        if (apagar_itens == true)
        {
            item_apagar(&no->item);
        }

        free(no);

        no = proximo_linha;
        
    }
    
    
}

static void apagar_cabeca(CABECA *cabeca){
    if (cabeca == NULL)
    {
        return;
    }

    while (cabeca != NULL)
    {
        CABECA *prox = cabeca->prox;

        free(cabeca->rotulo);
        free(cabeca);

        cabeca = prox;
        
    }
    
    
}

void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens){
    if (lista ==  NULL || *lista == NULL)
    {
        return;
    }

    CABECA *atual_linha = (*lista)->inicio_linhas;

    while (atual_linha != NULL)
    {
        CABECA *proximo_linha = atual_linha->prox;

        apagar_nos(atual_linha->primeiro, apagar_itens);

        atual_linha = proximo_linha;

    }

   apagar_cabeca((*lista)->inicio_linhas);
   apagar_cabeca((*lista)->inicio_colunas);
    
    free(*lista);
    *lista = NULL;
}


