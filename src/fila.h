#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "item.h"

typedef struct fila FILA;

// cria uma fila vazia
FILA* fila_criar(void);
// insere no fim da fila; guarda so o ponteiro do item, nao faz copia
bool fila_inserir(FILA *fila, ITEM *item);
// remove do inicio e devolve o item; quem chama passa a ser o dono dele
ITEM* fila_remover(FILA *fila);
// libera a fila; os itens so sao apagados se apagar_itens for true (use false no result, pois a base ja vai apagar os itens)
void fila_apagar(FILA **fila, bool apagar_itens);
bool fila_vazia(FILA *fila);
int fila_tamanho(FILA *fila);
// percorre sem remover: primeiro vai para o inicio e proximo avanca um item por chamada em direcao ao fim; devolvem NULL quando nao ha mais item
ITEM* fila_primeiro(FILA *fila);
ITEM* fila_proximo(FILA *fila);

#endif
