#ifndef HD_H
#define HD_H

#include "memoria.h"

#include <stdio.h>
#include <stdlib.h>

int posNoArquivo(FILE *arquivo, Endereco end);
void leBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end);
void salvaBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end);

#endif