#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/* VARIï¿½VEIS CONSTANTES */
// TODOS OS TEMPOS ESTï¿½O EM UNIDADES DE TEMPO (U.T.)

#define QTDE_PROC 15		// Nï¿½mero de processos
#define TMAX 15				// Tempo mï¿½ximo de serviï¿½o de um processo
#define QUANTUM 3			// Fatia de tempo de cada processo na CPU 
#define TEMPO_DISCO 2 		// Tempo de E/S para Disco 
#define TEMPO_FITA 6 		// Tempo de E/S para Fita Magnï¿½tica
#define TEMPO_IMPRESSORA 4 	// Tempo de E/S para Impressora
#define PROB_IO 4			// Probabilidade com que um processo solicita IO na criaï¿½ï¿½o (1/freq)

typedef struct {
	char tipo;				// Tipo de IO que vai ser solicitado (D, F, I) ou zero se nï¿½o tem tipo
	int inicio;				// Quando ocorrerï¿½ a solicitaï¿½ï¿½o
	int duracao;			// Duraï¿½ï¿½o do IO
} IO;

 typedef struct PCB{
	int pid;				// Identificador do processo
	int tempo_serv;			// Tempo de serviï¿½o para o processo executar
	int criacao;			// Tempo em que processo foi criado
	IO* io;					// O momentos de IO do processo
	struct PCB *prox, *ant;	// Aponta para o prï¿½ximo elemento e para o elemento anterior
} PCB;

typedef struct {
    PCB* inicio;				// Topo da fila
    PCB* fim;					// Base da fila
    int qtde;  					// Quantidade de elementos na fila
} FILA;


/*
FILA* filaAltaPrioridade;
FILA* filaBaixaPrioridade;
FILA* filaIO;
*/

int qtdeProcessos = 0;



/* Funï¿½ï¿½es */

//Aloca fila
FILA* aloca_fila(){
	FILA* fila = (FILA *) malloc(sizeof(FILA));
	fila->inicio = NULL;
	fila->fim = NULL;
	fila->qtde = 0;
	
	return fila;
}

// Cria os processos
FILA* gera_processos(){
    FILA* fila = aloca_fila();
    
    char tipo[3] = {'D', 'F', 'I'};
	int TIO[3] = {TEMPO_DISCO, TEMPO_FITA, TEMPO_IMPRESSORA};
    
    // Primeiro processo
    fila->fim = (PCB *) malloc(sizeof(PCB));
    
    fila->fim->prox = NULL;
	fila->fim->ant = NULL;
	
    fila->inicio = fila->fim;
    fila->fim->pid = 1;
    
   	fila->fim->tempo_serv = rand() % TMAX + 1; // Gera um valor aleatorio para tempo de serv
   	fila->fim->criacao = 0;
    
    if(rand() % PROB_IO){ // Sorteia se o processo tem IO
   		fila->fim->io = (IO *) malloc(sizeof(IO));
   		fila->fim->io->inicio = -1;
    	fila->fim->io->tipo = 0;
    	fila->fim->io->duracao = -1;
   	}
    else{
    	int t = rand() % 3;
    	fila->fim->io = (IO *) malloc(sizeof(IO));
    	fila->fim->io->inicio = rand() % (fila->fim->tempo_serv + 1);
    	
    	// Define o tipo de IO solicitado e o tempo de duracao do IO
    	fila->fim->io->tipo = tipo[t];
    	fila->fim->io->duracao = TIO[t];
    }
    
    int i = 1;
    while(i < qtdeProcessos){
    	fila->inicio->prox = (PCB *) malloc(sizeof(PCB));
    	fila->inicio->prox->ant = fila->inicio;
    	fila->inicio = fila->inicio->prox;
    	fila->inicio->prox = NULL;
    	fila->inicio->pid = i+1;
    	fila->inicio->tempo_serv = rand() % TMAX + 1;
    	fila->inicio->criacao = fila->inicio->ant->criacao + (rand() % 5 + 1);
    	
    	if(rand() % PROB_IO){
    		fila->inicio->io = (IO *) malloc(sizeof(IO));
    		fila->inicio->io->inicio = -1;
    		fila->inicio->io->tipo = 0;
    		fila->inicio->io->duracao = -1;
    	}
    	else{
    		int t = rand() % 3;
    		fila->inicio->io = (IO *) malloc(sizeof(IO));
    		fila->inicio->io->inicio = rand()%(fila->inicio->tempo_serv + 1);
    		fila->inicio->io->tipo = tipo[t];
    		fila->inicio->io->duracao = TIO[t];
    	}
    	
    	i++;
	}
    
    
    
    return fila;
}


void imprimirTabela(int tabela[][2], int LINHAS){
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d\t", tabela[i][j]);
        }
        printf("\n");
    }
}

// Retira processo do inicio da fila
PCB* retira_processo_fila(FILA* fila){
	if (fila->inicio == NULL) {
        return NULL; // Fila vazia
    }

    PCB* temp = fila->inicio;
    fila->inicio = temp->ant;
    
    temp->ant = NULL;
    temp->prox = NULL;

    if (fila->inicio == NULL) {
        fila->fim = NULL; // Fila agora esta vazia
    }

    fila->qtde--;

    return temp;
}

// Vai add no fim da fila de alta e baixa prioridade
void adiciona_processo_fila(FILA* fila, PCB* processo){
	if (fila->inicio == NULL) {
        fila->inicio = fila->fim = processo;
        processo->ant = NULL;
        processo->prox = NULL;
    } else {
    	processo->prox = fila->fim;
    	
        fila->fim->ant = processo;
        fila->fim = processo;
        
        processo->ant = NULL;
    }

    fila->qtde++;
}

// Funcao para imprimir os elementos da fila
void imprimirFila(FILA* fila) {
    PCB* atual = fila->inicio;

    printf("Elementos da fila: ");
    while (atual != NULL) {
        printf("%d ", atual->pid);
        atual = atual->ant;
    }
    printf("\n");
}

void imprimirTabelaIO(int tabela[][3], int linhas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d\t", tabela[i][j]);
        }
        printf("\n");
    }
}


FILA* filaAltaPrioridade;
FILA* filaBaixaPrioridade;
FILA* filaIO;


int main(){
	srand(time(NULL));
	qtdeProcessos = QTDE_PROC;
	int processosTerminados = 0;
	
	int tabela_IO[QTDE_PROC][3] = {{0}};
	int tabela_processos[QTDE_PROC][2];
	
    // Algoritmo RR com Feedback
    printf("### SIMULADOR ###\n");
  	printf("Condicao de corrida:\n");
  	
  	// Condicao de corrida e IO's
  	
  	// Mostrar os processos, seus tempos de serviï¿½o e seus IO
  	printf("-------------------------------------------\n");
  	FILA* listaProcessos = gera_processos();
  	PCB* processo = listaProcessos->fim;
    for(int i = 0; i < qtdeProcessos; i++){
    	tabela_processos[i][0] = processo->pid;
    	tabela_processos[i][1] = processo->criacao;
    	printf("P%i com tempo de servico = %i u.t.", processo->pid, processo->tempo_serv);
    	if(processo->io->tipo){
    		if(processo->io->tipo == 'D')
    			printf(", IO de Disco em %i u.t.\n", processo->io->inicio);
    		else if(processo->io->tipo == 'F')
    			printf(", IO de Fita em %i u.t.\n", processo->io->inicio);
    		else if(processo->io->tipo == 'I')
    			printf(", IO de Impressora em %i u.t.\n", processo->io->inicio);
    	}
    	else printf("\n");
    	processo = processo->prox;
    }
    printf("-------------------------------------------\n");
    
    // Mostra tabela dos instantes de entrada dos processos
    printf("|PROCESSO|INSTANTE DE ENTRADA|\n");
    imprimirTabela(tabela_processos, QTDE_PROC);
    
    // Inicio simulacao
    int instanteAtual = 0;
    int temProcessoExecutar; // Var booleana para verificar se há processo pra executar
    int tempoDecorrido = 0;
    processo = listaProcessos->fim;

	filaAltaPrioridade = aloca_fila();
	filaBaixaPrioridade = aloca_fila();
	filaIO = aloca_fila();

    while(processosTerminados < QTDE_PROC){
    	instanteAtual += tempoDecorrido;
    	
    	// Verifica se chegou algum processo novo e se sim o coloca no fim da fila de alta prioridade
    	if(processo->criacao <= instanteAtual){
    		PCB* novoProcesso = (PCB *) malloc(sizeof(PCB));
    		
    		novoProcesso->ant = NULL;
    		novoProcesso->prox = NULL;
    		novoProcesso->criacao = processo->criacao;
    		novoProcesso->tempo_serv = processo->tempo_serv;
    		novoProcesso->io = processo->io;
    		novoProcesso->pid = processo->pid;
    		
    		
    		
			adiciona_processo_fila(filaAltaPrioridade, novoProcesso);
			printf("Novo processo P%d criado no instante %d\n\n", novoProcesso->pid, novoProcesso->criacao);
			processo = processo->prox;
		}
		
		// Verificar se algum IO terminou e colocar na fila adequada
		
		
		// Executa processo
		temProcessoExecutar = (filaAltaPrioridade->qtde) + (filaBaixaPrioridade->qtde);
		
		
		if(!temProcessoExecutar){
			tempoDecorrido = 1;
		}else{
			// verificar se tem IO
			if(filaAltaPrioridade->qtde > 0){
				PCB* processoExecutar = retira_processo_fila(filaAltaPrioridade);
				
				printf("---Infos processo---\n");
				printf("Criacao = %d\n", processoExecutar->criacao);
				printf("Tempo_serv = %d\n", processoExecutar->tempo_serv);
				printf("PID = %d\n", processoExecutar->pid);
				printf("--------------------------\n\n");
				
				
				
				
				printf("Processo P%d comecou a executar em %d\n", processoExecutar->pid, instanteAtual);
				int inicioIO = processoExecutar->io->inicio;
				if(inicioIO != -1){
					if(inicioIO - QUANTUM <= 0){
						tempoDecorrido = inicioIO;
						processoExecutar->tempo_serv -= tempoDecorrido;
						processoExecutar->io->inicio = -1;
						printf("Processo executou por %d\n", tempoDecorrido);
						
						adiciona_processo_fila(filaIO, processoExecutar);
						tabela_IO[processoExecutar->pid - 1][0] = processoExecutar->pid;
						tabela_IO[processoExecutar->pid - 1][1] = instanteAtual + tempoDecorrido;
						tabela_IO[processoExecutar->pid - 1][2] = (instanteAtual + tempoDecorrido) + processoExecutar->io->duracao;
						
						printf("Processo pediu solicitacao de IO do tipo %c no instante %d\n\n", processoExecutar->io->tipo, (instanteAtual + tempoDecorrido));
					}
					else{
						processoExecutar->io->inicio -= QUANTUM; // adptar inicio
						processoExecutar->tempo_serv -= QUANTUM; // quanto ainda falta de temp_serv
						tempoDecorrido = QUANTUM;
						printf("Processo executou por %d\n", tempoDecorrido);
						
						adiciona_processo_fila(filaBaixaPrioridade, processoExecutar);
						printf("Processo %d sofreu preempcao no instante %d\n\n", processoExecutar->pid, (instanteAtual + tempoDecorrido));
					}
				}else{
					if(processoExecutar->tempo_serv - QUANTUM <= 0){
						tempoDecorrido = processoExecutar->tempo_serv;
						processoExecutar->tempo_serv = 0;
						printf("Processo executou por %d\n", tempoDecorrido);
					
						printf("Processo %d terminou a execucao no instante %d\n\n", processoExecutar->pid, (instanteAtual + tempoDecorrido));
					}
					else{
						processoExecutar->tempo_serv -= QUANTUM;
						tempoDecorrido = QUANTUM;
						printf("Processo executou por %d\n", tempoDecorrido);
					
						adiciona_processo_fila(filaBaixaPrioridade, processoExecutar);
						printf("Processo %d sofreu preempcao no instante %d\n\n", processoExecutar->pid, (instanteAtual + tempoDecorrido));
					}
				}
			}else{
				tempoDecorrido = 1;
				printf("ELSE\n\n");
			}
		}
    	
		processosTerminados++;
	} // Fim while
    
    imprimirFila(filaAltaPrioridade);
    printf("%d", filaAltaPrioridade->qtde);
    printf("\n\nTABELA IO\n");
    
    imprimirTabelaIO(tabela_IO, QTDE_PROC);
    
    /*
    free(filaAltaPrioridade);
	free(filaBaixaPrioridade;
	free(filaIO);
	*/
	
	return 0;
}
