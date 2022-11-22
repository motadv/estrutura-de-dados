#include <stdio.h>
#include "metadados.c"
#include "no_interno.c"

void imprime_arquivos() {
    int i;
    //Imprime arquivo de metadados
    FILE *arq_m = fopen("metadados.dat", "rb");
    TMetadados *m = le_metadados(arq_m);
    printf("*** Arquivo de Metadados ***\n");
    imprime_metadados(m);
    fclose(arq_m);

    //Imprime arquivo de índice (nós internos da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_i = fopen("indice.dat", "rb");
    printf("\n\n*** Arquivo de Indice ***\n");
    i = 0;
    TNoInterno *ni = le_no_interno(m->d, arq_i);
    while (ni != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_interno(m->d));
        imprime_no_interno(m->d, ni);
        ni = le_no_interno(m->d, arq_i);
        i += 1;
    }
    fclose(arq_i);

    //Imprime arquivo de dados (nós folha da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_d = fopen("clientes.dat", "rb");
    printf("\n*** Arquivo de Dados ***\n");
    i = 0;
    TNoFolha *nf = le_no_folha(m->d, arq_d);
    while (nf != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_folha(m->d));
        imprime_no_folha(m->d, nf);
        nf = le_no_folha(m->d, arq_d);
        i += 1;
    }
    fclose(arq_d);
}

/*
 * Executa busca em Arquivos utilizando Arvore B+
 * Assumir que ponteiros para NULL têm valor -1
 *
 * chave: chave do cliente que esta sendo buscado
 * nome_arquivo_metadados: nome do arquivo binário que contem os metadados
 * nome_arquivo_indice: nome do arquivo binário de indice (que contem os nohs internos da arvore B+)
 * nome_arquivo_dados: nome do arquivo binário de dados (que contem as folhas da arvore B+)
 *
 * Retorna ponteiro para nó em que a chave está ou deveria estar
 */
int busca(int chave, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
	//TODO: Implementar essa função

    int resultado;
    int is_folha;
    int contador;
    int address;
    TNoFolha* folha;
    TNoInterno* indice;
    TMetadados* meta;

    //Carrega arquivo de metadados na memória
    FILE* fmetadado = fopen(nome_arquivo_metadados, "rb");
    if(fmetadado != NULL){
        meta = le_metadados(fmetadado);
        fclose(fmetadado);
    }
    
    //Abre arquivos de indice e dados
    FILE* findice = fopen(nome_arquivo_indice, "rb");
    if (findice != NULL)
    {
        FILE* fdados = fopen(nome_arquivo_dados, "rb");
        if(fdados != NULL)
        {
                //>>> Logica entra aqui <<<
                is_folha = meta->raiz_folha;
                address = meta->pont_raiz;
                if(is_folha){
                    fseek(fdados, address, SEEK_SET);
                    folha = le_no_folha(meta->d, fdados);
                    imprime_no_folha(meta->d, folha);

                    resultado = meta->pont_raiz;
                }
                else
                {
                    address = meta->pont_raiz;
                    while (!is_folha)
                    {
                        fseek(findice, address, SEEK_SET);
                        indice = le_no_interno(meta->d, findice);
                        imprime_no_interno(meta->d, indice);

                        contador = 0;
                        while (indice->chaves[contador] <= chave && contador < indice->m)
                        {
                            contador++;
                        }

                        is_folha = indice->aponta_folha;
                        address = indice->p[contador];
                    }

                    //Proximo address é para uma folha:
                    fseek(fdados, address, SEEK_SET);
                    folha = le_no_folha(meta->d, fdados);
                    imprime_no_folha(meta->d, folha);

                    resultado = address;                    
                }

                return resultado;
                //>>> Fim da Logica <<<

            fclose(fdados);
        }
        fclose(findice);
    }
    //Verifica se raiz é folha no metadado
        //Se for no folha
            //carrega folha na memoria
            //imprime no folha
            //retorna endereço
        //Se não for nó folha
            //is_folha = 0

            //Enquanto !is_folha:
                //carrega indice na memoria
                //imprime indice

                //contador = 0
                //Percorre nó enquanto chave[contador] < chave && contador < m
                    //contador++
                //Sai do loop com contador sendo ponteiro que tem que descer.

                //is_folha = aponta folha

            //Quando o nó finalmente for folha:
                //carrega folha na memoria
                //imprime folha
                //resultado estará no conteúdo do contador
                //retorna conteudo do contador

	return -1;
}

int main () {
    //Descomente essa linha de código caso deseje imprimir o conteúdo dos arquivos de entrada para analisar
    //o comportamento da sua implementação

    // imprime_arquivos();

    /* Le chave a ser buscada */
    int chave;

    scanf("%d", &chave);

    //Chama função de busca
    int pont = busca(chave, "metadados.dat", "indice.dat", "clientes.dat");

    //Imprime resultado da função
    printf("RESULTADO DA BUSCA: %d", pont);
}