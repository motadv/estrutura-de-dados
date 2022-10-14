#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct viz{
    int no_viz;
    struct viz *prox_viz;
}TV;
typedef struct grafo{
    int no;
    int cor;
    struct grafo *prox;
    TV *prim;
}TG;

TG* busca(TG* g, TV* v){return NULL;}

int nao_tem_mesma_cor(TG* g){
    int res = 1;

    TG* atual = g;
    while (atual != NULL && res != 0)   
    {

        TV* v_atual = g->prim;
        while (v_atual != NULL && res != 0)
        {
            res = (atual->cor != busca(g, v_atual)->cor);
            v_atual = v_atual->prox_viz;
        }

        atual = atual->prox;
    }
    
    return res;
}

int iguais(TG* grafo1, TG* grafo2){
    int res = 1;

    TG* g1 = grafo1;
    TG* g2 = grafo2;
    while (g1 != NULL && g2 != NULL)   
    {
        if (g1->no != g2->no){return 0;}

        TV* v1 = g1->prim;
        TV* v2 = g2->prim;
        while (v1 != NULL && v2 !=NULL)
        {
            v1 = v1->prox_viz;
            v2 = v2->prox_viz;
        }

        g1 = g1->prox;
        g2 = g2->prox;
    }
    
    return res;
}

int main(int argc, char const *argv[])
{
    
    return 0;
}
