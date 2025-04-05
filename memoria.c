#include "sistema.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Inicia um bloco com valores zerados
void inicializaBloco(BlocoMemoria *bloco){
    bloco->atualizado = false;
    bloco->custo = 0;
    bloco->cacheHit = 0;
    bloco->clock = 0;
}

// Aloca memória para a RAM com os dados zerados
void criarRamVazia(Mem *ram, int tamanho){
    // Aloca um vetor para memória de tamanho n
    ram->memoria = (BlocoMemoria*)malloc(tamanho * sizeof(BlocoMemoria));
    ram->tamanhoMem = tamanho;

    // Inicializa todos os endereços vazios
    for(int i = 0; i < tamanho; i++){
        ram->memoria[i].endBloco = i;
        inicializaBloco(&ram->memoria[i]);

        for(int j = 0; j < 4; j++)
            ram->memoria[i].palavras[j] = 0;
    }
}

// Aloca memória para a RAM com os dados aleatórios
void criarRamAleatoria(Mem *ram, int tamanho){
    srand(time(NULL));

    // Aloca um vetor para memória de tamanho n
    ram->memoria = (BlocoMemoria*)malloc(tamanho * sizeof(BlocoMemoria));
    ram->tamanhoMem = tamanho;

    // Inicializa todos os endereços aleatórios
    for(int i = 0; i < tamanho; i++){
        ram->memoria[i].endBloco = i;
        inicializaBloco(&ram->memoria[i]);

        for(int j = 0; j < 4; j++)
            ram->memoria[i].palavras[j] = rand() % 10000;
    }
}

// Desaloca memória da memória
void liberaRam(Mem *ram){
    free(ram->memoria);
}

// Retorna o conteúdo do endereço na RAM
BlocoMemoria getDadoRam(int endBloco, Mem *ram){
    return ram->memoria[endBloco];
}

// Salva o conteúdo no endereço na RAM
void setDadoRam(int endBloco, BlocoMemoria conteudo, Mem *ram){
    ram->memoria[endBloco] = conteudo;
}

// Imprime o conteúdo de um bloco (as 4 palavras)
void imprimirBloco(BlocoMemoria bloco){

    for(int i = 0; i < 4; i++)
        printf("%d ", bloco.palavras[i]);
}

// Imprime todo o conteúdo da RAM
void imprimirRam(Mem *ram){

    printf("\nConteudo da RAM");
    for(int i = 0; i < ram->tamanhoMem; i++){
        imprimirBloco(ram->memoria[i]);
        printf("\n");
    }
}

// Lê as n instruções do arquivo gerado e salva na memória de Instruções
int leInstrucoes(char *nome, int tamanho, Instrucao *memInstrucoes){
    FILE *arquivo = fopen(nome, "r");
    if(arquivo == NULL){
        printf("\nNÃO FOI POSSÍVEL ABRIR O ARQUIVO!\n\n");
        return 0;
    }

    for(int i = 0; i < tamanho-1; i++){
        fscanf(arquivo, "%d:", &memInstrucoes[i].opcode);
        fscanf(arquivo, "%d:%d:", &memInstrucoes[i].end1.endBloco, &memInstrucoes[i].end1.endPalavra);
        fscanf(arquivo, "%d:%d:", &memInstrucoes[i].end2.endBloco, &memInstrucoes[i].end2.endPalavra);
        fscanf(arquivo, "%d:%d", &memInstrucoes[i].end3.endBloco, &memInstrucoes[i].end3.endPalavra);
    }

    // Comando de parada do programa
    memInstrucoes[tamanho-1].opcode = -1;

    fclose(arquivo);
    return 1;
}

// ==================================================================================================
// GERA AS INSTRUÇÕES PARA O PROGRAMA DE TESTE

#define N_FOR 5     //numero de instrucoes que vao se repetir
#define N_MEM 1000  //numero da memoria para endereco do bloco
#define N_OPCODE 3  //numero maximo de opcodes
#define N_WORD 4    //numero de palavras no bloco

void instGenerator(int qtdInst, int prob, int tipo) {
    int rep[5][7];
    int inst[10000][7];  
    FILE *arquivo;

    if(tipo == 1)
        arquivo = fopen("instructions.txt", "w");
    else if(tipo == 2)
        arquivo = fopen("interrupcao.txt", "w");

    int random;
    int instruc[8] = {6, 6, 6, 6, 6, 6, 6, 6};
    srand(time(NULL));

    for (int i = 0; i < N_FOR; i++) {
        if(tipo == 1)
            random = rand() % (N_OPCODE - 1);
        else if(tipo == 2)
          random = rand() % (N_OPCODE - 1);  

        rep[i][0] = random;
        for (int j = 1; j <= (instruc[random]); j += 2) {
            rep[i][j] = rand() % N_MEM;
            rep[i][j + 1] = rand() % N_WORD;
        }
    }

    for (int i = 0; i < qtdInst;) {
        random = (rand() % 100) + 1;
        if (random <= prob && i + N_FOR < qtdInst) {
            for (int j = 0; j < N_FOR; j++) {
                for (int k = 0; k < 7; k++) {
                    inst[i + j][k] = rep[j][k];
                }
            }
            i += N_FOR;
        } else {
            if(tipo == 1)
            random = rand() % N_OPCODE;
            else if(tipo == 2)
            random = rand() % (N_OPCODE - 1); 
            inst[i][0] = random;
            for (int j = 1; j <= instruc[random]; j += 2) {
                inst[i][j] = rand() % N_MEM;
                inst[i][j + 1] = rand() % N_WORD;
            }
            i++;
        }
    }
    for (int i = 0; i < qtdInst; i++) {
        for (int k = 0; k < 6; k++) {
            fprintf(arquivo, "%d:", inst[i][k]);
        }
        fprintf(arquivo, "%d\n", inst[i][6]);
    }

    fclose(arquivo);
}


