#include <limits.h>
#include <stdio.h>

#include "cliente.c"
#include "compartimento_hash.c"

#define M 7

void imprime_arquivos(char *nome_arquivo_hash, char *nome_arquivo_dados) {
    //Imprime arquivo de hash
    printf("TABELA HASH:\n");
    imprime_arquivo_hash(nome_arquivo_hash);

    //Imprime arquivo de dados
    printf("\nDADOS:\n");
    imprime_arquivo_dados(nome_arquivo_dados);
}

/* Executa busca em Arquivos por Encadeamento Exterior (Hash)
 * Assumir que ponteiro para proximo noh eh igual a -1 quando nao houver proximo noh
 * Ponteiros são lógicos (é preciso multiplicar pelo tamanho do registro para calcular o valor do deslocamento
 * Use a função tamanho_compartimento() e tamanho_cliente() dependendo do arquivo que está sendo manipulado
 *
 * Parametros:
 * cod_cli: chave do cliente que esta sendo buscado
 * nome_arquivo_hash: nome do arquivo que contem a tabela hash
 * nome_arquivo_dados: nome do arquivo onde os dados estao armazenados
 * m: tamanho da tabela hash (a função de hash é h(x) = x mod m
 *
 * Retorna o endereco (lógico) onde o cliente foi encontrado, ou -1 se nao for encontrado
 */

int isCliente(TCliente* cli, int cod){
    if (cli->cod == cod && cli->ocupado) return 1;
    else return 0;
}

int busca(int cod_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int m) {
    //TODO: Implementar essa função
    int resultado = -1;

    FILE* tabela;
    FILE* dados;

    tabela = fopen(nome_arquivo_hash, "rb");
    dados = fopen(nome_arquivo_dados, "rb");
    if (tabela == NULL || dados == NULL){printf("Não foi possível abrir algum arquivo essencial"); return -1;} //Guard clause pra deixar mais legivel, valor -2 arbitrário

    TCompartimento* compart;
    TCliente* cli;

    int compart_num = cod_cli % m;
    fseek(tabela, compart_num * tamanho_compartimento(), SEEK_SET); //Busca o endereço do compartimento
    compart = le_compartimento(tabela); //Carrega compartimento em memória

    int logic_address = compart->prox;
    // printf("Compartimento: %d | Endereço: %d == %d\n", cod_cli%m, compart->prox, logic_address);
    if(logic_address != -1) {
        fseek(dados, logic_address * tamanho_cliente(), SEEK_SET); //Busca endereço do dado
        cli = le_cliente(dados); //Carrega cliente na memoria
        imprime_cliente(cli);
        
        while (cli->prox != -1 && !isCliente(cli, cod_cli)) //Percorre lista encadeada de clientes até achar e ocupado ou acabar
        {
            logic_address = cli->prox;
            fseek(dados, logic_address * tamanho_cliente(), SEEK_SET);
            cli = le_cliente(dados);
            imprime_cliente(cli);            
        }

        if (isCliente(cli, cod_cli)){ //Se achou
            resultado = logic_address;
        }
        
    }

    return resultado;
}

int main() {
    /* Essa função gera a saída que é usada no teste do run.codes. Ela NÃO DEVE SER MODIFICADA */
    int cod;
    int pont;

    //Imprime arquivos de entrada
    // imprime_arquivos("tabHash.dat", "clientes.dat");

    //le chave a ser buscada
    scanf("%d", &cod);

    // printf("Buscando: %d\n", cod);

    printf("REGISTROS ACESSADOS:\n");
    pont = busca(cod, "tabHash.dat", "clientes.dat", M);

    //Imprime resultado da função
    printf("RESULTADO: %d", pont);
}