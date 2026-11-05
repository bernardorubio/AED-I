#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdbool.h>
#include "item.h"
#include "query.h"

typedef struct estrutura_ ESTRUTURA;

ESTRUTURA *estrutura_criar(char *nome, QUERY *q); //[recebe query pra extrair decisao
bool estrutura_inserir(ESTRUTURA *e, ITEM *item);
ITEM *estrutura_primeiro(ESTRUTURA *e);
ITEM *estrutura_proximo(ESTRUTURA *e);
void estrutura_apagar(ESTRUTURA **e, bool apagar_itens);

#endif
