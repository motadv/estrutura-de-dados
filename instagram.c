#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct vizinho {
    char nome[10];
    struct vizinho *prox;
} TVizinho;

typedef struct grafo {
    char nome[10];
    int idade;
    TVizinho *prim_vizinho;
    struct grafo *prox;
} TGrafo;

TGrafo *insere_vertice(TGrafo *g, char *nome, int idade) {
    TGrafo *vertice = (TGrafo *) malloc(sizeof(TGrafo));
    strcpy(vertice->nome, nome);
    vertice->idade = idade;
    vertice->prox = g;
    vertice->prim_vizinho = NULL;
    return vertice;
}

TGrafo *busca_vertice(TGrafo *vertice, char *nome) {
    while ((vertice != NULL) && (strcmp(vertice->nome, nome) != 0)) {
        vertice = vertice->prox;
    }
    return vertice;
}

TVizinho *busca_vizinho(TVizinho *vizinho, char *nome) {
    while ((vizinho != NULL) && (strcmp(vizinho->nome, nome) != 0)) {
        vizinho = vizinho->prox;
    }
    return vizinho;
}

void insere_aresta(TGrafo *g, char *nome_origem, char *nome_destino) {
    TGrafo *vertice = busca_vertice(g, nome_origem);
    TVizinho *vizinho = (TVizinho *) malloc(sizeof(TVizinho));
    strcpy(vizinho->nome, nome_destino);
    vizinho->prox = vertice->prim_vizinho;
    vertice->prim_vizinho = vizinho;
}

void imprime(TGrafo *vertice) {
    while (vertice != NULL) {
        printf("Pessoa: %s - %d anos\n", vertice->nome, vertice->idade);
        printf("Segue: ");
        TVizinho *vizinho = vertice->prim_vizinho;
        while (vizinho != NULL) {
            printf("%s ", vizinho->nome);
            vizinho = vizinho->prox;
        }
        printf("\n\n");
        vertice = vertice->prox;
    }
}

int numero_seguidos(TGrafo *g, char *nome) {
    //TODO: Implementar essa função
    TGrafo* a = g;
    a = busca_vertice(a, nome);

    if (a != NULL){
        
        int numSeg = 0;
        TVizinho* v = a->prim_vizinho;
        if (v != NULL){
            numSeg = 1;
            while (v->prox != NULL)
            {
                numSeg = numSeg + 1;
                v = v->prox;
            }
            
        }

        return numSeg;
    }

    return 0;
}

int seguidores(TGrafo *vertice, char *nome, int imprime) {
    
    TGrafo* g = vertice;
    TVizinho* v = NULL;
    int numSeg = 0;

    while (g != NULL)
    {
        v = busca_vizinho(g->prim_vizinho, nome);
        if(v != NULL){
            numSeg = numSeg + 1;
            if (imprime == 1){
                printf("%s ", g->nome);
            }
        }

        g = g->prox;
    }
    
    if (imprime == 1){
        printf("\n");
    }

    return numSeg;
}

TGrafo *mais_popular(TGrafo *g) {
    TGrafo* grafo = g;
    TGrafo* a = g;
    TGrafo* pop = NULL;
    int maxSeg = -1;

    while (a != NULL)
    {
        int seg = seguidores(grafo, a->nome, 0);
        if(maxSeg < seg)
        {
            maxSeg = seg;
            pop = a;
        }

        a = a->prox;
    }
    
    return pop;
}

int segue_mais_velho(TGrafo *g, int imprime) {
    
    TGrafo* grafo = g;
    TGrafo* aux = g;
    TVizinho* v = NULL;
    int quant = 0;


    while (aux != NULL){
        int soMais = 1;

        v = aux->prim_vizinho;
        if(busca_vertice(grafo, v->nome)->idade <= aux->idade){
            soMais = 0;
        }
        v = v->prox;
        while (soMais != 0 && v != NULL)
        {
            if(busca_vertice(grafo, v->nome)->idade <= aux->idade){
                soMais = 0;
            }

            v = v-> prox;
        }

        if(soMais != 0){
            if(imprime == 1){
                printf("%s ", aux->nome);
            }

            quant = quant + 1;
        }

        aux = aux->prox; 
        
    }

    if(imprime == 1){
        printf("\n");
    }

    return quant;
}

void libera_vizinho(TVizinho *vizinho) {
    if (vizinho != NULL) {
        libera_vizinho(vizinho->prox);
        free(vizinho);
    }
}

void libera_vertice(TGrafo *vertice) {
    if (vertice != NULL) {
        libera_vizinho(vertice->prim_vizinho);
        libera_vertice(vertice->prox);
        free(vertice);
    }
}

int main() {
    /* A função main lê os dados de entrada, cria o grafo e chama as funções solicitadas no problema
    * depois imprime os resultados solicitados
    * ELA NÃO DEVE SER MODIFICADA
    * */
    int num_vertices, num_arestas;
    char nome[30];
    char origem[30], destino[30];
    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int idade;
    int i;
    TGrafo *g = NULL;

    //le numero de vertices
    scanf("%d", &num_vertices);
    //le e cria os vertices
    for (i = 0; i < num_vertices; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(nome,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        idade = atoi(ptr);
        g = insere_vertice(g, nome, idade);
    }

    //Le numero de arestas e depois le os dados de cada aresta
    //Cria as arestas no grafo
    scanf("%d", &num_arestas);
    for (i = 0; i < num_arestas; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(origem,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        strcpy(destino,ptr);
        insere_aresta(g, origem, destino);
    }

    //Le nome de pessoa
    scanf("%s", nome);

    //Encontra o número de seguidos dessa pessoa
    printf("SEGUIDOS por %s: %d\n", nome, numero_seguidos(g, nome));

    //Encontra os seguidores de uma determinada pessoa
    printf("SEGUIDORES de %s:\n", nome);
    seguidores(g, nome, 1);

    TGrafo *p;

    //Encontra mais popular
    p = mais_popular(g);
    printf("MAIS POPULAR: %s\n", p->nome);

    //Encontra as pessoas que seguem apenas pessoas mais velhas
    printf("SEGUEM APENAS PESSOAS MAIS VELHAS:\n");
    segue_mais_velho(g,1);

    libera_vertice(g);
}