#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISK_TAM 10000

int main(){
    // Cria ou sobrescreve um novo arquivo em modo binário
    FILE *arquivo = fopen("disco.dat", "wb");
    if(arquivo == NULL){
        perror("Não foi possível abrir o arquivo.");
        return 1; // Retorne um valor diferente de 0 em caso de erro
    }
    
    // Define a semente para gerar números aleatórios
    srand(time(NULL));
    int num;

    // Preenche o arquivo com valores aleatórios
    for(int i = 0; i < DISK_TAM * 4; i++){
        num = rand() % 10000;
        if(fwrite(&num, sizeof(int), 1, arquivo) != 1){
            perror("Erro ao escrever no arquivo.");
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}
