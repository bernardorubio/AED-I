#ifndef SCHEMA_H
#define SCHEMA_H
#include <stdbool.h>

typedef struct schema_ SCHEMA;

SCHEMA *schema_criar(char* cabecalho);//recebe linha 1 arq e devolve o schema
int schema_indice_nome(SCHEMA *s, char *nome); //indiciar nomes
int schema_n_colunas(SCHEMA *s); //recebe schema e devolve ncolunas
const char *schema_get_header(SCHEMA *s); //geter pra saída
void schema_apagar(SCHEMA **s);

#endif