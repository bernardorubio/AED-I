#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "item.h"
#include "query.h"
#include "estrutura.h"
#include "pilha.h"
#include "fila.h"
#include "lista_seq.h"
#include "lista_enc.h"
#include "lista_cabeca.h"
#include "lista_ord.h"
#include "lista_gen.h"
#include "lista_cruz.h"

typedef enum {
    est_pilha,
    est_fila,
    est_lista_seq,
    est_lista_enc,
    est_lista_cabeca,
    est_lista_ord,
    est_lista_gen,
    est_lista_cruz
} TIPO_EST;

struct estrutura_{
    TIPO_EST tipo;
    void *dados;
    ITEM **vetor_gen;
    int qtd_gen;
    int pos_gen;
};

//chave do ORDER usada pelo comparador da lista_ord. o comparador so recebe os dois itens,
//entao a coluna e a direcao ficam aqui e sao preenchidas no estrutura_criar
static int ord_coluna = -1;
static bool ord_similaridade = false;
static bool ord_decrescente = false;

//sem ORDER na consulta: todos empatam, e a lista_ord mantem a ordem de chegada
static int comparar_nada(ITEM *a, ITEM *b){
    (void)a;
    (void)b;
    return 0;
}

//compara pela similaridade ou pela coluna do ORDER. numero compara como numero, texto como texto,
//campo vazio vai para o fim; com DESC o sinal e invertido
static int comparar_order(ITEM *a, ITEM *b){
    int cmp;
    if(ord_similaridade){
        double x = item_get_similaridade(a);
        double y = item_get_similaridade(b);
        cmp = (x > y) - (x < y);
    }
    else{
        const char *ta = item_get_texto(a, ord_coluna);
        const char *tb = item_get_texto(b, ord_coluna);
        bool a_vazio = (ta == NULL || ta[0] == '\0');
        bool b_vazio = (tb == NULL || tb[0] == '\0');
        if(a_vazio && b_vazio){
            return 0;
        }
        if(a_vazio){
            return 1;
        }
        if(b_vazio){
            return -1;
        }
        double x = item_get_valor(a, ord_coluna);
        double y = item_get_valor(b, ord_coluna);
        if(!isnan(x) && !isnan(y)){
            cmp = (x > y) - (x < y);
        }
        else{
            int r = strcmp(ta, tb);
            cmp = (r > 0) - (r < 0);
        }
    }
    if(ord_decrescente){
        cmp = -cmp;
    }
    return cmp;
}

static bool tipo_do_nome(char *nome, TIPO_EST *tipo){
    if(strcmp(nome, "pilha") == 0){
        *tipo = est_pilha;
    }
    else if(strcmp(nome, "fila") == 0){
        *tipo = est_fila;
    }
    else if(strcmp(nome, "lista_seq") == 0){
        *tipo = est_lista_seq;
    }
    else if(strcmp(nome, "lista_enc") == 0){
        *tipo = est_lista_enc;
    }
    else if(strcmp(nome, "lista_cabeca") == 0){
        *tipo = est_lista_cabeca;
    }
    else if(strcmp(nome, "lista_ord") == 0){
        *tipo = est_lista_ord;
    }
    else if(strcmp(nome, "lista_gen") == 0){
        *tipo = est_lista_gen;
    }
    else if(strcmp(nome, "lista_cruz") == 0){
        *tipo = est_lista_cruz;
    }
    else{
        return false;
    }
    return true;
}

//cria a estrutura escolhida. lista_ord pega o ORDER, lista_gen o GROUP e lista_cruz o CROSS da consulta;
//lista_gen sem GROUP ou lista_cruz sem CROSS devolve NULL (o main imprime STATUS=ERRO)
ESTRUTURA *estrutura_criar(char *nome, QUERY *q){
    if(nome == NULL || q == NULL){
        return NULL;
    }
    TIPO_EST tipo;
    if(!tipo_do_nome(nome, &tipo)){
        return NULL;
    }
    ESTRUTURA *e = malloc(sizeof(ESTRUTURA));
    if(e == NULL){
        return NULL;
    }
    e->tipo = tipo;
    e->dados = NULL;
    e->vetor_gen = NULL;
    e->qtd_gen = 0;
    e->pos_gen = 0;
    int coluna1 = -1;
    int coluna2 = -1;
    switch(tipo){
        case est_pilha:
            e->dados = pilha_criar();
            break;
        case est_fila:
            e->dados = fila_criar();
            break;
        case est_lista_seq:
            e->dados = lista_seq_criar();
            break;
        case est_lista_enc:
            e->dados = lista_enc_criar();
            break;
        case est_lista_cabeca:
            e->dados = lista_cabeca_criar();
            break;
        case est_lista_ord:
            ord_coluna = -1;
            ord_similaridade = false;
            ord_decrescente = false;
            if(query_get_order(q, &ord_coluna, &ord_similaridade, &ord_decrescente)){
                e->dados = lista_ord_criar(comparar_order);
            }
            else{
                e->dados = lista_ord_criar(comparar_nada);
            }
            break;
        case est_lista_gen:
            if(query_get_group(q, &coluna1, &coluna2)){
                e->dados = lista_gen_criar(coluna1, coluna2);
            }
            break;
        case est_lista_cruz:
            if(query_get_cross(q, &coluna1, &coluna2)){
                e->dados = lista_cruz_criar(coluna1, coluna2);
            }
            break;
    }
    if(e->dados == NULL){
        free(e);
        return NULL;
    }
    return e;
}

//a lista_gen percorre com callback (lista_gen_percorrer), que so recebe o item;
//estas variaveis dizem ao callback em qual vetor e posicao copiar
static ITEM **gen_destino = NULL;
static int gen_tam = 0;
static int gen_pos = 0;

//callback da lista_gen: copia cada item visitado para o vetor, na ordem grupo por grupo
static void copiar_para_vetor(ITEM *item){
    if(gen_pos < gen_tam){
        gen_destino[gen_pos] = item;
        gen_pos++;
    }
    return;
}

bool estrutura_inserir(ESTRUTURA *e, ITEM *item){
    if(e == NULL || item == NULL){
        return false;
    }
    switch(e->tipo){
        case est_pilha:
            return pilha_empilhar(e->dados, item);
        case est_fila:
            return fila_inserir(e->dados, item);
        case est_lista_seq:
            return lista_seq_inserir(e->dados, item);
        case est_lista_enc:
            return lista_enc_inserir(e->dados, item);
        case est_lista_cabeca:
            return lista_cabeca_inserir(e->dados, item);
        case est_lista_ord:
            return lista_ord_inserir(e->dados, item);
        case est_lista_gen:
            return lista_gen_inserir(e->dados, item);
        case est_lista_cruz:
            return lista_cruz_inserir(e->dados, item);
    }
    return false;
}

//comeca o percurso e devolve o primeiro item. na lista_gen, copia todos os itens para um vetor
//e passa a percorrer o vetor, para funcionar com primeiro/proximo como as outras
ITEM *estrutura_primeiro(ESTRUTURA *e){
    if(e == NULL){
        return NULL;
    }
    switch(e->tipo){
        case est_pilha:
            return pilha_primeiro(e->dados);
        case est_fila:
            return fila_primeiro(e->dados);
        case est_lista_seq:
            return lista_seq_primeiro(e->dados);
        case est_lista_enc:
            return lista_enc_primeiro(e->dados);
        case est_lista_cabeca:
            return lista_cabeca_primeiro(e->dados);
        case est_lista_ord:
            return lista_ord_primeiro(e->dados);
        case est_lista_gen:
            free(e->vetor_gen);
            e->vetor_gen = NULL;
            e->qtd_gen = 0;
            e->pos_gen = 0;
            int tam = lista_gen_tamanho(e->dados);
            if(tam <= 0){
                return NULL;
            }
            e->vetor_gen = malloc(tam * sizeof(ITEM *));
            if(e->vetor_gen == NULL){
                return NULL;
            }
            gen_destino = e->vetor_gen;
            gen_tam = tam;
            gen_pos = 0;
            lista_gen_percorrer(e->dados, copiar_para_vetor);
            e->qtd_gen = gen_pos;
            gen_destino = NULL;
            gen_tam = 0;
            gen_pos = 0;
            return estrutura_proximo(e);
        case est_lista_cruz:
            return lista_cruz_primeiro(e->dados);
    }
    return NULL;
}

ITEM *estrutura_proximo(ESTRUTURA *e){
    if(e == NULL){
        return NULL;
    }
    switch(e->tipo){
        case est_pilha:
            return pilha_proximo(e->dados);
        case est_fila:
            return fila_proximo(e->dados);
        case est_lista_seq:
            return lista_seq_proximo(e->dados);
        case est_lista_enc:
            return lista_enc_proximo(e->dados);
        case est_lista_cabeca:
            return lista_cabeca_proximo(e->dados);
        case est_lista_ord:
            return lista_ord_proximo(e->dados);
        case est_lista_gen:
            if(e->pos_gen >= e->qtd_gen){
                return NULL;
            }
            e->pos_gen++;
            return e->vetor_gen[e->pos_gen - 1];
        case est_lista_cruz:
            return lista_cruz_proximo(e->dados);
    }
    return NULL;
}

void estrutura_apagar(ESTRUTURA **e, bool apagar_itens){
    if(e == NULL || *e == NULL){
        return;
    }
    switch((*e)->tipo){
        case est_pilha: {
            PILHA *p = (*e)->dados;
            pilha_apagar(&p, apagar_itens);
            break;
        }
        case est_fila: {
            FILA *f = (*e)->dados;
            fila_apagar(&f, apagar_itens);
            break;
        }
        case est_lista_seq: {
            LISTA_SEQ *l = (*e)->dados;
            lista_seq_apagar(&l, apagar_itens);
            break;
        }
        case est_lista_enc: {
            LISTA_ENC *l = (*e)->dados;
            lista_enc_apagar(&l, apagar_itens);
            break;
        }
        case est_lista_cabeca: {
            LISTA_CABECA *l = (*e)->dados;
            lista_cabeca_apagar(&l, apagar_itens);
            break;
        }
        case est_lista_ord: {
            LISTA_ORD *l = (*e)->dados;
            lista_ord_apagar(&l, apagar_itens);
            break;
        }
        case est_lista_gen: {
            LISTA_GEN *l = (*e)->dados;
            lista_gen_apagar(&l, apagar_itens);
            break;
        }
        case est_lista_cruz: {
            LISTA_CRUZ *l = (*e)->dados;
            lista_cruz_apagar(&l, apagar_itens);
            break;
        }
    }
    free((*e)->vetor_gen);
    free(*e);
    *e = NULL;
    return;
}

//switch case de desicao das estruturas;
//enum pra evitar switch case nao escolher com strings
//switch volta no apagar pra apagar a estrutura certa
//basicamente, escolher qual estrutura pra criar apagar inserir etc.