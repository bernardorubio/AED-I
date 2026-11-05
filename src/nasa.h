#ifndef NASA_H
#define NASA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "schema.h"

typedef struct leitor_ LEITOR;

LEITOR *nasa_abrir_arquivo(char *caminho, int limite, SCHEMA **s);
ITEM *nasa_proximo_item(LEITOR *l, bool *terminou);
void nasa_apagar(LEITOR **l);

//nasa_apagar não apaga schema. por isso nasa abrir tem SCHEMA **s e schema nao faz parte de LEITOR
#endif