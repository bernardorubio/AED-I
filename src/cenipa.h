#ifndef CENIPA_H
#define CENIPA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "schema.h"

typedef struct leitor_cenipa_ LEITOR_CENIPA;

LEITOR_CENIPA *cenipa_abrir_arquivo(char *caminho, int limite, SCHEMA **s);
ITEM *cenipa_proximo_item(LEITOR_CENIPA *l, bool *terminou);
void cenipa_apagar(LEITOR_CENIPA **l);

//leitor da parte 2 (CENIPA, ocorrencias aeronauticas): mesmo funcionamento do nasa, com separador ';'
//cenipa_apagar não apaga schema. por isso cenipa abrir tem SCHEMA **s e schema nao faz parte de LEITOR_CENIPA
#endif