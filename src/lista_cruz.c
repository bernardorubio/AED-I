#include "item.h"
#include "lista_cruz.h"

#include <stdlib.h>
#include <string.h>

// estrutura (ex.: linhas = threat_category, colunas = year):
//              [2020] -> [2021] -> ...      cabeças de coluna
//                |         |
// [alta]  ---> (A)  ---> (B)
//                          |
// [media] --------------> (C)
//    |
//   ...   cabeças de linha
// cada nó está ao mesmo tempo numa linha (prox_linha) e numa coluna (prox_coluna)

typedef struct no_cruz
{
    ITEM *item; // ponteiro do item, a lista não é dona dele
    struct no_cruz *prox_linha; // próximo nó da mesma linha (direita)
    struct no_cruz *prox_coluna; // próximo nó da mesma coluna (baixo)

}NO_CRUZ;

// a mesma struct serve para cabeça de linha e de coluna, só muda por qual ponteiro se anda nos nós
typedef struct cabeca
{
    char *rotulo; // valor do campo (ex.: "alta", "2021"), cópia alocada pela lista
    NO_CRUZ *primeiro; // primeiro nó da linha/coluna
    struct cabeca *prox; // próxima cabeça do mesmo tipo
}CABECA;

struct lista_cruz{
    CABECA *inicio_colunas;
    CABECA *inicio_linhas;

    int campo_linha; // índices das colunas do CSV usadas, não os valores
    int campo_coluna;

    int tamanho; // quantidade de itens (nós), não de cabeças

    CABECA *atual_cabeca; // cursor do primeiro/proximo: linha em que está
    NO_CRUZ *atual; // e o nó dentro dela

};


LISTA_CRUZ *lista_cruz_criar(int campo_linha, int campo_coluna){
    if (campo_linha < 0 || campo_coluna < 0 )
    {
        return NULL;

    }

    LISTA_CRUZ *lista = (LISTA_CRUZ *)malloc(sizeof(LISTA_CRUZ));


    if (lista != NULL)
    {
        lista->campo_linha = campo_linha;
        lista->campo_coluna = campo_coluna;
        lista->tamanho = 0;
        lista->atual_cabeca = NULL;
        lista->atual = NULL;
        lista->inicio_colunas = NULL;
        lista->inicio_linhas = NULL;
    }
    

    return lista;
    
}

//aux

// cria uma cabeça vazia (sem nós e sem próxima). serve tanto para linha quanto para coluna
static CABECA *criar_cabeca(const char *rotulo){
    if (rotulo == NULL)
    {
        return NULL;
    }

    CABECA *cabeca = malloc(sizeof(CABECA));

    if (cabeca == NULL)
    {
        return NULL;
    }

    // copia o rótulo em vez de apontar para o texto do item: o item pode ser apagado antes da lista,
    // e o apagar_cabeca dá free no rótulo. o +1 é para o '\0'
    cabeca->rotulo = malloc(strlen(rotulo) + 1);

    if (cabeca->rotulo == NULL)
    {
        free(cabeca); // a cabeça já foi alocada, então libera antes de desistir
        return NULL;
    }

    strcpy(cabeca->rotulo, rotulo);

    cabeca->prox = NULL;
    cabeca->primeiro = NULL;


    return cabeca;
}

// cria um nó solto; quem liga ele na linha e na coluna é o inserir
static NO_CRUZ *criar_no(ITEM *item){
    if (item == NULL)
    {
        return NULL;
    }

    NO_CRUZ *no = malloc(sizeof(NO_CRUZ));

    if (no == NULL)
    {
        return NULL;
    }
    no->item = item; // só o ponteiro, sem cópia

    no->prox_coluna = NULL;
    no->prox_linha = NULL;

    return no;
}

bool lista_cruz_inserir(LISTA_CRUZ *lista, ITEM *item){
    if (lista == NULL || item == NULL)
    {
        return false;
    }
    
    
    const char *rotulo_coluna = item_get_texto(item, lista->campo_coluna);
    const char *rotulo_linha = item_get_texto(item, lista->campo_linha);

    if (rotulo_coluna == NULL || rotulo_linha == NULL) // item_get_texto devolve NULL se o índice não existe no item
    {
        return false;
    }

    // 1) acha a cabeça da linha; se não existir, cria e liga no fim (mantém a ordem de chegada)
    CABECA *cursor_linha = lista->inicio_linhas;
    CABECA *anterior_linha= NULL; // anda um passo atrás: quando o cursor chega em NULL, ele é a última cabeça



    // testa NULL antes do strcmp: o && para no primeiro falso e não lê cursor_linha->rotulo
    while (cursor_linha != NULL && strcmp(rotulo_linha,cursor_linha->rotulo ) != 0)
    {
        anterior_linha = cursor_linha;
        cursor_linha = cursor_linha->prox;
    }

    if (cursor_linha == NULL)
    {
        CABECA *nova_cabeca_linha = criar_cabeca(rotulo_linha);
        if (nova_cabeca_linha == NULL)
        {
            return false;
        }

        if (anterior_linha == NULL) // não tinha nenhuma linha ainda
        {
            lista->inicio_linhas = nova_cabeca_linha;
        }
        else{
            anterior_linha->prox = nova_cabeca_linha;
        }

        cursor_linha = nova_cabeca_linha; // assim, achando ou criando, cursor_linha sempre termina na cabeça certa
        
       
        
    }

    // 2) mesma coisa para a coluna
    CABECA *cursor_coluna = lista->inicio_colunas;
    CABECA *anterior_coluna = NULL;


    while (cursor_coluna != NULL && strcmp(rotulo_coluna, cursor_coluna->rotulo))
    {
        anterior_coluna = cursor_coluna;
        cursor_coluna = cursor_coluna->prox;
    }

    if (cursor_coluna == NULL)
    {
        CABECA *nova_cabeca_coluna = criar_cabeca(rotulo_coluna);
        if (nova_cabeca_coluna == NULL)
        {
            return false;
        }

        if (anterior_coluna == NULL)
        {
            lista->inicio_colunas = nova_cabeca_coluna;
        }
        else{
            anterior_coluna->prox = nova_cabeca_coluna;
        }

        cursor_coluna = nova_cabeca_coluna;
        
        
    }

    // 3) cria o nó e pendura no fim da linha e no fim da coluna (é o mesmo nó nas duas)
    // se falhar aqui, uma cabeça criada acima pode ficar vazia; não vaza, o apagar libera e o percorrer pula ela
    NO_CRUZ *no = criar_no(item);

    if (no == NULL)
    {
        return false;
    }

    if (cursor_linha->primeiro == NULL) // linha vazia: o nó vira o primeiro
    {
        cursor_linha->primeiro = no;
    }

    else{

        NO_CRUZ *cursor_linha_no = cursor_linha->primeiro;

        while (cursor_linha_no->prox_linha != NULL) // testa o prox para parar em cima do último nó, e não depois dele (NULL)
        {

        cursor_linha_no = cursor_linha_no->prox_linha;

        }

        cursor_linha_no->prox_linha = no;

    }

    if (cursor_coluna->primeiro == NULL)
    {
        cursor_coluna->primeiro = no;
    }

    else{
        NO_CRUZ *cursor_coluna_no = cursor_coluna->primeiro;

        while (cursor_coluna_no->prox_coluna != NULL) // aqui anda pelo prox_coluna, não pelo prox_linha
        {

            cursor_coluna_no = cursor_coluna_no->prox_coluna;

        }
        
        cursor_coluna_no->prox_coluna = no;
    }

    lista->tamanho++;

    return true;



}

int lista_cruz_tamanho(LISTA_CRUZ *lista){
    if (lista == NULL)
    {
        return ERRO_LISTA_CRUZ;
    }

    return lista->tamanho;
    
}

// libera os nós de uma linha, andando só pelo prox_linha.
// cada nó também está numa coluna, então se andasse pelas colunas também daria free duas vezes no mesmo nó
static void apagar_nos(NO_CRUZ *no, bool apagar_itens){
    if (no == NULL)
    {
        return;
    }

    while (no != NULL)
    {
        
        NO_CRUZ *proximo_linha = no->prox_linha; // guardar antes do free, depois dele o nó não pode mais ser lido

        if (apagar_itens == true)
        {
            item_apagar(&no->item);
        }

        free(no);

        no = proximo_linha;
        
    }
    
    
}

// libera a cabeça recebida e todas as seguintes (rótulo + cabeça). não mexe nos nós
static void apagar_cabeca(CABECA *cabeca){
    if (cabeca == NULL)
    {
        return;
    }

    while (cabeca != NULL)
    {
        CABECA *prox = cabeca->prox;

        free(cabeca->rotulo);
        free(cabeca);

        cabeca = prox;
        
    }
    
    
}

void lista_cruz_apagar(LISTA_CRUZ **lista, bool apagar_itens){
    if (lista ==  NULL || *lista == NULL)
    {
        return;
    }

    // primeiro os nós, linha por linha (precisa das cabeças de linha ainda vivas para chegar neles)
    CABECA *atual_linha = (*lista)->inicio_linhas;

    while (atual_linha != NULL)
    {
        CABECA *proximo_linha = atual_linha->prox;

        apagar_nos(atual_linha->primeiro, apagar_itens);

        atual_linha = proximo_linha;

    }

   // depois as cabeças. as de coluna só apontam para nós que já foram liberados, então não precisa passar por eles
   apagar_cabeca((*lista)->inicio_linhas);
   apagar_cabeca((*lista)->inicio_colunas);

    free(*lista);
    *lista = NULL; // por isso recebe **: quem chamou fica com NULL e não com um ponteiro solto
}

ITEM *lista_cruz_primeiro(LISTA_CRUZ *lista){
    if (lista == NULL)
    {
        return NULL;
    }

    lista->atual_cabeca = lista->inicio_linhas; // sempre recomeça do início

    // pula linhas vazias (só acontece se o inserir falhou depois de criar a cabeça)
    while (lista->atual_cabeca != NULL && lista->atual_cabeca->primeiro == NULL)
    {
        lista->atual_cabeca = lista->atual_cabeca->prox;

    }

    if (lista->atual_cabeca == NULL)
    {
        lista->atual = NULL;
        return NULL;
    }
    else{
        lista->atual = lista->atual_cabeca->primeiro;

        return lista->atual->item;
    }
    
    


    
}

ITEM *lista_cruz_proximo(LISTA_CRUZ *lista){
    if (lista == NULL || lista->atual == NULL) // atual NULL = já acabou ou ninguém chamou o primeiro
    {
        return NULL;
    }

    // anda um nó na mesma linha
    lista->atual = lista->atual->prox_linha;

    if (lista->atual != NULL)
    {
        return lista->atual->item;
    }


    // a linha acabou: vai para a próxima cabeça (não para o início) e faz igual ao primeiro
    lista->atual_cabeca = lista->atual_cabeca->prox;

    while (lista->atual_cabeca != NULL && lista->atual_cabeca->primeiro == NULL)
    {
        lista->atual_cabeca = lista->atual_cabeca->prox;
    }
    if (lista->atual_cabeca == NULL)
    {
        lista->atual = NULL;
        return NULL;
    }
    else{
        lista->atual = lista->atual_cabeca->primeiro;

        return lista->atual->item;
    }
    
    
    
    


    
}


