#ifndef LISTA_CABECA_H
#define LISTA_CABECA_H

#include <stdbool.h>
#include "item.h"

// lista encadeada com no cabeca: um no inicial que nao guarda item e existe desde a criacao
// assim todo item tem um anterior, inclusive o primeiro, e nao ha caso especial para o inicio
// lista vazia: cabeca -> NULL    com itens: cabeca -> [A] -> [B] -> NULL

typedef struct lista_cabeca_ LISTA_CABECA;

LISTA_CABECA *lista_cabeca_criar(void);
// libera os nos e a cabeca; os itens so sao apagados se apagar_itens for true (use false no result)
void lista_cabeca_apagar(LISTA_CABECA **lista, bool apagar_itens);
// insere no final
bool lista_cabeca_inserir(LISTA_CABECA *lista, ITEM *item);
// insere na posicao pos (de 0 ate o tamanho); false se pos for invalida
bool lista_cabeca_inserir_posicao(LISTA_CABECA *lista, ITEM *item, int pos);
// devolve o primeiro item que comparar diz ser igual ao modelo (return 0) ou NULL se nao achar
ITEM *lista_cabeca_buscar(LISTA_CABECA *lista, ITEM *modelo, int (*comparar)(ITEM *, ITEM *));
// remove o item da posicao pos e devolve ele; quem chama passa a ser o dono; NULL se pos for invalida
ITEM *lista_cabeca_remover(LISTA_CABECA *lista, int pos);
// percorre do inicio ao fim: primeiro vai para o primeiro item e proximo avanca um no por chamada; devolvem NULL quando nao ha mais item
ITEM *lista_cabeca_primeiro(LISTA_CABECA *lista);
ITEM *lista_cabeca_proximo(LISTA_CABECA *lista);

#endif
