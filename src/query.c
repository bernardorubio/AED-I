#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "schema.h"
#include "item.h"
#include "query.h"
#include "csv.h"
#define FILTROS_MAX 32
#define SIM_MAX 32
#define TAM_MAX 1000

typedef enum {
    op_maiorigual,
    op_menorigual,
    op_maior,
    op_menor,
    op_igual,
    op_diferente
} OP;


typedef struct filter_{
    int coluna;
    char *valor_texto;
    double valor_num; //strtod
    bool eh_numero;
    OP operacao;
}FILTER;

typedef struct sim_{
    int coluna;
    double alvo;
    double tolerancia;
    double peso;
}SIM;

struct query_{
    FILTER filtros[FILTROS_MAX];
    int qtd_filtros;
    SIM similaridades[SIM_MAX];
    int qtd_crit;
    bool com_accept;
    double limiar_accept;
    bool com_order;
    int order_coluna;
    bool order_similaridade;
    bool ordem_decrescente;
    bool com_group;
    int group_coluna1;
    int group_coluna2;
    bool com_cross;
    int cross_coluna1;
    int cross_coluna2;
};

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


//sim e accept pra evitar uso de nmr invalido
static bool ler_numero(char *texto, double *saida){
    if(texto == NULL || saida == NULL){ 
        return false;
    }
    char *fim; 
    double valor = strtod(texto, &fim);
    if(fim == texto || *fim != '\0'){ 
        return false;
    }
    if(!isfinite(valor)){ //evitar nan e inf como limiar
        return false;
    }
    *saida = valor;
    return true;
}

//func de cada cmd

static bool func_filter(QUERY *q, char **partes, int qtd_partes, SCHEMA *s){
    if(qtd_partes != 4){
        return false;
    }
    if(q->qtd_filtros >= FILTROS_MAX){
        return false;
    }
    int ind;
    ind = schema_indice_nome(s, partes[1]);
    if(ind == -1){
        return false;
    }
    OP d;
    if(strcmp(partes[2], ">=") == 0){
        d = op_maiorigual;
    }
    else if(strcmp(partes[2], "<=") == 0){
        d = op_menorigual;
    }
    else if(strcmp(partes[2], ">") == 0){
        d = op_maior;
    }
    else if(strcmp(partes[2], "<") == 0){
        d = op_menor;
    }
    else if(strcmp(partes[2], "==") == 0){
        d = op_igual;
    }
    else if(strcmp(partes[2], "!=") == 0){
        d = op_diferente;
    }
    else{
        return false;
    }
    q->filtros[q->qtd_filtros].coluna = ind;
    q->filtros[q->qtd_filtros].operacao = d;
    q->filtros[q->qtd_filtros].valor_texto = copiar_string_alocando(partes[3]);
    if(q->filtros[q->qtd_filtros].valor_texto == NULL){
        return false;
    }
    double t;
    if(ler_numero(partes[3], &t)){
        q->filtros[q->qtd_filtros].eh_numero = true;
        q->filtros[q->qtd_filtros].valor_num = t;
    }
    else{
        q->filtros[q->qtd_filtros].eh_numero = false;
    }
    q->qtd_filtros++;
    return true;
}

static bool func_sim(QUERY *q, char **partes, int qtd_partes, SCHEMA *s){
    if(qtd_partes != 5){ //sim+4 arg pra ver se sim ta com todos os args
        return false;
    }
    if(q->qtd_crit >= SIM_MAX){
        return false;
    }
    int ind;
    ind = schema_indice_nome(s, partes[1]); 
    if(ind == -1){ 
        return false;
    }
    double alvo, tolerancia, peso;
    if(!ler_numero(partes[2], &alvo) || !ler_numero(partes[3], &tolerancia) || !ler_numero(partes[4], &peso)){
        return false; //3 numeros validos
    }
    if(tolerancia <= 0 || peso < 0){
        return false;
    }
    q->similaridades[q->qtd_crit].coluna = ind;
    q->similaridades[q->qtd_crit].alvo = alvo;
    q->similaridades[q->qtd_crit].tolerancia = tolerancia;
    q->similaridades[q->qtd_crit].peso = peso;
    q->qtd_crit++;
    return true;
}

static bool func_accept(QUERY *q, char **partes, int qtd_partes){
    if(qtd_partes != 2){ //accept + 1arg
        return false;
    }
    if(q->com_accept){ //[flag pra evitar cmd repetido
        return false;
    }
    double limiar;
    if(!ler_numero(partes[1], &limiar)){
        return false;
    }
    q->com_accept = true;
    q->limiar_accept = limiar;
    return true;
}

static bool func_order(QUERY *q, char **partes, int qtd_partes, SCHEMA *s){
    if(qtd_partes != 3){ //ordem tem 2 argumetnos
        return false;
    }
    if(q->com_order){
        return false;
    }
    bool por_similaridade;
    int coluna;
    if(strcmp(partes[1], "similarity") == 0){
        por_similaridade = true;
        coluna = -1;
    }
    else{
        por_similaridade = false;
        coluna = schema_indice_nome(s, partes[1]);
        if(coluna == -1){
            return false;
        }
    }
    //so com maiusculas pra dir
    bool decrescente;
    if(strcmp(partes[2], "ASC") == 0){
        decrescente = false;
    }
    else if(strcmp(partes[2], "DESC") == 0){
        decrescente = true;
    }
    else{
        return false;
    }
    q->com_order = true;
    q->order_similaridade = por_similaridade;
    q->order_coluna = coluna;
    q->ordem_decrescente = decrescente;
    return true;
}

static bool func_colunas(char **partes, int qtd_partes, SCHEMA *s, bool *com, int *coluna1, int *coluna2){
    if(qtd_partes != 3){ //group + 2col
        return false;
    }
    if(*com){ //evitar repeticao
        return false;
    }
    int c1 = schema_indice_nome(s, partes[1]);
    int c2 = schema_indice_nome(s, partes[2]);
    if(c1 == -1 || c2 == -1){
        return false;
    }
    //aceitar subgrupo se vier 2 col repetidas
    *com = true;
    *coluna1 = c1;
    *coluna2 = c2;
    return true;
}


static char minuscula(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + ('a' - 'A');
    }
    return c;
}

static int comparar_sem_maiusculas(const char *a, const char *b){
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
        char ca = minuscula(a[i]);
        char cb = minuscula(b[i]);
        if(ca != cb){
            return ca - cb;
        }
        i++;
    }
    return minuscula(a[i]) - minuscula(b[i]);
}

static bool aplicar_operacao(OP operacao, int cmp){
    if(operacao == op_maiorigual){
        return cmp >= 0;
    }
    else if(operacao == op_menorigual){
        return cmp <= 0;
    }
    else if(operacao == op_maior){
        return cmp > 0;
    }
    else if(operacao == op_menor){
        return cmp < 0;
    }
    else if(operacao == op_igual){
        return cmp == 0;
    }
    else{
        return cmp != 0;
    }
}

static bool passa_filtro(FILTER *f, ITEM *item){
    const char *texto = item_get_texto(item, f->coluna);
    if(texto == NULL || texto[0] == '\0'){
        return false;
    }
    int cmp;
    if(f->eh_numero){
        double x = item_get_valor(item, f->coluna);
        if(isnan(x)){
            return false;
        }
        cmp = (x > f->valor_num) - (x < f->valor_num);
    }
    else{
        cmp = comparar_sem_maiusculas(texto, f->valor_texto);
    }
    return aplicar_operacao(f->operacao, cmp);
}

static double nota_sim(SIM *c, ITEM *item){
    double x = item_get_valor(item, c->coluna);
    if(isnan(x)){
        return 0.0;
    }
    double nota = 1.0 - fabs(x - c->alvo) / c->tolerancia;
    if(nota < 0.0){
        nota = 0.0;
    }
    return nota;
}

QUERY *query_ler_arquivo(char *endereco, SCHEMA *s){
    if(endereco == NULL || s == NULL){
        return NULL;
    }
    QUERY *q = malloc(sizeof(QUERY));
    if(q == NULL){
        return NULL;
    }
    //inicializacao
    q->qtd_filtros = 0;
    q->qtd_crit = 0;
    q->com_accept = false;
    q->limiar_accept = 0.0;
    q->com_order = false;
    q->order_coluna = -1;
    q->order_similaridade = false;
    q->ordem_decrescente = false;
    q->com_group = false;
    q->group_coluna1 = -1;
    q->group_coluna2 = -1;
    q->com_cross = false;
    q->cross_coluna1 = -1;
    q->cross_coluna2 = -1;
    FILE *arq = fopen(endereco, "r");
    if(arq == NULL){
        query_apagar(&q);
        return NULL;
    }
    while(true){
        char *linha = calloc(TAM_MAX, 1);
        if(linha == NULL){
            fclose(arq);
            query_apagar(&q);
            return NULL;
        }
        if(fgets(linha, TAM_MAX, arq) == NULL){
            free(linha);
            if(!feof(arq)){ // erro de leitura
                fclose(arq);
                query_apagar(&q);
                return NULL;
            }
            break;
        }
        int tam = strlen(linha);
        if(linha[tam-1] != '\n' && !feof(arq)){
            free(linha);
            fclose(arq);
            query_apagar(&q);
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
        if(linha[0] == '\0'){
            free(linha);
            continue;
        }
        int qtd_partes;
        char **partes = csv_separar_em_strings(linha, &qtd_partes, '|');
        free(linha);
        if(partes == NULL){
            fclose(arq);
            query_apagar(&q);
            return NULL;
        }

        bool ok = true;
        //escolher comando com strcmp == 0 
        if(strcmp(partes[0], "FILTER") == 0){
            ok = func_filter(q, partes, qtd_partes, s);
        }
        else if(strcmp(partes[0], "SIM") == 0){
            ok = func_sim(q, partes, qtd_partes, s);
        }
        else if(strcmp(partes[0], "ACCEPT") == 0){
            ok = func_accept(q, partes, qtd_partes);
        }
        else if(strcmp(partes[0], "ORDER") == 0){
            ok = func_order(q, partes, qtd_partes, s);
        }
        else if(strcmp(partes[0], "GROUP") == 0){
            ok = func_colunas(partes, qtd_partes, s, &q->com_group, &q->group_coluna1, &q->group_coluna2);
        }
        else if(strcmp(partes[0], "CROSS") == 0){
            ok = func_colunas(partes, qtd_partes, s, &q->com_cross, &q->cross_coluna1, &q->cross_coluna2);
        }
        else{ //nao tem esse comando
            ok = false;
        }

        csv_liberar_memoria_separada(partes, qtd_partes);
        if(!ok){
            fclose(arq);
            query_apagar(&q);
            return NULL;
        }
    }
    fclose(arq);
    return q;
}

bool query_avaliar(QUERY *q, ITEM *item){
    if(q == NULL || item == NULL){
        return false;
    }
    for(int i = 0; i < q->qtd_filtros; i++){
        if(!passa_filtro(&q->filtros[i], item)){
            return false;
        }
    }
    if(q->qtd_crit == 0){
        item_set_similaridade(item, 1.0);
        return true;
    }
    double soma = 0.0;
    double soma_pesos = 0.0;
    for(int i = 0; i < q->qtd_crit; i++){
        soma += q->similaridades[i].peso * nota_sim(&q->similaridades[i], item);
        soma_pesos += q->similaridades[i].peso;
    }
    double similaridade = 0.0;
    if(soma_pesos > 0.0){
        similaridade = soma / soma_pesos;
    }
    item_set_similaridade(item, similaridade);
    if(q->com_accept && similaridade < q->limiar_accept){
        return false;
    }
    return true;
}

//getters
bool query_get_order(QUERY *q, int *coluna, bool *por_similaridade, bool *decrescente){
    if(q == NULL || coluna == NULL || por_similaridade == NULL || decrescente == NULL){
        return false;
    }
    if(!q->com_order){
        return false;
    }
    *coluna = q->order_coluna;
    *por_similaridade = q->order_similaridade;
    *decrescente = q->ordem_decrescente;
    return true;
}

bool query_get_group(QUERY *q, int *coluna1, int *coluna2){
    if(q == NULL || coluna1 == NULL || coluna2 == NULL){
        return false;
    }
    if(!q->com_group){
        return false;
    }
    *coluna1 = q->group_coluna1;
    *coluna2 = q->group_coluna2;
    return true;
}

bool query_get_cross(QUERY *q, int *coluna1, int *coluna2){
    if(q == NULL || coluna1 == NULL || coluna2 == NULL){
        return false;
    }
    if(!q->com_cross){
        return false;
    }
    *coluna1 = q->cross_coluna1;
    *coluna2 = q->cross_coluna2;
    return true;
}

void query_apagar(QUERY **q){
    if(q == NULL || *q == NULL){
        return;
    }
    for(int i = 0; i < (*q)->qtd_filtros; i++){
        free((*q)->filtros[i].valor_texto);
    }
    free(*q);
    *q = NULL;
    return;
}
