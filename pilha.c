#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SZ_STD 100

char estados[SZ_STD];
char alfabeto[SZ_STD];
char alfa_pilha[SZ_STD];
char *transicoes[SZ_STD];
char estado_corrente;
char metodo_aceitacao;
char estados_finais[SZ_STD];
char *cadeias[SZ_STD];

char pilha[SZ_STD];
int topo;


/////////////////////////////////////
//////    FUNÇÕES DA PILHA   ///////
///////////////////////////////////

void inicializa_pilha(){
	register int i;
	topo =0;
	pilha[topo] = alfa_pilha[0];
	for(i =1; i <SZ_STD; i++)
		pilha[i] = '-';
};

int vazia(){
	return (topo <0)? 1 : 0;
};

char retorna_topo(){
	if(vazia())
		return '\0';
	return pilha[topo];
};

void push(char *cadeia){
	int i;
	for(i = strlen(cadeia); i >=0; i--)
		if(isalpha(cadeia[i])) //Impede impilhamento de '-' tambem
			pilha[++topo] = cadeia[i];
};

char pop(){
	if(vazia())
		return '\0';
	char temp = pilha[topo];
	pilha[topo--] =  '-';
	return temp;
};

void imprime_pilha(){
	int i;
	if(vazia())
		printf("4pilha vazia\n");
	printf("\nPilha-> ");
	for(i =topo; i>=0; i--)
		printf("%c", pilha[i]);
	printf("\n");
};

/////////////////////////////////////
//////  FUNÇÕES DO AUTOMATO  ///////
///////////////////////////////////

void carrega_automato(){
	register int i, j;
	char car, temp[SZ_STD];
	
	//INICIALIZA ARRAYS DE PONTEIROS
	for(i =0; i <SZ_STD; i++)
		transicoes[i] = cadeias[i] = NULL;
	
	//ESTADOS
	i =0;
	car = getchar();
	while(car != '\n'){
		if(isalnum(car)){
			estados[i] = car;
			i++;
		};
		car = getchar();
	};
	estados[i] = '\0';
	i =0;
	
	//ESTADO INICIAL
	estado_corrente = estados[0];
	
	//ALFABETO
	car = getchar();
	while(car != '\n'){
		if(isalpha(car)){
			alfabeto[i] = car;
			i++;
		};
		car = getchar();
	};
	estados[i] = '\0';
	i =0;
	
	////Alfabeto de pilha
	car = getchar();
	while(car != '\n'){
		if(isalpha(car)){
			alfa_pilha[i] = car;
			i++;
		};
		car = getchar();
	};
	estados[i] = '\0';
	i =0;
	
	//TRANSIÇÕES
	j =0;
	do{
		car = getchar();
		while((car != '\n') && (car != '#')){
			if(car != ' '){
				temp[i] = car;
				i++;
			};
			car = getchar();
		};
		if((car != '#')){
			temp[i] = '\0';
			transicoes[j] = malloc(strlen(temp));
			strcpy(transicoes[j], temp);
			j++;
			i =0;
		};
	}while(car != '#');
	
	//CARREGA METODO DE ACEITAÇAO
	car = getchar(); //Ignorando '\n'
	metodo_aceitacao = getchar();
	car = getchar(); //Ignorando '\n'
	
	//CARREGA ESTADOS FINAIS, SE ACEITAÇÃO FOR 'F'	
	if(metodo_aceitacao == 'F'){
		car = getchar(); //Ignorando '\n'
		while(car != '\n'){
			if(isalnum(car)){
				estados_finais[i] = car;
				i++;
			};
			car = getchar();
		};
		estados_finais[i] = '\0';
		i =0;
	};
	//printf("\nestados finais\n");
	//for(i =0; estados_finais[i] != '\0'; i++)
		//printf("%c", estados_finais[i]);
	
	//CARREGA CADEIAS
	j =0;
	do{
		car = getchar();
		while((car != '\n') && (car != '#')){
			if(car != ' '){
				temp[i] = car;
				i++;
			};
			car = getchar();
		};
		if((car != '#')){
			temp[i] = '\0';
			cadeias[j] = malloc(strlen(temp));
			strcpy(cadeias[j], temp);
			j++;
			i =0;
		};
	}while(car != '#');
};

int transicao(char estado, char simbolo, char topo_pilha){
	register int i;
	char temp[4];
	temp[0] = estado;
	temp[1] = simbolo;
	temp[2] = topo_pilha;
	temp[3] = '\0';
	
	for(i =0; transicoes[i] != NULL; i++){
		//printf("(%c,%c,%c)->", estado, simbolo, topo_pilha);
		if((strncmp(temp, transicoes[i], 3) == 0)){
			pop();
			push(&transicoes[i][5]);
			estado_corrente = transicoes[i][4];
			//printf(" (%c,%s)\n", transicoes[i][4], &transicoes[i][5]);
			return 1;
		};
		//printf("(NULO)\n");
		//printf("\tstrcncomp(%s, %s, 3)\n", temp, transicoes[i]);
	};
	return 0;
};

int automato(char cadeia[]){
	register int i;
	
	inicializa_pilha();
	estado_corrente = estados[0]; // Configura estado inicial
	
	strcat(cadeia, "-"); // Adiciona cadeia vazio no final da cadeia;
	for(i =0; transicao(estado_corrente, cadeia[i], retorna_topo()); i++);
		//imprime_pilha();

	switch (metodo_aceitacao){
		case 'P':{
			if(vazia())
				return 1;
			break;
		};
		case 'F':{
			//printf("estado cprrente: %c\n", estado_corrente);
			for(i =0; estados_finais[i] != '\0'; i++)
				if(estado_corrente == estados_finais[i])
					return 1;
		};
	};
	return 0;
};



int main(void){
	char texto_saida[SZ_STD];
	int i;
	
	carrega_automato();
	
	for(i =0; cadeias[i] != NULL; i++){
		strcpy(texto_saida, "Cadeia ");
		strcat(texto_saida, cadeias[i]);

		if(automato(cadeias[i]))
			strcat(texto_saida, " aceita por ");
		else		
			strcat(texto_saida, " rejeitada por ");
			
		if(metodo_aceitacao == 'F')
			strcat(texto_saida, "estados finais");		
		else if(metodo_aceitacao == 'P')
			strcat(texto_saida, "pilha");
			
		printf("%s\n", texto_saida);
	};
	
	return 1;
};
