#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Departamento {
    int cod_dept;
    int sala;
    char nome[TAM_NOME];
} TDepartamento;

typedef struct Funcionario {
    int cod_func;
    int cod_dept;
    char nome[TAM_NOME];
} TFuncionario;

void salva_departamento(TDepartamento *dept, FILE *out)
{
    fprintf(out, "%d", dept->cod_dept);
    fprintf(out, "%c", ';');
    fprintf(out, "%d", dept->sala);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", dept->nome);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", "\n");
}

void salva_funcionario(TFuncionario *func, FILE *out)
{
    fprintf(out, "%d", func->cod_func);
    fprintf(out, "%c", ';');
    fprintf(out, "%d", func->cod_dept);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", func->nome);
    fprintf(out, "%c", ';');
    fprintf(out, "%s", "\n");
}

TFuncionario *le_funcionario(FILE *in)
{
    TFuncionario *func = (TFuncionario *) malloc(sizeof(TFuncionario));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(func);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    func->cod_func = cod;
    ptr = strtok(NULL, delimitador);
    cod = atoi(ptr);
    func->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    strcpy(func->nome, ptr);

    return func;
}

TDepartamento *le_departamento(FILE *in)
{
    TDepartamento *dept = (TDepartamento *) malloc(sizeof(TDepartamento));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(dept);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod, sala;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    dept->cod_dept = cod;
    ptr = strtok(NULL, delimitador);
    sala = atoi(ptr);
    dept->sala = sala;
    ptr = strtok(NULL, delimitador);
    strcpy(dept->nome, ptr);

    return dept;
}

void imprime_arquivo(char *name) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(name, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets (linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets (linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

typedef struct listaFunc
{
    TFuncionario* func;
    struct listaFunc* prox;
} ListaF;

void insere_fimF(ListaF **ld, TFuncionario *f)
{
    if(f == NULL){
        return;
    }

    //create a new node
    ListaF *novo = malloc(sizeof(ListaF));
    novo->func = f;
    novo->prox= NULL;

    //if ld is NULL, it is an empty list
    if(*ld == NULL)
         *ld = novo;
    //Otherwise, find the last node and add the novo
    else
    {
        ListaF *p = *ld;

        //p's next address will be NULL.
        while(p->prox != NULL)
        {
            p = p->prox;
        }

        //add the novo at the end of the linked list
        p->prox = novo;
    }

}

ListaF* cria_listaF(void){
    return NULL;
}

typedef struct listaDep
{
    TDepartamento* dep;
    struct listaDep* prox;
} ListaD;

ListaD* cria_listaD(void){
    return NULL;
}

void insere_fimD(ListaD **ld, TDepartamento *d)
{
    if(d == NULL){
        return;
    }

    //create a new node
    ListaD *novo = malloc(sizeof(ListaD));
    novo->dep = d;
    novo->prox= NULL;

    //if ld is NULL, it is an empty list
    if(*ld == NULL)
         *ld = novo;
    //Otherwise, find the last node and add the novo
    else
    {
        ListaD *p = *ld;

        //p's next address will be NULL.
        while(p->prox != NULL)
        {
            p = p->prox;
        }

        //add the novo at the end of the linked list
        p->prox = novo;
    }

}


void imprime_listaF (ListaF* li) {
    ListaF* p = li;
    while (p != NULL)
    {
        printf("%s\n", p->func->nome);
        p=p->prox;
    }

}

void imprime_listaD (ListaD* li) {
    ListaD* p = li;
    while (p != NULL)
    {
        printf("%s->", p->dep->nome);
        p=p->prox;
    }
}

void escreve_registro(FILE* arq_out, TDepartamento* dep, TFuncionario* func){
    //Escrever:
    //cod dep
    //sala dep
    //nome dep
    //cod func
    //nome func
    if(arq_out != NULL){
        fprintf(arq_out, "%d;%d;%s;%d;%s;\n", dep->cod_dept, dep->sala, dep->nome, func->cod_func, func->nome);
    }
}

void join(char *nome_arq_dept, char *nome_arq_funcionarios, char *nome_arq_join) {
    //TODO: Implementar essa função
    FILE* arq_dept;
    FILE* arq_func;
    FILE* arq_out;

    ListaD* ld = cria_listaD();
    ListaF* lf = cria_listaF();

    arq_dept = fopen(nome_arq_dept, "r");
    if(arq_dept != NULL){
        arq_func = fopen(nome_arq_funcionarios, "r");
            if(arq_func != NULL){
                
                //Adicionando na memoria os departamentos e funcionarios
                while (!feof(arq_dept))
                {
                    insere_fimD(&ld, le_departamento(arq_dept));
                }

                while (!feof(arq_func)){
                    insere_fimF(&lf, le_funcionario(arq_func));
                }

                fclose(arq_func);
            }
        fclose(arq_dept);
    }

    // printf("\n\n imprime dep \n\n");
    // imprime_listaD(ld);
    // printf("\n\n imprime funcs \n\n");
    // imprime_listaF(lf);

    //Não preciso mais dos arquivos
    arq_out = fopen(nome_arq_join, "w");
    if(arq_out != NULL){
        ListaD* d = ld;
            while (d != NULL)
            {
                //Para cada departamento
                //Settar cabeça da lista de funcs de volta no começo
                ListaF* f = lf;
                while (f != NULL)
                {
                    if(f->func->cod_dept == d->dep->cod_dept){
                        //func == dep
                        escreve_registro(arq_out, d->dep, f->func);
                    }

                    f = f->prox;
                }
                
                d = d->prox;
            }
            fclose(arq_out);
    }
}

int main() {
    join("departamentos.txt", "funcionarios.txt", "join.txt");
    imprime_arquivo("join.txt");
}