#ifndef LISTA_CRUZ_H
#define LISTA_CRUZ_H

#include <stdbool.h>


#include "item.h"

// lista cruzada: organiza os itens numa "tabela" com duas chaves (ex.: CROSS|threat_category|year)
// linhas = valores do campo_linha, colunas = valores do campo_coluna, e cada item fica no cruzamento dos dois

#define ERRO_LISTA_CRUZ -1 // devolvido pelo tamanho quando a lista é NULL

typedef struct lista_cruz LISTA_CRUZ;

// campo_linha e campo_coluna são índices de coluna do CSV (posição do campo no item), não os valores.
// NULL se algum índice for < 0 ou faltar memória
LISTA_CRUZ *lista_cruz_criar(int campo_linha, int campo_coluna);

// coloca o item no fim da sua linha e da sua coluna, criando as cabeças que ainda não existem.
// guarda só o ponteiro do item (não copia). false se algum ponteiro é NULL, o item não tem o campo ou faltou memória
bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item);

// quantidade de itens da lista, ou ERRO_LISTA_CRUZ se a lista for NULL
int lista_cruz_tamanho(LISTA_CRUZ *lista);

// libera a lista (nós, cabeças e rótulos) e deixa *lista = NULL.
// os itens só são apagados se apagar_itens for true (use false no result, pois a base já vai apagar os itens)
void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens);

// percorre linha por linha, na ordem de chegada: primeiro vai para o primeiro item da primeira linha
// e proximo avança um item por chamada; devolvem NULL quando não há mais item
ITEM *lista_cruz_primeiro(LISTA_CRUZ *lista);

ITEM *lista_cruz_proximo(LISTA_CRUZ *lista);

//as proximas duas funções são praticamente iguais às anteriores, porém percorremos por meio das colunas

ITEM *lista_cruz_primeiro_coluna(LISTA_CRUZ *lista);

ITEM *lista_cruz_proximo_coluna(LISTA_CRUZ *lista);


#endif


