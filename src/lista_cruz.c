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

    int campo_linha;
    int campo_coluna;

    int tamanho;

    CABECA *atual_cabeca;
    NO_CRUZ *atual;
    
};
