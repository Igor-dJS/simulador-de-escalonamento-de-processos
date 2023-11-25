#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/* VARI¡VEIS CONSTANTES */
// TODOS OS TEMPOS EST√O EM UNIDADES DE TEMPO (U.T.)

#define QTDE_PROC 15		// N˙mero de processos
#define TMAX 15				// Tempo m·ximo de serviÁo de um processo
#define QUANTUM 3			// Fatia de tempo de cada processo na CPU 
#define TEMPO_DISCO 2 		// Tempo de E/S para Disco 
#define TEMPO_FITA 6 		// Tempo de E/S para Fita MagnÈtica
#define TEMPO_IMPRESSORA 4 	// Tempo de E/S para Impressora
#define PROB_IO 4			// Probabilidade com que um processo solicita IO na criaÁ„o (1/freq)

typedef struct {
	char tipo;				// Tipo de IO que vai ser solicitado (D, F, I) ou zero se n„o tem tipo
	int inicio;				// Quando ocorrer· a solicitaÁ„o
	int duracao;			// DuraÁ„o do IO
} IO;

 typedef struct PCB{
	int pid;				// Identificador do processo
	int tempo_serv;			// Tempo de serviÁo para o processo executar
	int criacao;			// Tempo em que processo foi criado
	IO* io;					// O momentos de IO do processo
	struct PCB *prox, *ant;	// Aponta para o prÛximo elemento e para o elemento anterior
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



/* FunÁıes */

//Alocar fila
FILA* aloca_fila(){
	FILA* fila = (FILA *) malloc(sizeof(FILA));
	fila->inicio = NULL;
	fila->fim = NULL;
	fila->qtde = 0;
	
	return fila;
}

// NOVO PROCESSO
FILA* gera_processos(){
    FILA* fila = aloca_fila();
    
    char tipo[3] = {'D', 'F', 'I'};
	int TIO[3] = {TEMPO_DISCO, TEMPO_FITA, TEMPO_IMPRESSORA};
    
    // Primeiro processo
    fila->fim = (PCB *) malloc(sizeof(PCB));;
    
    fila->fim->prox = NULL;
	fila->fim->ant = NULL;
	
    fila->inicio = fila->fim;
    fila->fim->pid = 1;
    
   	fila->fim->tempo_serv = rand() % TMAX + 1; // Gera um valor aleatÛrio pro tempo de serviÁo
   	fila->fim->criacao = rand() % 30;
    
    if(rand() % PROB_IO){ // Sorteia se o processo tem solicita√ß√£o de IO
   		fila->fim->io = (IO *) malloc(sizeof(IO));
   		fila->fim->io->inicio = -1;
    	fila->fim->io->tipo = 0;
    	fila->fim->io->duracao = -1;
   	}
    else{
    	int t = rand() % 3;
    	fila->fim->io = (IO *) malloc(sizeof(IO));
    	fila->fim->io->inicio = rand() % (fila->fim->tempo_serv + 1);
    	
    	// Define o tipo de IO solicitado e o tempo de duraÁ„o do tipo de IO para o processo
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
    	fila->inicio->criacao = rand() % 30;
    	
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
/*
// FunÁ„o que troca o processo na base de filaA para o topo de filaB
void troca_fila(PCB* processo, FILA* filaA, FILA* filaB){
	if(filaB->topo != NULL && filaA->qtde > 1){
		filaA->base = processo->prox;
		filaA->base->ant = NULL;
		
		filaB->topo->prox = processo;
		
		processo->prox = NULL;
		processo->ant = filaB->topo;
		
		filaB->topo = processo;
	}
	else if(filaB->topo != NULL && filaA->qtde == 1){
		filaA->base = NULL;
		filaA->topo = NULL;
		
		filaB->topo->prox = processo;
		
		processo->prox = NULL;
		processo->ant = filaB->topo;
		
		filaB->topo = processo;
	}
	else if(filaB->topo == NULL && filaA->qtde > 1){
		filaA->base = processo->prox;
		filaA->base->ant = NULL;
		
		processo->prox = NULL;
		processo->ant = NULL;
		
		filaB->base = processo;
		filaB->topo = processo;
	}
	else{
		filaA->base = NULL;
		filaA->topo = NULL;
		processo->prox = NULL;
		processo->ant = NULL;
		filaB->base = processo;
		filaB->topo = processo;
	}
	filaA->qtde--;
	filaB->numProcessos++;
}

// Fun√ß√£o para terminar processos
void termina_processo(PCB* processo, FilaProcessos* fila){
	if(fila->numProcessos == 1){
		fila->base = NULL;
		fila->topo = NULL;
	}
	else{
		fila->base = fila->base->prox;
		fila->base->ant = NULL;
	}
	printf("8: Processo %i conclu√≠do!\n", processo->pid);
	free(processo->io);
	free(processo);
	fila->numProcessos--;
	quantProc--;
}

// Fun√ß√£o que simula a execu√ß√£o do processo
int executa_processo(PCB* processo){
	int tempoPassado = 0;
	if(!(rand() % PROBFILHO)) cria_filho(processo);
	else puts("5: Nenhum processo filho foi criado");
	if(processo->io->tipo){ // Verifica se o processo tem solicita√ß√£o de IO
		if(processo->io->inicio - TRR <= 0){
			tempoPassado = processo->io->inicio;
			processo->tempo -= tempoPassado;
			processo->io->inicio = -1;
		}
		else{
			processo->io->inicio -= TRR;
			processo->tempo -= TRR;
			tempoPassado = TRR;
		}
	}
	else{
		if(processo->tempo - TRR <= 0){
			tempoPassado = processo->tempo;
			processo->tempo = -1;
		}
		else{
			processo->tempo -= TRR;
			tempoPassado = TRR;
		}
	}
	return tempoPassado;
}

void imprime_status(){
	if(filaIoDisco->numProcessos > 0) 
		printf("1: Processo %i utilizando o Disco com %is restante(s)\n", filaIoDisco->base->pid, filaIoDisco->base->io->duracao);
	if(filaIoFita->numProcessos > 0)
		printf("2: Processo %i utilizando a Fita com %is restante(s)\n", filaIoFita->base->pid, filaIoFita->base->io->duracao);
	if(filaIoImpressora->numProcessos > 0)
		printf("3: Processo %i utilizando a Impressora com %is restante(s)\n", filaIoImpressora->base->pid, filaIoImpressora->base->io->duracao);
	if(filaAltaPrioridade->numProcessos > 0) 
		printf("4: Fila de Alta Prioridade: Processo %i a ser executado com %is restante(s)\n", filaAltaPrioridade->base->pid, filaAltaPrioridade->base->tempo);
	else if(filaBaixaPrioridade->numProcessos > 0) 
		printf("4: Fila de Baixa Prioridade: Processo %i a ser executado com %is restante(s)\n", filaBaixaPrioridade->base->pid, filaBaixaPrioridade->base->tempo);
}

*/

void imprimirTabela(int tabela[][2], int LINHAS){
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d\t", tabela[i][j]);
        }
        printf("\n");
    }
}

void retira_processo_fila(FILA* fila){
	
}

void adiciona_processo_fila(FILA* fila){
	
}

int main(){
	srand(time(NULL));
	qtdeProcessos = QTDE_PROC;
	processosTerminados = 0;
	
	int tabela_IO[QTDE_PROC][3];
	int tabela_processos [QTDE_PROC][2];

	

    
    // Algoritmo RR com Feedback
    printf("### SIMULADOR ###\n");
  	printf("Condicao de corrida:\n");
  	
  	// CondiÁ„o de corrida e IO's
  	printf("-------------------------------------------\n");
  	FILA* listaProcessos = gera_processos();
  	PCB* processo = listaProcessos->fim;
    for(int i = 0; i < qtdeProcessos; i++){
    	tabela_processos[i][0] = processo->pid;
    	tabela_processos[i][1] = processo->criacao;
    	printf("Processo %i com tempo de CPU = %i u.t.", processo->pid, processo->tempo_serv);
    	if(processo->io->tipo){
    		if(processo->io->tipo == 'D')
    			printf(", IO = Disco e inicio em %i u.t.\n", processo->io->inicio);
    		else if(processo->io->tipo == 'F')
    			printf(", IO = Fita e inicio em %i u.t.\n", processo->io->inicio);
    		else if(processo->io->tipo == 'I')
    			printf(", IO = Impressora e inicio em %i u.t.\n", processo->io->inicio);
    	}
    	else printf("\n");
    	processo = processo->prox;
    }
    printf("-------------------------------------------\n");
    imprimirTabela(tabela_processos, QTDE_PROC);
    
    
    /*
    int tempoPassado = 0;
   	int tempoTotal = 0;
    while(qtdeProcessos > 0){
    	tempoTotal += tempoPassado;
    	printf("\n\n0: Instante %i u.t.:\n", tempoTotal);
    	imprime_status();
    	if(filaAltaPrioridade->numProcessos > 0){ // Procura processos para executar na fila de alta prioridade
    		processo = filaAltaPrioridade->base;
    		tempoPassado = executa_processo(processo);
    		printf("6: Processo %i ficou %is executando\n", processo->pid, tempoPassado);
    		passa_tempo_io(tempoPassado);
    		if(processo->tempo == -1)
    			termina_processo(processo, filaAltaPrioridade);
    		else{
    			if(processo->io->tipo == 'D' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Disco) na fila de alta prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    				troca_fila(processo, filaAltaPrioridade, filaIoDisco);
    			}
    			else if(processo->io->tipo == 'F' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Fita) na fila de alta prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    				troca_fila(processo, filaAltaPrioridade, filaIoFita);
    			}
    			else if(processo->io->tipo == 'I' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Impressora) na fila de alta prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    				troca_fila(processo, filaAltaPrioridade, filaIoImpressora);
    			}
    			else troca_fila(processo, filaAltaPrioridade, filaBaixaPrioridade);
    			printf("8: Processo %i foi bloqueado com %is restantes\n", processo->pid, processo->tempo);
    		}
    	}
    	else if(filaBaixaPrioridade->numProcessos > 0){ // Procura processos para executar na fila de baixa prioridade
    		processo = filaBaixaPrioridade->base;
    		tempoPassado = executa_processo(processo);
    		printf("6: Processo %i ficou %is executando\n", processo->pid, tempoPassado);
    		passa_tempo_io(tempoPassado);
    		if(processo->tempo == -1)
    			termina_processo(processo, filaBaixaPrioridade);
    		else{
    			if(processo->io->tipo == 'D' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Disco) na fila de baixa prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    				troca_fila(processo, filaBaixaPrioridade, filaIoDisco);
    			}
    			else if(processo->io->tipo == 'F' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Fita) na fila de baixa prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    				troca_fila(processo, filaBaixaPrioridade, filaIoFita);
    			}
    			else if(processo->io->tipo == 'I' && processo->io->inicio == -1){
    				printf("7: Processo %i solicitou IO (Impressora) na fila de baixa prioridade\n", processo->pid);
    				processo->io->tipo = 0;
    					troca_fila(processo, filaBaixaPrioridade, filaIoImpressora);
    			}
    			else if(processo->prox != NULL){
    				PCB* base = processo->prox;
					base->ant = NULL;
					processo->prox = NULL;
					filaBaixaPrioridade->topo->prox = processo;
						filaBaixaPrioridade->topo = processo;
					filaBaixaPrioridade->base = base;
    			}
    			printf("8: Processo %i foi bloqueado com %is restantes\n", processo->pid, processo->tempo);
    		}
    	}
    	else{ // Todos os processos est√£o em IO
    		tempoPassado = passa_tempo_io(-1); // Passa tempo at√© um processo estar pronto para executar
    	}
    }
    printf("\nTempo total do simulador: %is\n", tempoTotal+tempoPassado);
    free(filaAltaPrioridade);
    free(filaBaixaPrioridade);
    free(filaIoDisco);
    free(filaIoFita);
    free(filaIoImpressora);
    */
	
	return 0;
}
