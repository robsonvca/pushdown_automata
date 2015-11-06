#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 //MAX valerá para todos os arrays?
#define LEN_STR 15 //Esse realmente é o tamanho das palavras?
#define MAX_FNL_STTS 20 //Esse realmente é o máximo de estados finais?

/*Estrutura duma configuração do autômato*/
typedef struct{
	char curr_state, next_state;
	char token;
	char curr_stack, *stack_content;
	/*O conteúdo da pilha '*stack_content' nem sempre é um símbolo só,
	por isso é melhor representado numa string*/
} Auto_config;

/*Lê o arquivo apontado por fp, e carrega estadps, alfabeto, alfabeto de pilha, transições,
modo de aceitação, estados finais (se modo de aceitação for por estes).
Retorna 0 se ocorrer algum erro na leitura do arquivo, 1 se não. */
int load_automata(FILE *fp, char states[], char alphabet[], char alph_stack[], char *accept_mode, char final_stages[], Auto_config transitions[]){
	register int i;
	register char ch;

	ch = '\0'; //Reseta ch para validar entrada no for()
	
	/*Carrega estados*/
	for(i =0; (ch != '\n' && i <MAX); i++){
		ch = getc(fp);
		states[i] = ch;
	};
	
	ch = '\0';

	/*Carrega alfabeto*/
	for(i =0; (ch != '\n' && i <MAX); i++){
		ch = getc(fp);
		alphabet[i] = ch;
	};
	
	ch = '\0';

	/*Carrega alfabeto de pilha*/
	for(i =0; (ch != '\n' && i <MAX); i++){
		ch = getc(fp);
		alph_stack[i] = ch;
	};
	
	/*Carrega transições*/
	ch = getc(fp);
	for(i =0; (ch != '#' && i <MAX); i++){
		/*Apontador para string que armazenará o conteúdo da pilha em cada transição*/
		char *tmp, *str = (char *)malloc(sizeof(LEN_STR));
		tmp = str; //Salva posição inicial str pra retorno futuro
		
		/*fseek() sendo usado para avançar ponteiro do arquivo,
		ignorando caracteres desnecessários [espaços e >]*/

		transitions[i].curr_state = ch;
		fseek(fp, 1, SEEK_CUR); //Avançar 1 no cursor para ignorar [espaço]
	
		ch = getc(fp);
		transitions[i].token = ch;
		fseek(fp, 1, SEEK_CUR);
		
		ch = getc(fp);
		transitions[i].curr_stack = ch;
		fseek(fp, 3, SEEK_CUR); //Avançar 3 no cursor para ignorar [espaço > espaço]
		
		ch = getc(fp);
		transitions[i].next_state = ch;
		fseek(fp, 1, SEEK_CUR);
		
		/*Carrega o conteúdo da pilha numa string*/
		ch = getc(fp);
		while(ch != '\n'){
			*str = ch;
			str++;
			ch = getc(fp);
		};
		str = '\0'; //Finaliza a string
		transitions[i].stack_content = tmp;

		ch = getc(fp); //Carrega proxímo caractere pra validar condição de parada caso ch = '\n'

	};
	transitions[i].token = '#'; //Setando o array[n+1] como último do vetor com n transições
	
	fseek(fp, 1, SEEK_CUR); //Salta o '\n'
	
	*accept_mode = getc(fp);
	if(*accept_mode == 'F'){
		//final_stages = (char *)malloc(sizeof(char) *MAX_FNL_STTS);
		if(!final_stages)
			return 0;
		
		fseek(fp, 1, SEEK_CUR); //Salta o '\n'
		for(i =0; (ch != '\n' && i <MAX_FNL_STTS); i++){
			ch = getc(fp);
			final_stages[i] = ch;
		};
		final_stages[i] = '#'; //Setando o array[n+1] como último do vetor com n estados finais
	};

	if(ferror(fp)){	//Verifica se ocorreu erro em alguma operação em no arquivo
		perror("Error: ");
		return 0;
	};
	return 1;
};

//Função do próprio start_autômato
//Função auxiliar do autômato que escreve em arquivo de saída

/*Lê e retorna ponteiro para cadeias a serem analisadas pelo autômato.
Retorna NULL caso fim de arquivo de entrada '#', ou caso falha de alocação*/
char *read_string(FILE *fp){
	char *str = (char *)malloc(sizeof(char) *LEN_STR);
	
	if(getc(fp) == '#')
		return NULL;
	fseek(fp, -1, SEEK_CUR); //Retorna decrementa 1 no ponteiro do arquivo'
	
	if(str){
		register int i;
		register char ch = '\0';
		
		for(i =0; (ch != '\n' && i <MAX); i++){
			ch = getc(fp);
			str[i] = ch;
		};
		str[i] = '\0';

		return str;
	};
	
	return NULL;
};

/*FUNÇÃO!!! Prínta o que for printável*/
void show(FILE *fp, char states[], char alphabet[], char alph_stack[], char *accept_mode, char final_stages[], Auto_config transitions[]){
	register int i;
	
	for(i =0; (states[i-1] != '\n' && i <MAX); i++) printf("%c", states[i]);
	for(i =0; (alphabet[i-1] != '\n' && i <MAX); i++) printf("%c", alphabet[i]);
	for(i =0; (alph_stack[i-1] != '\n' && i <MAX); i++) printf("%c", alph_stack[i]);
	for(i =0; (transitions[i].token != '#' && i <MAX); i++){
		printf("%c ", transitions[i].curr_state);
		printf("%c ", transitions[i].token);
		printf("%c ", transitions[i].curr_stack);
		printf("> ");
		printf("%c ", transitions[i].next_state);
		printf("%s\n", transitions[i].stack_content);
	};
	printf("%c\n", *accept_mode);
	for(i =0; (final_stages[i] != '#' && i <MAX); i++)
		printf("%c", final_stages[i]);

	char *tmp;
	while((tmp = read_string(fp))) printf("%s", tmp);
};

int main(void){
	FILE *fp_in, *fp_out; //Apontador do arquivo de entrada e saída, respectivamente
	char states[MAX]; //Conjunto de estados
	char alphabet[MAX]; //Alfabeto de entrada
	char alph_stack[MAX]; //Alfabeto da pilha
	char accept_mode; //Modo de aceitação: F p/ estado final e P p/ esvaziamento de pilha
	char final_stages[MAX_FNL_STTS]; //Apontador para conjunto de estados finais
	Auto_config transitions[MAX]; //Conjunto de transições
		
	if(!(fp_in = fopen("input.txt", "r")) ||
	   !(fp_out = fopen("output.txt", "w"))){
		perror("Error: ");
		return 0;
	};
	
	if(!load_automata(fp_in, states, alphabet, alph_stack, &accept_mode, final_stages, transitions))
		return 0;
		
	show(fp_in, states, alphabet, alph_stack, &accept_mode, final_stages, transitions);
	
	fclose(fp_in);
	fclose(fp_out);
	return 0;
 };

/*
0 1 2 3 4
a c
Z C
0 a Z > 1 CCZ
0 a Z > 2 CZ
1 a C > 1 CCC
1 c C > 3 -
2 a C > 2 CC
2 c C > 3 -
3 c C > 3 -
3 - Z > 4 Z
#
F
4
aac
aacc
aacccc
#

0 1
a b c
Z C
0 a Z > 0 CCZ
0 a C > 0 CCC
0 b Z > 1 Z
0 b C > 1 C
1 c C > 1 -
1 - Z > 1 -
#
F
1
abcc
aabcc
#
*/
