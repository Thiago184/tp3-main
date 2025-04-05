#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>



// Variáveis globais de Hit e Miss de todas as caches e custo
int custoT = 0, 
hitC1 = 0, missC1 = 0, 
hitC2 = 0, missC2 = 0,
hitC3 = 0, missC3 = 0,
hitRam = 0, missRam = 0,
hitDisk = 0, missDisk = 0;

// Aloca a memória para a cache e inicializa seus blocos vazios
void criarCache(Mem *cache, int tamanho){
    cache->memoria = (BlocoMemoria*)malloc(tamanho * sizeof(BlocoMemoria));
    cache->tamanhoMem = tamanho;

    for(int i = 0; i < tamanho; i++){
        inicializaBloco(&cache->memoria[i]);
        cache->memoria[i].endBloco = -1;
    }
}

// Atualiza os valores de hit e miss de acordo com o acesso dos registradores
void atualizaHitMiss(BlocoMemoria *reg){
    switch(reg->cacheHit){
        case 1:{        // Encontrado na cache1
            hitC1++;
            break;
        }
        case 2:{        // Encontrado na cache2
            missC1++;
            hitC2++;
            break;
        }   
        case 3:{        // Encontrado na cache3
            missC1++;
            missC2++;
            hitC3++;
            break;
        }
        case 4:{        // Encontrado na RAM
            missC1++;
            missC2++;
            missC3++;
            hitRam++;
            break;
        }
        case 5:{        // Encontrado no disco
            missC1++;
            missC2++;
            missC3++;
            missRam++;
            hitDisk++;
            break;
        }
    }
}

void maquina(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Instrucao *memInstrucao){
    // Registradores
    BlocoMemoria reg1, reg2, reg3;

    int opcode = 0;
    int PC = 0;
    Instrucao instrucao;

    // Inicia a pipeline de execução das intruções, enquanto não encontrar o HALT
    while(opcode != -1){
        instrucao = memInstrucao[PC];
        opcode = instrucao.opcode;

        if(opcode != -1){
            reg1 = buscarNasMemorias(instrucao.end1, disco, ram, cache1, cache2, cache3);
            reg2 = buscarNasMemorias(instrucao.end2, disco, ram, cache1, cache2, cache3);
            reg3 = buscarNasMemorias(instrucao.end3, disco, ram, cache1, cache2, cache3);

            // Registra a cache hit ou miss do acesso
            atualizaHitMiss(&reg1);
            atualizaHitMiss(&reg2);
            atualizaHitMiss(&reg3);
    
            /* Executa a instrução
            -1 -> HALT
            0  -> SOMA
            1  -> SUBTRAÇÃO */
            switch(opcode){
                // HALT
                case -1: {
                    printf("\nFIM DO PROGRAMA!\n\n");
                    imprimirRam(ram);
                    break;
                }
                // SOMA
                case 0: {
                    // Soma o conteúdo do end1 com end2 e armazena no end3
                    reg3.palavras[instrucao.end3.endPalavra] = reg1.palavras[instrucao.end1.endPalavra] + reg2.palavras[instrucao.end2.endPalavra];
                    reg3.atualizado = true;

                    // Custo de acesso total de todos registradores
                    custoT += reg1.custo + reg2.custo + reg3.custo;

                    // Infos da CPU até o momento
                    printf("\n%d | OPCODE: %d", PC + 1, instrucao.opcode); 
                    printf("\nCusto ate o momento: %d", custoT);

                    printf("\nHIT C1  : %d | MISS C1  : %d", hitC1, missC1);
                    printf("\nHIT C2  : %d | MISS C2  : %d", hitC2, missC2);
                    printf("\nHIT C3  : %d | MISS C3  : %d", hitC3, missC3);
                    printf("\nHIT RAM : %d | MISS RAM : %d", hitRam, missRam);
                    printf("\nHIT DISK: %d | MISS DISK: %d", hitDisk, missDisk);
                    printf("\n\n");

                    break;
                }

                // SUBTRAÇÃO
                case 1: {
                    // Subtrai o conteúdo do end1 com end2 e armazena no end3
                    reg3.palavras[instrucao.end3.endPalavra] = reg1.palavras[instrucao.end1.endPalavra] - reg2.palavras[instrucao.end2.endPalavra];
                    reg3.atualizado = true;

                    // Custo de acesso total de todos registradores
                    custoT += reg1.custo + reg2.custo + reg3.custo;

                    // Infos da CPU até o momento
                    printf("%d | OPCODE: %d", PC + 1, instrucao.opcode); 
                    printf("\nCusto ate o momento: %d", custoT);

                    printf("\nHIT C1  : %d | MISS C1  : %d", hitC1, missC1);
                    printf("\nHIT C2  : %d | MISS C2  : %d", hitC2, missC2);
                    printf("\nHIT C3  : %d | MISS C3  : %d", hitC3, missC3);
                    printf("\nHIT RAM : %d | MISS RAM : %d", hitRam, missRam);
                    printf("\nHIT DISK: %d | MISS DISK: %d", hitDisk, missDisk);
                    printf("\n\n");

                    break;
                }

                // INTERRUPÇÃO
                case 2: {
                    printf("INTERRUPÇÃO IDENTIFICADA\n");

                    // Salva os dados atuais (hits, misses, custo) e zera todas as variaveis
                    salvarDados();

                    // Executa as instruções de Interrupção até o final
                    tratadorDeInstrucao(disco, ram, cache1, cache2, cache3);

                    // Finalizada a interrupção, retoma os dados de onde havia parado e continua
                    guardarDados();

                    break;
                }
            }

            PC++;   
        }
    }
}

BlocoMemoria buscarNasMemorias(Endereco end, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3){
    int custo = 0;

    //  CACHE1
    // Percorre toda a cache1 em busca do bloco correto
    for(int i = 0; i < cache1->tamanhoMem; i++){
        // Se encontrar, salva o custo, o clock e o hit
        if(cache1->memoria[i].endBloco == end.endBloco){
            cache1->memoria[i].custo = CUSTO_C1;
            cache1->memoria[i].clock = clock();

            // Indica onde foi encontrado
            cache1->memoria[i].cacheHit = 1;
            return cache1->memoria[i];
        }
    }

    // CACHE2
    for(int i = 0; i < cache2->tamanhoMem; i++){
        // Se encontrar, salva o custo, o clock e o hit
        if(cache2->memoria[i].endBloco == end.endBloco){
            cache2->memoria[i].custo = CUSTO_C2;
            cache2->memoria[i].clock = clock();

            // Indica onde foi encontrado
            cache2->memoria[i].cacheHit = 2;

            // Encontra o mais velho da cache1 e realiza a troca
            int posMaisVelho = verificaMaisVelho(cache1);
            return movCache2toCache1(posMaisVelho, i, ram, cache1, cache2, custo);
        }
    }

    // CACHE3
    for(int i = 0; i < cache3->tamanhoMem; i++){
        // Se encontrar, salva o custo, o clock e o hit
        if(cache3->memoria[i].endBloco == end.endBloco){
            cache3->memoria[i].custo = CUSTO_C3;
            cache3->memoria[i].clock = clock();
            cache3->memoria[i].cacheHit = 3;

            // Indica onde foi encontrado
            int posMaisVelho = verificaMaisVelho(cache2);
            return movCache3toCache2(posMaisVelho, i, ram, cache1, cache2, cache3, custo);
        }
    }

    // BUSCA NA RAM
    for(int i = 0; i < ram->tamanhoMem; i++){
        // Se encontrar, salva o custo, o clock e o hit
        if(ram->memoria[i].endBloco == end.endBloco){
            ram->memoria[i].custo = CUSTO_RAM;
            ram->memoria[i].clock = clock();
            ram->memoria[i].cacheHit = 4;

            int posMaisVelho = verificaMaisVelho(cache3);
            return movRamtoCache3(posMaisVelho, end.endBloco, ram, cache1, cache2, cache3, custo, end);
        }
    }

    // SE NÃO ENCONTRAR EM NENHUMA MEMÓRIA, BUSCA NO DISCO
    custo = CUSTO_DISK;

    int blocoMaisVelho = verificaMaisVelho(ram);
    return movDisktoRam(blocoMaisVelho, disco, ram, cache1, cache2, cache3, end, custo);
}

BlocoMemoria movCache2toCache1(int posicaoCache1, int posicaoCache2, Mem *ram, Mem *cache1, Mem *cache2, int custo){

    BlocoMemoria aux = cache1->memoria[posicaoCache1];

    // Passa o endereço para a cache1 e faz a troca com a cache2
    cache1->memoria[posicaoCache1] = cache2->memoria[posicaoCache2];
    cache2->memoria[posicaoCache2] = aux;

    // Atualiza com o custo de acordo com a cachehit
    cache1->memoria[posicaoCache1].custo = custo;

    // Retorna o bloco
    return cache1->memoria[posicaoCache1];
}

BlocoMemoria movCache3toCache2(int posicaoCache2, int posicaoCache3, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo){
    BlocoMemoria aux = cache2->memoria[posicaoCache2];

    // Passa o endereço para a cache2 e faz a troca com a cache3
    cache2->memoria[posicaoCache2] = cache3->memoria[posicaoCache3];
    cache3->memoria[posicaoCache3] = aux;

    // Atualiza com o custo de acordo com a cachehit
    cache2->memoria[posicaoCache2].custo = custo;

    // Verifica qual deve ser o bloco da cache1 que será realizada a troca
    int posicaoCache1 = verificaMaisVelho(cache1);
    // Retorna o bloco
    return movCache2toCache1(posicaoCache1, posicaoCache2, ram, cache1, cache2, custo);
}

BlocoMemoria movRamtoCache3(int posicaoCache3, int posicaoRam, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, int custo, Endereco end){
    BlocoMemoria aux = cache3->memoria[posicaoCache3];

    // Passa o endereço para a cache2 e faz a troca com a cache3
    cache3->memoria[posicaoCache3] = ram->memoria[posicaoRam];
    ram->memoria[posicaoRam] = aux;

    // Atualiza com o custo de acordo com a cachehit
    cache3->memoria[posicaoCache3].custo = custo;

    // Verifica qual deve ser o bloco da cache1 que será realizada a troca
    int posicaoCache2 = verificaMaisVelho(cache2);
    // Retorna o bloco
    return movCache3toCache2(posicaoCache2, posicaoCache3, ram, cache1, cache2, cache3, custo);
}

BlocoMemoria movDisktoRam(int blocoMaisVelho, FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3, Endereco end, int custo){
    // Se tiver sido atualizado, sobreescreve no disco
    if(ram->memoria[blocoMaisVelho].atualizado)
        salvaBlocoDisco(disco, &ram->memoria[blocoMaisVelho], end);

    // Le o bloco no disco e salva na memória RAM
    leBlocoDisco(disco, &ram->memoria[blocoMaisVelho], end);
    ram->memoria[blocoMaisVelho].custo = custo;
    ram->memoria[blocoMaisVelho].clock = 0;
    ram->memoria[blocoMaisVelho].cacheHit = 5;
    ram->memoria[blocoMaisVelho].endBloco = end.endBloco;

    int posicaoCache3 = verificaMaisVelho(cache3);

    return movRamtoCache3(posicaoCache3, blocoMaisVelho, ram, cache1, cache2, cache3, custo, end);
}

// Verifica qual o bloco mais velho de uma cache para substituição
int verificaMaisVelho(Mem *cache){
    int posMaisVelho = 0;

    // Verifica qual o maior ou se há algum bloco vazio
    for(int i = 0; i < cache->tamanhoMem; i++)
        if(cache->memoria[i].clock < cache->memoria[posMaisVelho].clock ||  cache->memoria[i].endBloco == -1)
            posMaisVelho = i;

    return posMaisVelho;
}

// Salva os dados de hit, miss e custo num arquivo texto para recuperar mais tarde
void salvarDados(){
    // Cria um novo arquivo para armazenarmos os dados
    FILE *dados = fopen("dados.txt", "w");
    
    fprintf(dados, "%d %d %d %d %d\n", hitC1, hitC2, hitC3, hitRam, hitDisk);
    fprintf(dados, "%d %d %d %d %d\n", missC1, missC2, missC3, missRam, missDisk);
    fprintf(dados, "%d\n", custoT);

    // Zera as variáveis globais
    custoT = 0, 
    hitC1 = 0, missC1 = 0, 
    hitC2 = 0, missC2 = 0,
    hitC3 = 0, missC3 = 0,
    hitRam = 0, missRam = 0,
    hitDisk = 0, missDisk = 0;

    fclose(dados);
}

// Recupera os dados salvos antes da interrupção nas variáveis globais
void guardarDados(){
    FILE *dados = fopen("dados.txt", "r");

    int c;

    fscanf(dados, "%d %d %d %d %d", &hitC1, &hitC2, &hitC3, &hitRam, &hitDisk);
    fscanf(dados, "%d %d %d %d %d", &missC1, &missC2, &missC3, &missRam, &missDisk);
    fscanf(dados, "%d", &c);

    custoT += c;

    fclose(dados);
}

// Ao encontrar o OPCODE 2, para a execução das instruções atuais e executa a interrupção
void tratadorDeInstrucao(FILE *disco, Mem *ram, Mem *cache1, Mem *cache2, Mem *cache3){
    // Gera instruções de interrupção 
    instGenerator(100, 50, 2);

    // Cria uma memória de instruções de interrupções
    Instrucao instrucoesInterrupcao[100];

    // Chama a função de máquina e faz a execução completa da interrupçao
    printf("INICIO - TRATADOR DE INTERRUPÇÃO\n");
    if(leInstrucoes("interrupcao.txt", 20, instrucoesInterrupcao)){
        printf("\033[31m");
        maquina(disco, ram, cache1, cache2, cache3, instrucoesInterrupcao);
    }

    printf("\033[0m");

    printf("FIM - TRATADOR DE INTERRUPÇÃO\n");

}