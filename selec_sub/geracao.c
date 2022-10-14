#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "cliente.c"

#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
    int congelado;
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


int selecao_substituicao(char *nome_arquivo_entrada, int tam_memoria) {
    //TODO: Implementar essa função
    FILE* arq;
    TVet* M[tam_memoria];

    TVet* c = NULL;
    c->cli = NULL;
    c->congelado = 0;

    int num_parts;
    int tudoCongelado;

    arq = fopen(nome_arquivo_entrada, "r");
    if (arq != NULL){
        int i = 0;

        c->cli = le_cliente(arq);
        c->congelado = 0;
        while (i < tam_memoria && c != NULL) //Carrega todos os registros que consegue na memória.
        {
            M[i] = c;

            c->cli = le_cliente(arq);
            i = i + 1; //Avança o loop
        }

        num_parts = 0;
        while (c->cli != NULL)
        {
            //Abre partição
            FILE* saida;
            saida = fopen(gera_nome_particao(num_parts+1), "w");
            if (saida != NULL){
                num_parts += 1;
                //Descongela array

                for (int i = 0; i < tam_memoria; i++)
                {
                    M[i]->congelado = 0; //Descongela elementos                    
                }
                
                tudoCongelado = 0;

                while(tudoCongelado == 0){ //Enquanto não estiver tudo congelado
                    int menor = -1;
                    //Busca menor elemento da lista NÃO CONGELADO
                    for (int i = 0; i < tam_memoria; i++) //Percorre o M inteiro
                    {
                        if(M[i]->congelado == 0 && M[i]->cli != NULL){ //Pula Congelado ou Vazio
                            if(menor == -1) menor = i; //Se for o primeiro valor checado
                            else{ //Se já houver algum valor menor
                                if(M[i]->cli->cod_cliente < M[menor]->cli->cod_cliente){
                                    menor = i;
                                }
                            }
                        }
                    }
                    //Salvo elemento na partição
                    salva_cliente(M[menor]->cli, saida);
                    //Le proximo do arquivo
                    if (c->cli != NULL)
                    {
                        c->cli = le_cliente(arq);
                        c->congelado = 0;

                        //Comparo se precisa congelar
                        if(c->cli->cod_cliente < M[menor]->cli->cod_cliente){ 
                            c->congelado = 1;

                            tudoCongelado = 1;
                            for (int i = 0; i < tam_memoria; i++)
                            {
                                if (M[i]->congelado == 0 && M[i]->cli != NULL)
                                {
                                    tudoCongelado = 0;
                                    break;
                                }   
                            }
                        } 
                    }
                    else{
                        tudoCongelado = 1;
                        for (int i = 0; i < tam_memoria; i++)
                        {
                            if (M[i]->congelado == 0 && M[i]->cli != NULL)
                            {
                                tudoCongelado = 0;
                                break;
                            }
                        }
                    }
                    
                    //Substituo no array ou definindo como nulo se tiver acabado
                    M[menor] = c; 
                }

                //Tudo estava congelado
                fclose(saida);
            }
        }

        //Pode ter algo congelado ainda mas o arquivo já acabou
        for (int i = 0; i < tam_memoria; i++)
            {
                M[i]->congelado = 0; //Descongela elementos                    
            }
        
        tudoCongelado = 1;
            for (int i = 0; i < tam_memoria; i++)
            {
                if (M[i]->congelado == 0 && M[i]->cli != NULL)
                {
                    tudoCongelado = 0;
                    break;
                }
            }

        if(tudoCongelado != 1){

            FILE* saida;
            saida = fopen(gera_nome_particao(num_parts+1), "w");
            num_parts += 1;
            if(saida != NULL){
                while (tudoCongelado != 1)
                {
                    int menor = -1;
                    //Busca menor elemento da lista NÃO CONGELADO
                    for (int i = 0; i < tam_memoria; i++) //Percorre o M inteiro
                    {
                        if(M[i]->cli != NULL){ //Pula Congelado ou Vazio
                            if(menor == -1) menor = i; //Se for o primeiro valor checado
                            else{ //Se já houver algum valor menor
                                if(M[i]->cli->cod_cliente < M[menor]->cli->cod_cliente){
                                    menor = i;
                                }
                            }
                        }
                    }

                    salva_cliente(M[menor]->cli, saida);
                    M[menor]->cli = NULL;

                    tudoCongelado = 1;
                    for (int i = 0; i < tam_memoria; i++)
                    {
                        if (M[i]->congelado == 0 && M[i]->cli != NULL)
                        {
                            tudoCongelado = 0;
                            break;
                        }
                    }
                }
            
                fclose(saida);
            }
        }
        fclose(arq);
    }

    return num_parts;
}

int main() {
    int tam_memoria;
    scanf("%d", &tam_memoria);
    int num_part = selecao_substituicao("entrada.txt", tam_memoria);
    for (int i = 1; i < num_part+1; i++) {
        printf("*** %s\n", gera_nome_particao(i));
        imprime_arquivo(gera_nome_particao(i));
    }
}