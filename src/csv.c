#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "csv.h"

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

char **csv_separar_em_strings(char *linha, int *qtd_campos){
    bool aspas = false;
    char **campos;
    int cont = 0;
    char *buf = malloc((strlen(linha) + 1) * sizeof(char));
    if(buf == NULL){
        *qtd_campos = -1;
        return NULL;
    }
    for(int i = 0; linha[i] != '\0' && linha[i] != '\n' && linha[i] != '\r'; i++){
        if (linha[i] == '"'){
            aspas = !aspas;
        }
        if(linha[i] == ',' && aspas == false){
            cont++;
        }
    }
    campos = malloc((cont+1)*sizeof(char*));
    if(campos == NULL){
        *qtd_campos = -1;
        free(buf);
        return NULL;
    }
    aspas = false;
    int posicao = 0;
    *qtd_campos = 0;
    for(int i = 0; linha[i] != '\0' && linha[i] != '\n' && linha[i] != '\r'; i++){
        if(linha[i] == '"'){
            aspas = !aspas;
        }
        if(linha[i] == ',' && aspas == false){
            buf[posicao] = '\0';
            campos[*qtd_campos] = copiar_string_alocando(buf);
            if(campos[*qtd_campos] == NULL){
                for(int i = 0; i < *qtd_campos; i++){
                    free(campos[i]);
                }
                free(campos);
                free(buf);
                *qtd_campos = -1;
                return NULL;
            }
            posicao = 0;
            (*qtd_campos)++;
        }
        else if(linha[i] != '"'){
            buf[posicao] = linha[i];
            posicao++;
        }
    }
    buf[posicao] = '\0';
    campos[*qtd_campos] = copiar_string_alocando(buf);
    if(campos[*qtd_campos] == NULL){
                for(int i = 0; i < *qtd_campos; i++){
                    free(campos[i]);
                }
                free(campos);
                free(buf);
                *qtd_campos = -1;
                return NULL;
            }
    (*qtd_campos)++;
    free(buf);
    return campos;
}