# Projeto 1 — Busca e Organização de Dados

Trabalho prático de SCC0202 (Algoritmos e Estruturas de Dados I), ICMC-USP.

Programa em C que lê um arquivo CSV, guarda os registros numa estrutura de dados escolhida pelo usuário, faz uma busca a partir de um arquivo de consulta e guarda os registros encontrados numa segunda estrutura, também escolhida pelo usuário. Os resultados são exportados num CSV com o mesmo formato da entrada.

O projeto tem duas partes: na primeira, os dados são registros de aproximação de asteroides; na segunda, um dataset público do dados.gov.br. As estruturas são as mesmas nas duas partes. Só muda o leitor que transforma cada linha do CSV num registro.

## Compilação

```
make
```

Gera o executável `trabalho`. O código compila com `-Wall -Wextra -pedantic`.

## Execução

```
./trabalho --reader nasa --input dados/asteroids.csv \
           --base lista_enc --result lista_ord \
           --query consultas/q1.txt --output resultado.csv \
           --top-k 100 --limit 10000
```

| Argumento  | Descrição |
|------------|-----------|
| `--reader` | leitor do dataset (`nasa` na Parte 1) |
| `--input`  | CSV de entrada |
| `--base`   | estrutura que guarda o dataset completo |
| `--result` | estrutura que guarda os resultados da busca |
| `--query`  | arquivo com os critérios da busca |
| `--output` | CSV de saída |
| `--top-k`  | máximo de registros exportados (0 = todos) |
| `--limit`  | máximo de registros carregados (0 = todos) |

Estruturas disponíveis para `--base` e `--result`: `pilha`, `fila`, `lista_seq`, `lista_enc`, `lista_cabeca`, `lista_ord`, `lista_gen`, `lista_cruz`.

## Formato da consulta

Um comando por linha, com campos separados por `|`:

```
FILTER|distance_au|<=|0.08
SIM|velocity_km_s|15.0|10.0|1.0
ACCEPT|0.60
ORDER|similarity|DESC
GROUP|threat_category|year
CROSS|threat_category|year
```

- `FILTER`: condição que o registro precisa cumprir.
- `SIM`: critério de similaridade numérica (`campo|alvo|tolerância|peso`).
- `ACCEPT`: similaridade mínima para aceitar o registro.
- `ORDER`: chave de ordenação da `lista_ord`.
- `GROUP`: campos de agrupamento da `lista_gen`.
- `CROSS`: campos das duas organizações da `lista_cruz`.

## Organização

```
src/     código-fonte
tests/   testes dos módulos
```

## Grupo

Bernardo Rubio, Samuel Gurgel, CAio andrade