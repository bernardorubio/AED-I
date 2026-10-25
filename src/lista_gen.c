#include <stdlib.h>
#include <string.h>

#include "lista_gen.h"





// estrutura:
// inicio -> [high] -> [medium] -> ...      nível 1 (grupos da coluna1)
//              |
//           [2019] -> [2021] -> ...        nível 2 (grupos da coluna2)
//              |
//           (item) -> (item) -> ...        nível 3 (átomos)
// prox liga nós do mesmo nível, filhos desce um nível

typedef struct no_gen{
    int eh_atomo; // 1 = átomo (vale info.atomo), 0 = grupo (vale info.sublista). olhar sempre antes de usar a union
    union {
        ITEM *atomo; // ponteiro do item, a lista não é dona dele
        struct sublista{
            char *rotulo; // chave do grupo (ex.: "high", "2019"), cópia alocada pela lista
            struct no_gen *filhos; // primeiro nó do nível de baixo
        }sublista;
    }info;
    struct no_gen *prox;
}NO_GEN;

struct lista_gen{
    NO_GEN *inicio; // primeiro grupo do nível 1
    int coluna1; // índices das colunas usadas no agrupamento, não os valores
    int coluna2;
    int tamanho; // quantidade de itens (átomos), não de grupos
};

LISTA_GEN *lista_gen_criar(int coluna1, int coluna2){
    if (coluna1 < 0 || coluna2 < 0)
    {
        return NULL;
    }
    LISTA_GEN *lista = (LISTA_GEN*)malloc(sizeof(LISTA_GEN));

    if (lista != NULL)
    {
        lista->inicio = NULL;
        lista->tamanho = 0;
        lista->coluna1 = coluna1;
        lista->coluna2 = coluna2;
    }

    return lista;
    
    
}

int lista_gen_tamanho(LISTA_GEN *lista){
    if (lista == NULL || lista->tamanho < 0)
    {
        return LISTA_GEN_ERRO;
    }

    return lista->tamanho;
    
}

//aux

// mesma lógica do percorrer_nos, mas libera cada nó do nível e, recursivamente, tudo abaixo dele
static void apagar_nos(NO_GEN *no, bool apagar_itens){
    while (no != NULL)
    {
        NO_GEN *proximo = no->prox; // guardar antes do free, depois dele o nó não pode mais ser lido
        if (no->eh_atomo == 0)
        {
            apagar_nos(no->info.sublista.filhos, apagar_itens);
            free(no->info.sublista.rotulo);
        }
        else if(no->eh_atomo == 1 && apagar_itens == true){
            item_apagar(&no->info.atomo);

        }
        free(no);
        no = proximo;

    }

    
    
}

void lista_gen_apagar(LISTA_GEN **lista, bool apagar_itens){
    if (lista == NULL || *lista == NULL)
    {
        return;

    }

    apagar_nos((*lista)->inicio, apagar_itens);
    free(*lista);
    *lista = NULL;
    
}

static NO_GEN *criar_sublista(const char *rotulo){
    if (rotulo == NULL)
    {
        return NULL;
    }

    NO_GEN *no = malloc(sizeof(NO_GEN));

    if (no == NULL)
    {
        return NULL;
    }

    no->eh_atomo = 0;

    // copia o rótulo em vez de apontar para o texto do item: o item pode ser apagado antes do grupo,
    // e o apagar_nos dá free no rótulo. o +1 é para o '\0'
    no->info.sublista.rotulo = malloc(strlen(rotulo) + 1);

    if (no->info.sublista.rotulo == NULL)
    {
        free(no);
        return NULL;
    }

    strcpy(no->info.sublista.rotulo, rotulo);
    


    no->info.sublista.filhos = NULL;
    no->prox = NULL;

    return no;
    
    
}

static NO_GEN *criar_atomo(ITEM *item){
    if (item == NULL)
    {
        return NULL;
    }

    NO_GEN *no = malloc(sizeof(NO_GEN));

    if (no == NULL)
    {
        return NULL;
    }
    no->eh_atomo = 1;

    no->info.atomo = item; // só o ponteiro, sem cópia
    
    no->prox = NULL;

    return no;

    
}

// procura no nível o grupo com rótulo == chave; se não existir, cria e liga no fim (mantém a ordem de chegada).
// recebe NO_GEN ** para conseguir alterar o primeiro nó quando o nível está vazio
static NO_GEN *buscar_ou_criar(NO_GEN **lista, const char *chave){
    NO_GEN *atual = *lista;
    NO_GEN *anterior = NULL;

    while (atual != NULL)
    {
        if (!strcmp(atual->info.sublista.rotulo, chave)) // strcmp devolve 0 quando os textos são iguais
        {
            return atual;
        }

        anterior = atual;
        atual = atual->prox;
        
    }

    NO_GEN *novo = criar_sublista(chave);

    if (novo == NULL)
    {
        return NULL;
    }
    

    if (anterior == NULL)
    {
        *lista = novo;
    }
    else{
        anterior->prox = novo;
    }

    return novo;
    
    
}

// liga novo no fim do nível (NO_GEN ** pelo mesmo motivo do buscar_ou_criar)
static void inserir_no_fim(NO_GEN **lista, NO_GEN *novo){
    if (*lista == NULL)
    {
        *lista = novo;
        return;
    }
    NO_GEN *ultimo = *lista;
    while (ultimo->prox != NULL) // testa o prox para parar em cima do último nó, e não depois dele (NULL)
    {
        ultimo = ultimo->prox;
    }

    ultimo->prox = novo;
    
    
}

bool lista_gen_inserir(LISTA_GEN *lista, ITEM *item){
    if (lista == NULL || item == NULL)
    {
        return false;
    }

    const char *chave1 = item_get_texto(item, lista->coluna1);
    const char *chave2 = item_get_texto(item, lista->coluna2);

    if (chave1 == NULL || chave2 == NULL)
    {
        return false;
    }

    // se algo falhar daqui pra baixo, um grupo criado pode ficar vazio; não vaza, o apagar libera ele
    NO_GEN *grupo1 = buscar_ou_criar(&lista->inicio, chave1);
    if (grupo1 == NULL)
    {
        return false;
    }
    
    NO_GEN *grupo2 = buscar_ou_criar(&grupo1->info.sublista.filhos, chave2);

    if (grupo2 == NULL)
    {
        return false;
    }

    NO_GEN *atomo = criar_atomo(item);

    if (atomo == NULL)
    {
        return false;
    }
    
    inserir_no_fim(&grupo2->info.sublista.filhos, atomo);

    lista->tamanho++;
    
    return true;
}

// visita os itens do nível e de tudo abaixo dele (busca em profundidade: termina um grupo antes de ir para o próximo).
// cada chamada devolve quantos itens visitou, e o grupo de cima soma esses totais
static int percorrer_nos(NO_GEN *no, void (*visitar)(ITEM *)){
    int total = 0;
    while (no != NULL)
    {
        if (no->eh_atomo == 0)
        {
            total += percorrer_nos(no->info.sublista.filhos, visitar);
        }
        else{
            visitar(no->info.atomo);
            total++;
        }
        no = no->prox;
    }

    return total;
    
}

int lista_gen_percorrer(LISTA_GEN *lista, void(*visitar)(ITEM*)){
    if (lista == NULL || visitar == NULL)
    {
        return LISTA_GEN_ERRO;
    }

    return percorrer_nos(lista->inicio, visitar);
    
}




