#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>


#include "item.h"

#define ERRO_LISTA_CRUZ -1

typedef struct lista_cruz LISTA_CRUZ;

LISTA_CRUZ *lista_cruz_criar(int campo_linha, int campo_coluna);

bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item);

int lista_cruz_tamanho(LISTA_CRUZ *lista);

void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens);

ITEM *lista_cruz_primeiro(LISTA_CRUZ *lista);

ITEM *lista_cruz_proximo(LISTA_CRUZ *lista);


#endif


