#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SZ_STD 100

char estados[SZ_STD];
char alfabeto[SZ_STD];
char alfa_pilha[SZ_STD];
char estados_finais[SZ_STD];
char metodo_aceitacao;

char estado_corrente;

char pilha[SZ_STD];
int topo;



char *transicoes[SZ_STD];

void carrega_automato(){
	register int i, j;
	char car;
	
	//Inicializa array
	for(i =0; i <SZ_STD; i++)
		transicoes[i] = NULL;
	
	i =0;
	
	//Estados
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
	estado_corrente = estados[0];
	
	//Alfabeto
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
	
	//Transições
	j =0;
	char temp[SZ_STD];
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
	
	
	for(i =0; transicoes[i] != NULL; i++)
		printf("%s\n", transicoes[i]);
	
	
};

//FUNÇÕES DE PILHA

void inicializa_pilha(){
	register int i;
	topo =0;
	pilha[topo] = 'Z';
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
	for(i = strlen(cadeia)-1; i >=0; i--)
		pilha[topo++] = cadeia[i];
};

char pop(){
	if(vazia())
		return '\0';
	return pilha[topo--];
};

int transicao(char estado, char simbolo, char topo_pilha){
	register int i;
	char temp[4];
	temp[0] = estado;
	temp[1] = simbolo;
	temp[2] = topo_pilha;
	temp[3] = '\0';
	
	for(i =0; transicoes[i] != NULL; i++){
		if((strncmp(temp, transicoes[i], 3) == 0)){
			push(&transicoes[i][5]);
			estado_corrente = transicoes[i][5];
			return 1;
		};
	};
	return 0;
};

int automato(char cadeia[]){
	register int i;
	
	for(i =0; i <strlen(cadeia); i++){
		if(!transicao(estado_corrente, cadeia[i], retorna_topo()))
			break;
	};
	switch (metodo_aceitacao){
		case 'P':
			if(vazia())
				return 1;
			break;
		case 'F': //DEPENDE DA LEITURA DOS ESTADOS FINAIS
				
	};
	return 0;
};


int main(void){
	inicializa_pilha();
	carrega_automato();

	return 1;
};
