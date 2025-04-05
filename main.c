#include "sistema.h"

int main(int argc, char *argv[]){
    // Memórias
    Mem ram, cache1, cache2, cache3;

    // Memória externa
    FILE *disco = fopen("disco.dat", "r+b");
    if(disco == NULL){
        perror("NÃO FOI POSSÍVEL ABRIR O ARQUIVO.");
        exit(1);
    }

    // Instrucoes
    Instrucao memInstrucoes[QTD_INST];

    // Inicia as memórias
    criarCache(&ram, RAM_TAM);
    criarCache(&cache1, C1_TAM);
    criarCache(&cache2, C2_TAM);
    criarCache(&cache3, C3_TAM);
  
    // Gera o arquivo de instruções
    instGenerator(QTD_INST, PROB, 1);

    // Execução do programa
    printf("\n\n===MÁQUINA SIMULADA - ORGANIZAÇÃO DE COMPUTADORES===\n\n");

    // Lê as instruções de teste a partir de arquivo
    if(leInstrucoes("instructions.txt", QTD_INST, memInstrucoes))
        // Executa todas as instruções e nos dá as estatísticas
        maquina(disco, &ram, &cache1, &cache2, &cache3, memInstrucoes);

    // Libera as memórias
    fclose(disco);
    liberaRam(&ram);
    liberaRam(&cache1);
    liberaRam(&cache2);
    liberaRam(&cache3);

    return 0;
}
