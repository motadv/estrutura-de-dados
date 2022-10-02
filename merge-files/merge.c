#include <stdio.h>
#include <stdlib.h>

void imprime_arquivo(char *nomeArq){
    FILE *arq; //declara ponteiro para arquivo
    //abre arquivo para leitura
    arq = fopen(nomeArq, "r");
    if (arq != NULL){// checa se não deu erro na abertura do arquivo
        char s[10];
        fscanf(arq, "%s", s);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s\n", s);
            fscanf(arq, "%s", s);
        }
        fclose(arq); //fecha arquivo
    }
    else printf("Erro ao abrir arquivo\n");
}

void merge(char *nomeArq1, char *nomeArq2, char *nomeArqMerge) {
    //TODO: Implementar essa função
    FILE *arq1;
    FILE *arq2;
    FILE *arqOut;


    arq1 = fopen(nomeArq1, "r");
    arq2 = fopen(nomeArq2, "r");
    arqOut = fopen(nomeArqMerge, "w");

    if(arq1 != NULL && arq2 != NULL && arqOut != NULL){
        int p1;
        int p2;

        fscanf(arq1, "%d", &p1);
        fscanf(arq2, "%d", &p2);
        

        while ( !feof(arq1) && !feof(arq2) ) //Se os 2 arquivos não terminaram
        {
            if (p1 == p2){
                //iguais
                fprintf(arqOut, "%i\n", p1); //escreve 1
                fscanf(arq1, "%d", &p1); //avança cursor 1
                fscanf(arq2, "%d", &p2); // avança cursor 2
            }
            else if (p1 < p2){
                //p1 menor
                fprintf(arqOut, "%i\n", p1); //escreve 1
                fscanf(arq1, "%d", &p1); //avança cursor 1
            }
            else if (p2 < p1){
                //p2 menor
                fprintf(arqOut, "%i\n", p2); //escreve 2
                fscanf(arq2, "%d", &p2); //avança cursor 2
            }
        }
        if( feof(arq1) ){ //Arq 1 terminou
            //dump no arq2
            while (!feof(arq2))
            {
                fprintf(arqOut, "%i\n", p2); //escreve 2
                fscanf(arq2, "%d", &p2); //avança cursor 2
            }
            

        } else{ //Arq 2 terminou
            //dump no arq1
            while (!feof(arq1))
            {
                fprintf(arqOut, "%i\n", p1); //escreve 2
                fscanf(arq1, "%d", &p1); //avança cursor 2
            }
        }
        
    } else{
        printf("\nErro ao abrir os arquivos\n");
    }

    if(arq1 != NULL){
        fclose(arq1);
    }
    if(arq2 != NULL){
        fclose(arq2);
    }
    if(arqOut != NULL){
        fclose(arqOut);
    }
    
    
}

int main(int argc, char **argv) {
    merge("numeros1.txt", "numeros2.txt", "merge.txt");
    imprime_arquivo("merge.txt");
}
