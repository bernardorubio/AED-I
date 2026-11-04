#include "item.h"
#include "lista_cruz.h"

#include <stdlib.h>

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
