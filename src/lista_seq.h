#ifndef LISTA_SEQ_H
#define LISTA_SEQ_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_seq_ LISTA_SEQ;

// lista sequencial: vetor que cresce sozinho, dobrando a capacidade quando enche
LISTA_SEQ *lista_seq_criar(void);
// libera a lista; os itens so sao apagados se apagar_itens for true (use false no result, pois os itens sao os mesmos da base)
void lista_seq_apagar(LISTA_SEQ **lista, bool apagar_itens);
// insere no final
bool lista_seq_inserir(LISTA_SEQ *lista, ITEM *item);
// insere em qualquer posicao de 0 ate o tamanho, deslocando os itens seguintes; false se pos for invalida
bool lista_seq_inserir_posicao(LISTA_SEQ *lista, ITEM *item, int pos);
// percorre em ordem de posicao: primeiro vai para o inicio e proximo avanca um item por chamada; devolvem NULL quando nao ha mais item
ITEM *lista_seq_primeiro(LISTA_SEQ *lista);
ITEM *lista_seq_proximo(LISTA_SEQ *lista);

#endif
