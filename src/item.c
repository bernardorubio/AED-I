#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "item.h"

struct item_{
    char *linha;
    char **campos;
    int qtd_campos;
    double similarity;
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

//ta no header

ITEM *item_criar(char *linha, char **campos, int qtd_campos){
    if(linha == NULL || campos == NULL || qtd_campos <= 0){
        return NULL;
    }
    ITEM *i = malloc(sizeof(ITEM));
    if(i == NULL){
        return NULL;
    }
    i->linha = copiar_string_alocando(linha); //funcao auxiliar para copiar junto com a alocacao(nao ter que ficar repetindo aquilo tudo ali em cima)
    if(i->linha == NULL){
        free(i);
        return NULL;
    }
    i->campos = malloc(qtd_campos*sizeof(char*));
    if(i->campos == NULL){
        free(i->linha);
        free(i);
        return NULL; // liberar (tendencia p todas) oq ja foi alocado pra nao ter leak
    }
    for(int j = 0; j < qtd_campos; j++){
        i->campos[j] = copiar_string_alocando(campos[j]);
        if(i->campos[j] == NULL){
            for(int k = 0; k < j; k++){
                free(i->campos[k]);
            }
            free(i->campos);
            free(i->linha);
            free(i);
            return NULL;
        }
    }
    i->qtd_campos = qtd_campos;
    i->similarity = 0;
    return i;
}

const char *item_get_texto(ITEM *i, int indice){
    if(i == NULL || indice > i->qtd_campos-1 || indice < 0){
        return NULL;
    }
    return i->campos[indice];
}

double item_get_valor(ITEM *i, int indice){
    if(i == NULL || indice > i->qtd_campos-1 || indice < 0){
        return NAN; //daq pra baixo tem: not a number, pq outros numeros seriam "retornos validos"
    }
    char *fim;
    double valor = strtod(i->campos[indice], &fim);
    if(fim == (i->campos[indice])){
        return NAN;
    }
    return valor;
}

const char* item_get_linha(ITEM *i){
    if(i == NULL){
        return NULL;
    }
    return i->linha;
}

bool item_set_similaridade(ITEM *i, double valor){
    if(i == NULL){
        return false;
    }
    i->similarity = valor;
    return true;
}

double item_get_similaridade(ITEM *i){
    if(i == NULL){
        return NAN;
    }
    return i->similarity;
}

void item_apagar(ITEM **i){
    if(i == NULL || *i == NULL){
        return;
    }
    for(int j = 0; j < (*i)->qtd_campos; j++){
        free((*i)->campos[j]);
    }
    free((*i)->campos);
    free((*i)->linha);
    free((*i));
    *i = NULL;
    return;
}