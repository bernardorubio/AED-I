#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include "item.h"
#include "schema.h"

typedef struct query_ QUERY;

QUERY *query_ler_arquivo(char *endereco, SCHEMA *s);
bool query_avaliar(QUERY *q, ITEM *item); //item passa filter e sim accept? grava sim no item 
bool query_get_order(QUERY *q, int *coluna, bool *por_similaridade, bool *decrescente); //query com order com flag similaridade e arg desc ou asc
bool query_get_group(QUERY *q, int *coluna1, int *coluna2); //query com group 
bool query_get_cross(QUERY *q, int *coluna1, int *coluna2); // query com cross
void query_apagar(QUERY **q); //nao apaga schema so query

#endif