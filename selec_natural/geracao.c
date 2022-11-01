#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "cliente.c"

#include <limits.h>
#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
} TVet;

void imprime_arquivo(char *nome) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(nome, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets(linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets(linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

char *gera_nome_particao(int num_part) {
    char *nome_particao = (char *) malloc(sizeof(char[10]));
    char num_particao[10];

    strcpy(nome_particao, "p");
    sprintf(num_particao, "%d", num_part);
    strcat(nome_particao, num_particao);
    strcat(nome_particao, ".txt");

    return nome_particao;
}

int findMenorChave(int tam_memoria, TCliente** array){
    int menor_chave = INT_MAX;
    int menor_pos = -1;
    for (int i = 0; i < tam_memoria; i++)
    {
        if(array[i] != NULL){
            if(array[i]->cod_cliente < menor_chave){
                menor_pos = i;
                menor_chave = array[i]->cod_cliente;
            }
        }
    }
    
    return menor_pos;
}
int selecao_natural(char *nome_arquivo_entrada, int tam_memoria) {
    //Variaveis locais
    FILE* entrada;
    FILE* part;
    FILE* reserv;
    TCliente* mem[tam_memoria];

    TCliente* cli;
    TCliente* wCli = NULL;
    int menorPos;

    int q_mem = 0;
    int q_reserv = 0;

    int num_parts = 0;

    //Implementação
    entrada = fopen(nome_arquivo_entrada, "r");
    if(entrada != NULL){ //Checa se conseguiu abrir o arquivo de entrada

        //Passo 1: Primeira leitura do arquivo
        for (int i = 0; i < tam_memoria; i++)
        {
            cli = le_cliente(entrada);
            mem[i] = cli;
            if(cli != NULL) q_mem += 1;
        }
    
        //Gera partição de saída
        while(q_mem > 0){ // Arquivo não acabou
            num_parts += 1;
            part = fopen(gera_nome_particao(num_parts), "w");
            if (part != NULL){ 
                reserv = fopen("reserv.txt", "w");
                q_reserv = 0;
                if(reserv != NULL)
                {
                    //Conseguiu gerar partição de saída
                    //Escreve o cliente da menor chave:
                    //Buscar nova chave no arquivo
                    //Somente se, há chave no arquivo e o reservatório não encheu
                        while (q_reserv < tam_memoria && q_mem > 0)
                        {
                            // for (int i = 0; i < tam_memoria; i++)
                            // {
                            //     printf("Mem: ");
                            //     if(mem[i] != NULL) printf("%i | ", mem[i]->cod_cliente);
                            // }
                            // printf("\n");

                            menorPos = findMenorChave(tam_memoria, mem);
                            wCli = mem[menorPos]; //Salva quem acabou de ser escrito na partição
                            salva_cliente(wCli, part);

                            cli = le_cliente(entrada);
                            mem[menorPos] = cli; //Substitui pelo ultimo lido do arquivo
                            if(cli == NULL) q_mem -= 1;

                            while(cli != NULL && cli->cod_cliente < wCli->cod_cliente && q_reserv < tam_memoria)
                            {
                                //Ultimo numero lido é menor que o escrito
                                //Grava no reservatorio e le o próximo
                                salva_cliente(cli, reserv);
                                q_reserv += 1;
                                mem[menorPos] = NULL;
                                if(q_reserv < tam_memoria)
                                {
                                    cli = le_cliente(entrada);
                                    mem[menorPos] = cli;
                                }
                                if(mem[menorPos] == NULL) q_mem -= 1;
                            }
                        }

                        while (q_mem > 0)
                        {
                            menorPos = findMenorChave(tam_memoria, mem);
                            wCli = mem[menorPos]; //Salva quem acabou de ser escrito na partição
                            salva_cliente(wCli, part);

                            mem[menorPos] = NULL;
                            q_mem -= 1;
                        }
                        

                    fclose(reserv);

                    //Copia reservatorio pra memoria
                    q_mem = 0;
                    reserv = fopen("reserv.txt", "r");
                    if (reserv != NULL){
                        for (int i = 0; i < tam_memoria; i++)
                        {
                            cli = le_cliente(reserv);
                            mem[i] = cli;
                            if(cli != NULL) q_mem += 1;
                        }
                    }


                    fclose(reserv);
                }
                
                fclose(part);
            }
        }
    }

    fclose(entrada);
    return num_parts;
}


int main() {
    int tam_memoria;
    scanf("%d", &tam_memoria);
    int num_part = selecao_natural("entrada.txt", tam_memoria);
    for (int i = 1; i < num_part+1; i++) {
        printf("*** %s\n", gera_nome_particao(i));
        imprime_arquivo(gera_nome_particao(i));
    }
}