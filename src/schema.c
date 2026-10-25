#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "schema.h"
#include "csv.h"

struct schema_ {
    char *linha;
    char **campos;
    int qtd_campos;
};

//aux

static char *copiar_string_alocando(char *origem){
    if(origem == NULL){
        return NULL;
    }
    char *alvo = NULL;
    alvo = malloc(strlen(origem) + 1);
    if(alvo == NULL){
        return NULL;
    }
    strcpy(alvo, origem);
    return alvo;
}


SCHEMA *schema_criar(char *linha){
    if(linha == NULL){
        return NULL;
    }
    SCHEMA *s = malloc(sizeof(SCHEMA));
    if(s == NULL){
        return NULL;
    }
    s->linha = copiar_string_alocando(linha);
    if (s->linha == NULL){
        free(s);
        return NULL;
    }
    int qtd_campos = 0;
    s->campos = csv_separar_em_strings(s->linha, &qtd_campos);
    if(s->campos == NULL || qtd_campos == -1){
        free(s->linha);
        free(s);
        return NULL;
    }
    s->qtd_campos = qtd_campos;
    return s;
}

int schema_indice_nome(SCHEMA *s, char *nome){
    if(s == NULL || nome == NULL){
        return -1;
    }
    for(int i = 0; i < s->qtd_campos; i++){
        int v = strcmp(nome, s->campos[i]);
        if(v == 0){
            return i;
        }
    }
    return -1;
}

int schema_n_colunas(SCHEMA *s){
    if(s == NULL){
        return -1;
    }
    return s->qtd_campos;
}

const char *schema_get_header(SCHEMA *s){
    if(s == NULL){
        return NULL;
    }
    return s->linha;
}

void schema_apagar(SCHEMA **s){
    if(s == NULL || *s == NULL){
        return;
    }
    csv_liberar_memoria_separada((*s)->campos, (*s)->qtd_campos);
    free((*s)->linha);
    free(*s);
    *s = NULL;
}