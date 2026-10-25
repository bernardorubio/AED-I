#ifndef PILHA_H
#define PILHA_H

#include "item.h"
#include <stdbool.h>

typedef struct pilha PILHA;

// cria uma pilha vazia
PILHA *pilha_criar(void);
// libera a pilha; os itens so sao apagados se apagar_itens for true (use false no result, pois os itens sao os mesmos da base)
void pilha_apagar(PILHA **pilha, bool apagar_itens);
bool pilha_vazia(PILHA *pilha);
int pilha_tamanho(PILHA *pilha);
// devolve o item do topo sem remover
ITEM *pilha_topo(PILHA *pilha);
// guarda so o ponteiro do item, nao faz copia
bool pilha_empilhar(PILHA *pilha, ITEM *item);
// remove o topo e devolve o item; quem chama passa a ser o dono dele
ITEM *pilha_desempilhar(PILHA *pilha);
// percorre sem remover: primeiro vai para o topo e proximo avanca um item por chamada em direcao ao fundo; devolvem NULL quando nao ha mais item
ITEM *pilha_primeiro(PILHA *pilha);
ITEM *pilha_proximo(PILHA *pilha);

#endif
