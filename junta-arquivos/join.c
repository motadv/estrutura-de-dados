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

ListaF* insere_func (ListaF* li, TFuncionario* f) {
    ListaF* novo = (ListaF*) malloc(sizeof(ListaF));
    novo->func = f;
    novo->prox = NULL;
    ListaF* p = li;
    if (p == NULL) { //se a lista estiver vazia
        li = novo;
    }
    else {
        while (p->prox != NULL) { //encontra o ultimo elemento
            p = p->prox;
        }
        p->prox = novo;
    }
    return li;
} 

typedef struct listaDep
{
    TDepartamento* dep;
    struct listaDep* prox;
} ListaD;

ListaD* insere_dep (ListaD* li, TDepartamento* d) {
    ListaD* novo = (ListaD*) malloc(sizeof(ListaD));
    novo->dep = d;
    novo->prox = NULL;
    ListaD* p = li;
    if (p == NULL) { //se a lista estiver vazia
        li = novo;
    }
    else {
        while (p->prox != NULL) { //encontra o ultimo elemento
            p = p->prox;
        }
        p->prox = novo;
    }
    return li;
} 

void escreve_registro(FILE* arq_out, TDepartamento* dep, TFuncionario* func){
    //Escrever:
    //cod dep
    //sala dep
    //nome dep
    //cod func
    //nome func
    if(arq_out != NULL){
        fwrite(&dep->cod_dept, sizeof(int), 1, arq_out);
        fputc(';', arq_out);

        fwrite(&dep->sala, sizeof(int), 1, arq_out);
        fputc(';', arq_out);

        fwrite(dep->nome, sizeof(char), sizeof(dep->nome), arq_out);
        fputc(';', arq_out);

        fwrite(&func->cod_func, sizeof(int), 1, arq_out);
        fputc(';', arq_out);

        fwrite(func->nome, sizeof(char), sizeof(func->nome), arq_out);
        fputc(';', arq_out);
    }
}

void join(char *nome_arq_dept, char *nome_arq_funcionarios, char *nome_arq_join) {
    //TODO: Implementar essa função
    FILE* arq_dept;
    FILE* arq_func;
    FILE* arq_out;

    ListaD* ld = NULL;
    ListaF* lf = NULL;

    arq_dept = fopen(nome_arq_dept, "r");
    if(arq_dept != NULL){
        arq_func = fopen(nome_arq_funcionarios, "r");
            if(arq_func != NULL){
                
                //Adicionando na memoria os departamentos e funcionarios
                while (!feof(arq_dept))
                {
                    insere_dep(ld, le_departamento(arq_dept));
                }

                while (!feof(arq_func)){
                    insere_func(lf, le_funcionario(arq_func));
                }

                fclose(arq_func);
            }
        fclose(arq_dept);
    }

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
    }
    
    
}

int main() {
    join("departamentos.txt", "funcionarios.txt", "join.txt");
    imprime_arquivo("join.txt");
}