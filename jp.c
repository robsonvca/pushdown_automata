#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SZ_STD 100

char estados[SZ_STD];
char alfabeto[SZ_STD];
char alfa_pilha[SZ_STD];

int estado_corrente;

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

int main(void){
	int i;

	carrega_automato();

	return 1;
};
