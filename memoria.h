 #ifndef MEMORIA_H
 #define MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct{
    int palavras[4];
    int endBloco;
    int custo;
    int cacheHit;
    bool atualizado;
    clock_t clock;
} BlocoMemoria;

typedef struct{
    BlocoMemoria *memoria;
    int tamanhoMem;
} Mem;

typedef struct{
    int endBloco;
    int endPalavra;
} Endereco;

typedef struct{
    int opcode;
    Endereco end1;
    Endereco end2;
    Endereco end3;
} Instrucao;

// Inicializa a RAM
void inicializaBloco(BlocoMemoria *bloco);
void criarRamVazia(Mem *ram, int tamanho);
void criarRamAleatoria(Mem *ram, int tamanho);
void liberaRam(Mem *ram);

// Busca ou salva dados na RAM
BlocoMemoria getDadoRam(int endBloco, Mem *ram);
void setDadoRam(int endBloco, BlocoMemoria conteudo, Mem *ram);

// Faz a impressão dos dados da RAM
void imprimirBloco(BlocoMemoria bloco);
void imprimirRam(Mem *ram);

//Faz a leitura de um conjunto de instruções de um arquivo
void instGenerator(int qtdInst, int prob, int tipo);
int leInstrucoes(char *nome, int tamanho, Instrucao *memInstrucoes);

#endif