#ifndef LISTA_GEN_H
#define LISTA_GEN_H

#include "item.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct lista_gen LISTA_GEN;

LISTA_GEN *lista_gen_criar(int coluna1, int coluna2);

bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item);

int lista_gen_percorrer(LISTA_GEN *lista, void (*visitar) (ITEM *));

int lista_gen_tamanho(LISTA_GEN *lista);

void lista_gen_apagar(LISTA_GEN **lista);

#endif
