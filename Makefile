CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
LDLIBS = -lm

OBJS = main.o item.o csv.o schema.o nasa.o query.o estrutura.o \
       pilha.o fila.o lista_seq.o lista_enc.o lista_cabeca.o lista_ord.o lista_gen.o lista_cruz.o

all: trabalho

trabalho: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o trabalho $(LDLIBS)

main.o: src/main.c src/item.h src/schema.h src/nasa.h src/query.h src/estrutura.h
	$(CC) $(CFLAGS) -c src/main.c
item.o: src/item.c src/item.h
	$(CC) $(CFLAGS) -c src/item.c
csv.o: src/csv.c src/csv.h
	$(CC) $(CFLAGS) -c src/csv.c
schema.o: src/schema.c src/schema.h src/csv.h
	$(CC) $(CFLAGS) -c src/schema.c
nasa.o: src/nasa.c src/nasa.h src/csv.h src/schema.h src/item.h
	$(CC) $(CFLAGS) -c src/nasa.c
query.o: src/query.c src/query.h src/schema.h src/item.h src/csv.h
	$(CC) $(CFLAGS) -c src/query.c
estrutura.o: src/estrutura.c src/estrutura.h src/item.h src/query.h src/schema.h src/pilha.h src/fila.h src/lista_seq.h src/lista_enc.h src/lista_cabeca.h src/lista_ord.h src/lista_gen.h
	$(CC) $(CFLAGS) -c src/estrutura.c
pilha.o: src/pilha.c src/pilha.h src/item.h
	$(CC) $(CFLAGS) -c src/pilha.c
fila.o: src/fila.c src/fila.h src/item.h
	$(CC) $(CFLAGS) -c src/fila.c
lista_seq.o: src/lista_seq.c src/lista_seq.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_seq.c
lista_enc.o: src/lista_enc.c src/lista_enc.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_enc.c
lista_cabeca.o: src/lista_cabeca.c src/lista_cabeca.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_cabeca.c
lista_ord.o: src/lista_ord.c src/lista_ord.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_ord.c
lista_gen.o: src/lista_gen.c src/lista_gen.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_gen.c
lista_cruz.o: src/lista_cruz.c src/lista_cruz.h src/item.h
	$(CC) $(CFLAGS) -c src/lista_cruz.c

clean:
	rm -f *.o trabalho

.PHONY: all clean
