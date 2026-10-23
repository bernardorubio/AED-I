#ifndef ITEM_H
#define ITEM_H

typedef struct item_ ITEM;

ITEM *item_criar(char *linha, char **campos, int qtd_campos); //linha é a linha inteira do arq, item possui copias
const char *item_get_texto(ITEM *i, int indice);//pega texto por indice
double item_get_valor(ITEM *i, int indice); // pega valor por indice : strtod
const char *item_get_linha(ITEM *i); //linha original pra saida
double item_get_similaridade(ITEM *i); //similaridade relativa guardada no item pra facilitar 
bool item_set_similaridade(ITEM *i, double valor); /// '''''
void item_apagar(ITEM **i);


#endif