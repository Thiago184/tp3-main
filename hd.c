#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>

// Identifica a posição de onde o bloco que buscamos está
int posNoArquivo(FILE *arquivo, Endereco end){
    int posBloco = (end.endBloco) * (sizeof(int) * 4);       // Obtem o número total de bytes onde o bloco está
    int posArq = ftell(arquivo);

    // Se posição atual estiver depois do bloco, retorna n bytes
    if(posBloco < posArq)
        posArq = posBloco - posArq;
    // Se posição atual estiver antes do bloco, avança n bytes"
    else if(posBloco > posArq)
        posArq = posBloco - posArq;

    return posArq;
}

// Le o nosso arquivo binário, encontra o bloco desejado e salva no bloco de memória passado
void leBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end){
    int palavras[4];
    
    int posArq = posNoArquivo(arquivo, end);
    
    // Vai até a posição correta do bloco
    fseek(arquivo, posArq, SEEK_CUR);
    
    // Le as 4 palavras do bloco
    fread(palavras, sizeof(int), 4, arquivo);

    // Atualiza o bloco com os dados do disco
    for(int i = 0; i < 4; i++)
        bloco->palavras[i] = palavras[i];
}

// Salva as palavras de um bloco no arquivo binário
void salvaBlocoDisco(FILE *arquivo, BlocoMemoria *bloco, Endereco end){
    int posArq = posNoArquivo(arquivo, end);
    
    // Vai até a posição correta do bloco
    fseek(arquivo, posArq, SEEK_CUR);

    // Salva as palavras no bloco
    fwrite(bloco->palavras, sizeof(int), 4, arquivo);
}