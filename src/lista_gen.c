#include <stdlib.h>
#include <string.h>

#include "lista_gen.h"

typedef struct no_gen{
    int eh_atomo;
    union {
        ITEM *atomo;
        struct sublista{
            char *rotulo;
            struct no_gen *filhos;
        }sublista;
    }info;
    struct no_gen *prox;
}NO_GEN;

struct lista_gen{
    NO_GEN *inicio;
    int coluna1;
    int coluna2;
    int tamanho;
};




