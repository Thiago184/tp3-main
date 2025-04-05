#ifndef CPU_H
#define CPU_H

#include "memoria.h"
#include "hd.h"

// Inicializa as caches
void criarCache(Mem *cache, int tamanho);

// Realiza a execucão de todas instruções
void maquina(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Instrucao *memInstrucao);

// Realiza a busca e movimentações dos blocos entre as caches e a memória
BlocoMemoria buscarNasMemorias(Endereco end, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3);
BlocoMemoria movCache2toCache1(int posicaoCache1, int posicaoCache2, Mem *ram, Mem *cache1, Mem *cache2, int custo);
BlocoMemoria movCache3toCache2(int posicaoCache2, int posicaoCache3, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo);
BlocoMemoria movRamtoCache3(int posicaoCache3, int posicaoRam, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo, Endereco end);
BlocoMemoria movDisktoRam(int blocoMaisVelho, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Endereco end, int custo);

// Realiza as operações de interrupção
void salvarDados();
void guardarDados();
void tratadorDeInstrucao(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3);

int verificaMaisVelho(Mem *cache);

#endif