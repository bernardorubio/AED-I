#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "csv.h"
#include "schema.h"
#include "item.h"
#include "nasa.h"
#define TAM_MAX 1000

struct leitor_{
    FILE *f;
    int limite;
    int lidos;
    int qtd_col;
};

LEITOR *nasa_abrir_arquivo(char *caminho, int limite, SCHEMA **s){
    if(s == NULL || caminho == NULL){
        return NULL;
    }
    LEITOR *l = malloc(sizeof(LEITOR));
    if(l == NULL){
        *s = NULL;
        return NULL;
    }
    l->f = fopen(caminho, "r");
    if(l->f == NULL){
        free(l);
        *s = NULL;
        return NULL;
    }
    char *linha = calloc(TAM_MAX, 1);
    if(linha == NULL){
        fclose(l->f);
        free(l);
        *s = NULL;
        return NULL;
    }
    if(fgets(linha, TAM_MAX, l->f) == NULL){
        free(linha);
        fclose(l->f);
        free(l);
        *s = NULL;
        return NULL;
    }
    int tam = strlen(linha);
    for(int i = tam-1; i>=0; i--){
        if(linha[i] == '\r' || linha[i] == '\n'){
            linha[i] = '\0';
        }
        else{
            break;
        }
    }
    SCHEMA *head = schema_criar(linha);
    if(head == NULL){
        fclose(l->f);
        free(l);
        free(linha);
        *s = NULL;
        return NULL;
    }
    free(linha);
    *s = head;
    l->qtd_col = schema_n_colunas(head);
    l->lidos = 0;
    l->limite = limite;
    return l;
}

ITEM *nasa_proximo_item(LEITOR *l, bool *terminou){
    if(l == NULL || terminou == NULL){
        return NULL;
    }
    if(l->limite != 0 && l->limite == l->lidos){
        *terminou = true;
        return NULL;
    }
    while(true){ //evita linha vazia
        char *linha = calloc(TAM_MAX, 1);
        if(linha == NULL){
            *terminou = false;
            return NULL;
        }
        if(fgets(linha, TAM_MAX, l->f) == NULL){
             free(linha); 
             if(!feof(l->f)){
                *terminou = false;
                return NULL;
             }
             else{
                *terminou = true;
                return NULL;
             }
        }
        int tam = strlen(linha);
        if(linha[tam-1] != '\n' && !feof(l->f)){
            free(linha); 
            *terminou = false; 
            return NULL;
        }
        for(int i = tam-1; i >= 0; i--){  
            if(linha[i] == '\n' || linha[i] == '\r'){
                linha[i] = '\0';
            }
            else{
                break;
            }
        }
        if(linha[0] == '\0'){ //se fazia libera e vai p prox
            free(linha);
            continue;
        }
        int qtdl;
        char **campos = csv_separar_em_strings(linha, &qtdl);
        if(campos == NULL){ 
            free(linha);
            *terminou = false;
            return NULL;
        }
        if(qtdl != l->qtd_col){
            csv_liberar_memoria_separada(campos, qtdl);
            free(linha);
            continue;
        }
        ITEM *it = item_criar(linha, campos, qtdl);
        csv_liberar_memoria_separada(campos, qtdl);
        free(linha); 
        if(it == NULL){ 
            *terminou = false;
            return NULL;
        }
        l->lidos++;
        *terminou = false;
        return it;
    }
}

void nasa_apagar(LEITOR **l){
    if(l == NULL || *l == NULL){
        return;
    }
    fclose((*l)->f);
    free(*l);
    *l = NULL;
    return;
}