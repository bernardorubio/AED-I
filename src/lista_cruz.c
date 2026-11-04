#include "item.h"
#include "lista_cruz.h"

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

    int linha;
    int coluna;

    int tamanho;
    
};
