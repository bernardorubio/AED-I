#ifndef CSV_H
#define CSV_H

char **csv_separar_em_strings(char *linha, int *qtd_campos);
void csv_liberar_memoria_separada(char **campos, int qtd_campos);

// lembrar de tirar o \r\n
// func aloca e quem chamou chama liberar
// se erro qtd campos -1;
#endif