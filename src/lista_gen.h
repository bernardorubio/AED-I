#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include <stdbool.h>
#include "item.h"

// lista generalizada: agrupa os itens em 2 níveis (ex.: GROUP|threat_category|year)
// nível 1 = grupos pela coluna1, nível 2 = subgrupos pela coluna2, nível 3 = os itens

#define LISTA_GEN_ERRO (-1) // devolvido pelas funções que retornam int quando a lista (ou o visitar) é NULL


typedef struct lista_gen LISTA_GEN;

// coluna1 e coluna2 são índices de coluna do CSV (posição do campo no item):
// coluna1 = agrupamento de fora (nível 1), coluna2 = de dentro (nível 2). NULL se coluna < 0 ou sem memória
LISTA_GEN *lista_gen_criar(int coluna1, int coluna2);

// coloca o item no grupo/subgrupo das suas colunas, criando os grupos que ainda não existem.
// guarda só o ponteiro do item (não copia). false se algum ponteiro é NULL, o item não tem a coluna ou faltou memória
bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item);

// visita grupo por grupo (na ordem de chegada), chamando visitar para cada item.
// devolve a quantidade de itens visitados, ou LISTA_GEN_ERRO se lista ou visitar for NULL
int lista_gen_percorrer(LISTA_GEN *lista, void (*visitar) (ITEM *));

// quantidade de itens da lista, ou LISTA_GEN_ERRO se a lista for NULL
int lista_gen_tamanho(LISTA_GEN *lista);

// libera a lista (nós e rótulos) e deixa *lista = NULL.
// NÃO apaga os ITEMs: quem criou os itens continua responsável pelo item_apagar
void lista_gen_apagar(LISTA_GEN **lista);

#endif
