#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define CUSTO_C1 50
#define CUSTO_C2 100
#define CUSTO_C3 200
#define CUSTO_RAM 400
#define CUSTO_DISK 800
#define C1_TAM 16
#define C2_TAM 32  
#define C3_TAM 64
#define RAM_TAM  100

#define QTD_INST 1000
#define PROB     50


// Estruturas de dados
typedef struct {
    int palavras[4];
    int endBloco;
    int custo;
    int cacheHit;
    bool atualizado;
    clock_t clock;
} BlocoMemoria;

typedef struct {
    BlocoMemoria *memoria;
    int tamanhoMem;
} Mem;

typedef struct {
    int endBloco;
    int endPalavra;
} Endereco;

typedef struct {
    int opcode;
    Endereco end1;
    Endereco end2;
    Endereco end3;
} Instrucao;

// Funções relacionadas à memória
void inicializaBloco(BlocoMemoria *bloco);
void criarRamVazia(Mem *ram, int tamanho);
void criarRamAleatoria(Mem *ram, int tamanho);
void liberaRam(Mem *ram);
BlocoMemoria getDadoRam(int endBloco, Mem *ram);
void setDadoRam(int endBloco, BlocoMemoria conteudo, Mem *ram);
void imprimirBloco(BlocoMemoria bloco);
void imprimirRam(Mem *ram);
void instGenerator(int qtdInst, int prob, int tipo);
int leInstrucoes(char *nome, int tamanho, Instrucao *memInstrucoes);

// Funções relacionadas ao HD
int posNoArquivo(FILE *arquivo, Endereco end);
void leBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end);
void salvaBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end);

// Funções relacionadas à CPU
void criarCache(Mem *cache, int tamanho);
void maquina(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Instrucao *memInstrucao);
BlocoMemoria buscarNasMemorias(Endereco end, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3);
BlocoMemoria movCache2toCache1(int posicaoCache1, int posicaoCache2, Mem *ram, Mem *cache1, Mem *cache2, int custo);
BlocoMemoria movCache3toCache2(int posicaoCache2, int posicaoCache3, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo);
BlocoMemoria movRamtoCache3(int posicaoCache3, int posicaoRam, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo, Endereco end);
BlocoMemoria movDisktoRam(int blocoMaisVelho, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Endereco end, int custo);
void salvarDados();
void guardarDados();
void tratadorDeInstrucao(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3);
int verificaMaisVelho(Mem *cache);

#endif