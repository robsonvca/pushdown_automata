#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SZ_GROUP 100 //Cardinalidade padrão para os conjuntos
#define SZ_STR 256 //Comprimento padrão das cadeias

//Estrutura duma configuração do autômato
typedef struct{
	char curr_state, next_state;
	char token;
	char curr_stack, *stack_content;
	/* O conteúdo da pilha '*stack_content' nem sempre é um símbolo só,
	 * por isso é melhor representado numa string. */
	 
} T_Config;

//Estrutura da pilha do autômato
typedef struct{
	int top;
	char data[SZ_GROUP];
} T_Stack;

//VARIAVÉIS GLOBAIS
FILE *fp_in, *fp_out; //Apontador do arquivo de entrada e saída, respectivamente
char states[SZ_GROUP]; //Conjunto de estados. A primeira posição do array representa o estado inicial
char alphabet[SZ_GROUP]; //Alfabeto de entrada. A primeira posição do array representa o símbolo inicial da pilha
char alph_stack[SZ_GROUP]; //Alfabeto da pilha
char accept_mode; //Modo de aceitação: F p/ estado final e P p/ esvaziamento de pilha
char final_stages[SZ_GROUP]; //Conjunto de estados finais
T_Config settings[SZ_GROUP]; //Conjunto de transições

/* Lê o arquivo apontado por fp_in, e carrega estadps, alfabeto, alfabeto de pilha, transições,
 * modo de aceitação, estados finais (se modo de aceitação for por estes).
 * Retorna 0 se ocorrer algum erro na leitura do arquivo, 1 se não. */
int load_automata(void){
	register int i;
	register char ch;

	ch = '\0'; //Reseta ch para validar entrada no for()
	
	//Carrega estados
	for(i =0; (ch != '\n' && i <SZ_GROUP); i++){
		ch = getc(fp_in);
		states[i] = ch;
	};
	
	ch = '\0';

	//Carrega alfabeto
	for(i =0; (ch != '\n' && i <SZ_GROUP); i++){
		ch = getc(fp_in);
		alphabet[i] = ch;
	};
	
	ch = '\0';

	//Carrega alfabeto de pilha
	for(i =0; (ch != '\n' && i <SZ_GROUP); i++){
		ch = getc(fp_in);
		alph_stack[i] = ch;
	};
	
	//Carrega transições
	ch = getc(fp_in);
	for(i =0; (ch != '#' && i <SZ_GROUP); i++){
		//Apontador para string que armazenará o conteúdo da pilha em cada transição
		char *tmp, *str = (char *)malloc(sizeof(SZ_STR));
		tmp = str; //Salva posição inicial str pra retorno futuro
		
		/*
		 * fseek() sendo usado para avançar ponteiro do arquivo,
		 * ignorando caracteres desnecessários [espaços e >] */
		settings[i].curr_state = ch;
		fseek(fp_in, 1, SEEK_CUR); //Avançar 1 no cursor para ignorar [espaço]
	
		ch = getc(fp_in);
		settings[i].token = ch;
		fseek(fp_in, 1, SEEK_CUR);
		
		ch = getc(fp_in);
		settings[i].curr_stack = ch;
		fseek(fp_in, 3, SEEK_CUR); //Avançar 3 no cursor para ignorar [espaço > espaço]
		
		ch = getc(fp_in);
		settings[i].next_state = ch;
		fseek(fp_in, 1, SEEK_CUR);
		
		//Carrega o conteúdo da pilha numa string
		ch = getc(fp_in);
		while(ch != '\n'){
			*str = ch;
			str++;
			ch = getc(fp_in);
		};
		str = '\0'; //Finaliza a string
		settings[i].stack_content = tmp;

		ch = getc(fp_in); //Carrega proxímo caractere pra validar condição de parada caso ch = '\n'

	};
	settings[i].token = '#'; //Setando o array[n+1] como último do vetor com n transições
	
	fseek(fp_in, 1, SEEK_CUR); //Salta o '\n'
	
	accept_mode = getc(fp_in);
	if(accept_mode == 'F'){
		fseek(fp_in, 1, SEEK_CUR); //Salta o '\n'
		for(i =0; (ch != '\n' && i <SZ_GROUP); i++){
			ch = getc(fp_in);
			final_stages[i] = ch;
		};
		final_stages[i] = '#'; //Setando o array[n+1] como último do vetor com n estados finais
	};

	if(ferror(fp_in)){	//Verifica se ocorreu erro em alguma operação em no arquivo
		perror("Error: ");
		return 0;
	};
	return 1;
};

/* Função do próprio start_autômato
 * Função auxiliar do autômato que escreve em arquivo de saída*/
 
/* Lê e retorna ponteiro para cadeias a serem analisadas pelo autômato.
 * Retorna NULL caso fim de arquivo de entrada '#', ou caso falha de alocação */
char *read_string(void){
	char *str = (char *)malloc(sizeof(SZ_STR));
	
	if(getc(fp_in) == '#')
		return NULL;
	fseek(fp_in, -1, SEEK_CUR); //Retorna decrementa 1 no ponteiro do arquivo'
	
	if(str){
		register int i;
		register char ch = '\0';
		
		for(i =0; (ch != '\n' && i <SZ_GROUP); i++){
			ch = getc(fp_in);
			str[i] = ch;
		};
		str[i] = '\0';

		return str;
	};
	
	return NULL;
};

//Inicializa pilha
void init_stack(T_Stack *stack){
	register int i;
	
	stack->top =-1;
	stack->data[0] = 'Z'; //Símbolo inicial da pilha
	for(i =1; i <SZ_GROUP; i++)
		stack->data[i] = '\0';
};

//Empilha token
void push(T_Stack *stack, char token){
};

//Retorna 1 se a pilha estiver vazia.
int empty_stack(T_Stack *stack){
	return (stack->top <0)? 1 : 0;
};

//Desempilha retornando o símbolo do topo
char pop(T_Stack *stack){
	if(!empty_stack(stack))
		return '-'; //Símbolo de vazio
		
	return stack->data[stack->top--];
};

//Consulta topo da pilha. Retorna valho do topo
char query(T_Stack *stack){
	if(!empty_stack(stack))
		return '-';
	return stack->data[stack->top];
};
/*
 * IMPLEMENTAR transition()
 * CONSIDERAR TRANSIÇÕES EM VAZIO
 * 
 * Verificar se a configuração passada como parâmetro gera uma movimentação
 * se sim, atuliza stack e retorna o próximo estado (next_stage)
 * 
*/

int transition(int curr_state, char token, T_Stack *stack){
	register int i;

	for(i =0; i <SZ_GROUP; ++i){
		if((settings[i].curr_state == curr_state) &&
		   (settings[i].token == token) &&
		   (settings[i].curr_stack == pop(stack))){
		};
	};
	return -1;
};

int automata(char *string){
	register int i;
	int curr_state, length_str;
	T_Stack stack;
	
	init_stack(&stack); //Inicializa pilha
	curr_state = 0; //Re-Inicia cursor
	length_str = strlen(string);

	 //POR ESTADO FINAL
	 for(i =0; i <length_str; i++){
		curr_state = transition(curr_state, string[i], &stack);

		if(curr_state <0)
			return 0; //Nâo houve mais movimentação. Nâo reconheceu a cadeia
	 };
/*
 * Dá pra usar switch case aqui com F e P

* Se a cadeia foi completamente consumida (i == length_str)
* Se curr_state é estado final
* RECONHECEU A CADEIA
*/
	 if(i == length_str){
		for(i =0; i <SZ_GROUP; i++){
			if(curr_state == final_stages[i])
				return 1; //curr_state é um dos estados finais
		};
	 };

	 return 0;
};

/* Chama n-x automato() com a cadeia a ser analisada.
 * Grava string formatada no arquivo de saídas. */
void start(void){
	char *string = NULL;
	char write_file[SZ_STR +43];
	
	while((string = read_string())){
		strcpy(write_file, "Cadeia ");
		strcat(write_file, string);
		write_file[strlen(write_file) -1] = ' '; //Removendo o '\n' do final da string

		if(automata(string))
			strcat(write_file, "aceita ");
		else
			strcat(write_file, "rejeitada ");
		
		if(accept_mode == 'F')
			strcat(write_file, "por estado final\n");
		else
			strcat(write_file, "por esvaziamento de pilha\n");
		
		fputs(write_file, fp_out); //Gravando write_file em arquivo
	};
};

//FUNÇÃO TESTE!!! Prínta o que for printável
void show(void){
	register int i;
	
	for(i =0; (states[i-1] != '\n' && i <SZ_GROUP); i++) printf("%c", states[i]);
	for(i =0; (alphabet[i-1] != '\n' && i <SZ_GROUP); i++) printf("%c", alphabet[i]);
	for(i =0; (alph_stack[i-1] != '\n' && i <SZ_GROUP); i++) printf("%c", alph_stack[i]);
	for(i =0; (settings[i].token != '#' && i <SZ_GROUP); i++){
		printf("%c ", settings[i].curr_state);
		printf("%c ", settings[i].token);
		printf("%c ", settings[i].curr_stack);
		printf("> ");
		printf("%c ", settings[i].next_state);
		printf("%s\n", settings[i].stack_content);
	};
	printf("%c\n", accept_mode);
	for(i =0; (final_stages[i] != '#' && i <SZ_GROUP); i++)
		printf("%c", final_stages[i]);

	char *tmp;
	while((tmp = read_string())) printf("%s", tmp);
};

int main(void){
	//Tentar carregar fp_in e fp_out. Sai do programa caso erro
	if(!(fp_in = fopen("entrada.txt", "r")) ||
	   !(fp_out = fopen("saida.txt", "w"))){
		perror("Error: ");
		return 0;
	};
	
	if(!load_automata()){
		printf("Error ao carregar autômato\n");
		return 0;
	};
		
	//show();
	start();
	
	fclose(fp_in);
	fclose(fp_out);
	return 0;
 };
