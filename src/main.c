#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "item.h"
#include "schema.h"
#include "nasa.h"
#include "cenipa.h"
#include "query.h"
#include "estrutura.h"

static double agora_ms(void){
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
}

static bool ler_inteiro(char *texto, int *saida){
    char *fim;
    long valor = strtol(texto, &fim, 10);
    if(fim == texto || *fim != '\0'){
        return false;
    }
    if(valor < 0 || valor > INT_MAX){
        return false;
    }
    *saida = (int)valor;
    return true;
}

//o resultado e apagado sem os itens (false) porque so guarda ponteiros; quem libera os itens e a base (true)
static void liberar_tudo(ESTRUTURA **resultado, ESTRUTURA **base, LEITOR_NASA **l, LEITOR_CENIPA **lc, QUERY **q, SCHEMA **s){
    estrutura_apagar(resultado, false);
    estrutura_apagar(base, true);
    nasa_apagar(l);
    cenipa_apagar(lc);
    query_apagar(q);
    schema_apagar(s);
    return;
}

static int erro(ESTRUTURA **resultado, ESTRUTURA **base, LEITOR_NASA **l, LEITOR_CENIPA **lc, QUERY **q, SCHEMA **s){
    liberar_tudo(resultado, base, l, lc, q, s);
    printf("STATUS=ERRO\n");
    return 1;
}

int main(int argc, char *argv[]){
    char *reader = NULL;
    char *input = NULL;
    char *nome_base = NULL;
    char *nome_resultado = NULL;
    char *arq_query = NULL;
    char *output = NULL;
    char *texto_topk = NULL;
    char *texto_limit = NULL;

    for(int i = 1; i < argc; i += 2){
        if(i + 1 >= argc){
            printf("STATUS=ERRO\n");
            return 1;
        }
        char **destino = NULL;
        if(strcmp(argv[i], "--reader") == 0){
            destino = &reader;
        }
        else if(strcmp(argv[i], "--input") == 0){
            destino = &input;
        }
        else if(strcmp(argv[i], "--base") == 0){
            destino = &nome_base;
        }
        else if(strcmp(argv[i], "--result") == 0){
            destino = &nome_resultado;
        }
        else if(strcmp(argv[i], "--query") == 0){
            destino = &arq_query;
        }
        else if(strcmp(argv[i], "--output") == 0){
            destino = &output;
        }
        else if(strcmp(argv[i], "--top-k") == 0){
            destino = &texto_topk;
        }
        else if(strcmp(argv[i], "--limit") == 0){
            destino = &texto_limit;
        }
        if(destino == NULL || *destino != NULL){
            printf("STATUS=ERRO\n");
            return 1;
        }
        *destino = argv[i + 1];
    }
    if(reader == NULL || input == NULL || nome_base == NULL || nome_resultado == NULL ||
       arq_query == NULL || output == NULL || texto_topk == NULL || texto_limit == NULL){
        printf("STATUS=ERRO\n");
        return 1;
    }
    int top_k;
    int limit;
    if(!ler_inteiro(texto_topk, &top_k) || !ler_inteiro(texto_limit, &limit)){
        printf("STATUS=ERRO\n");
        return 1;
    }
    //leitores aceitos: nasa (parte 1) e cenipa (parte 2)
    bool eh_cenipa = (strcmp(reader, "cenipa") == 0);
    if(strcmp(reader, "nasa") != 0 && !eh_cenipa){
        printf("STATUS=ERRO\n");
        return 1;
    }

    SCHEMA *s = NULL;
    LEITOR_NASA *l = NULL;
    LEITOR_CENIPA *lc = NULL;
    QUERY *q = NULL;
    ESTRUTURA *base = NULL;
    ESTRUTURA *resultado = NULL;

    if(eh_cenipa){
        lc = cenipa_abrir_arquivo(input, limit, &s);
    }
    else{
        l = nasa_abrir_arquivo(input, limit, &s);
    }
    if(l == NULL && lc == NULL){
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }
    q = query_ler_arquivo(arq_query, s);
    if(q == NULL){
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }
    //a base e criada depois de ler a consulta porque lista_ord, lista_gen e lista_cruz usam o ORDER, GROUP e CROSS na criacao
    base = estrutura_criar(nome_base, q);
    if(base == NULL){
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }

    int input_records = 0;
    double t_inicio = agora_ms();
    while(true){
        bool terminou;
        ITEM *it;
        if(eh_cenipa){
            it = cenipa_proximo_item(lc, &terminou);
        }
        else{
            it = nasa_proximo_item(l, &terminou);
        }
        if(it == NULL){
            if(terminou){
                break;
            }
            return erro(&resultado, &base, &l, &lc, &q, &s);
        }
        if(!estrutura_inserir(base, it)){
            item_apagar(&it);
            return erro(&resultado, &base, &l, &lc, &q, &s);
        }
        input_records++;
    }
    double load_ms = agora_ms() - t_inicio;
    nasa_apagar(&l);
    cenipa_apagar(&lc);

    resultado = estrutura_criar(nome_resultado, q);
    if(resultado == NULL){
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }

    //busca: percorre a base e insere os aceitos no --result.
    //o tempo das insercoes e somado a parte e descontado do tempo da busca
    int matched_records = 0;
    double insert_ms = 0.0;
    t_inicio = agora_ms();
    for(ITEM *it = estrutura_primeiro(base); it != NULL; it = estrutura_proximo(base)){
        if(query_avaliar(q, it)){
            double t_insere = agora_ms();
            bool ok = estrutura_inserir(resultado, it);
            insert_ms += agora_ms() - t_insere;
            if(!ok){
                return erro(&resultado, &base, &l, &lc, &q, &s);
            }
            matched_records++;
        }
    }
    double search_ms = agora_ms() - t_inicio - insert_ms;

    //saida: cabecalho e os primeiros k itens na ordem de acesso do --result,
    //com \r\n no fim de cada linha, igual aos arquivos de entrada (nasa e cenipa)
    int output_records = 0;
    t_inicio = agora_ms();
    FILE *saida = fopen(output, "w");
    if(saida == NULL){
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }
    fprintf(saida, "%s\r\n", schema_get_header(s));
    for(ITEM *it = estrutura_primeiro(resultado); it != NULL; it = estrutura_proximo(resultado)){
        if(top_k != 0 && output_records >= top_k){
            break;
        }
        fprintf(saida, "%s\r\n", item_get_linha(it));
        output_records++;
    }
    if(fclose(saida) != 0){ //o fclose grava o que ainda estava no buffer, entao um erro de escrita pode aparecer so aqui
        return erro(&resultado, &base, &l, &lc, &q, &s);
    }
    double write_ms = agora_ms() - t_inicio;

    printf("STATUS=OK\n");
    printf("INPUT_RECORDS=%d\n", input_records);
    printf("MATCHED_RECORDS=%d\n", matched_records);
    printf("OUTPUT_RECORDS=%d\n", output_records);
    printf("LOAD_MS=%.3f\n", load_ms);
    printf("SEARCH_MS=%.3f\n", search_ms);
    printf("RESULT_INSERT_MS=%.3f\n", insert_ms);
    printf("WRITE_MS=%.3f\n", write_ms);

    liberar_tudo(&resultado, &base, &l, &lc, &q, &s);
    return 0;
}
