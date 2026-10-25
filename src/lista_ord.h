#ifndef LISTA_ORD_H
#define LISTA_ORD_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_ord_ LISTA_ORD;

// comparar recebe dois itens e devolve negativo se o 1o vem antes do 2o, 0 se empatam e positivo se o 1o vem depois
// quem cria a lista escolhe a regra de ordem; a lista so guarda a funcao e usa em cada insercao
//ao implementar na main a funcao comparar precisa seguir essa regra

LISTA_ORD *lista_ord_criar(int (*comparar)(ITEM *, ITEM *));
// libera a lista; os itens so sao apagados se apagar_itens for true (use false no result, pois a base ja vai apagar os itens)
void lista_ord_apagar(LISTA_ORD **lista, bool apagar_itens);
// insere ja na posicao certa (nao ordena no final); itens que empatam ficam na ordem de chegada
bool lista_ord_inserir(LISTA_ORD *lista, ITEM *item);
// percorre na ordem da lista: primeiro vai para o inicio e proximo avanca um no por chamada; devolvem NULL quando nao ha mais item
ITEM *lista_ord_primeiro(LISTA_ORD *lista);
ITEM *lista_ord_proximo(LISTA_ORD *lista);

#endif