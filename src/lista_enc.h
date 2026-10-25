#ifndef LISTA_ENC_H
#define LISTA_ENC_H

#include <stdbool.h>
#include "item.h"

typedef struct lista_enc_ LISTA_ENC;

// lista encadeada simples, guarda o primeiro e o ultimo no
LISTA_ENC *lista_enc_criar(void);
// libera a lista; os itens so sao apagados se apagar_itens for true (use false no result, pois a base ja vai apagar os itens)
void lista_enc_apagar(LISTA_ENC **lista, bool apagar_itens);
// insere no final sem percorrer a lista, usando o ponteiro do ultimo no
bool lista_enc_inserir(LISTA_ENC *lista, ITEM *item);
// insere em qualquer posicao de 0 ate o tamanho, andando ate o no anterior; false se pos for invalida
bool lista_enc_inserir_posicao(LISTA_ENC *lista, ITEM *item, int pos);
// percorre do inicio ao fim: primeiro vai para o inicio e proximo avanca um no por chamada; devolvem NULL quando nao ha mais item
ITEM *lista_enc_primeiro(LISTA_ENC *lista);
ITEM *lista_enc_proximo(LISTA_ENC *lista);

#endif
