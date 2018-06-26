#include <stdio.h>
#include <stdlib.h>

/*
Mateus Ferreira Gomes
Nº USP: 10734773

Instituto de Ciencias Matematicas e de Computacao - Universidade de Sao Paulo

Trabalho 4
Programa que calcula se ha' ou nao interseccao entre dois planos, duas retas ou plano e reta dadas suas equacoes,
alem disso imprime a matriz escalonada que auxilia a chegar a conclusao.
*/

//Definindo a struct do pivo(numero do canto superior esquerdo, que se altera conforme a recursao), 
//com "a" como numerador e "b" como denominador, essa ultima logica vale para o trabalho todo.
typedef struct pivo{

	int a;
	int b;

}pivo;

//struct para pegar os coeficientes (a, b, c, d) das equacoes. Sendo "a", o numerador, e "b" o denominador desses numeros.
typedef struct fracao{

	int a;
	int b;

}fracao;

//struct para repor a primeira linha da matriz quando ela for escalonada, pois escalonamos tudo na mesma
typedef struct salvar{

	int a;
	int b;

}salvar;

//funcao swap() para trocar as linhas da matriz caso necessario, numero a numero
void swap(int* a0, int* a1, int* b0, int* b1){

	//variavel temporaria para auxiliar na troca de numeros
	int temp;

	temp = *a0;
	*a0 = *a1;
	*a1 = temp;

	temp = *b0;
	*b0 = *b1;
	*b1 = temp;
}

//para auxiliar a funcao mmc abaixo com a mudanca de denominador
void swap_den(int* a, int* b){

	int prov;

	prov = *a;
	*a = *b;
	*b = prov;

}

//funcao para calcular o mmc dos denominadores de modo a subtrair os numeros
int mmc (int a, int b){

	//zerando o mmc
	int mmc = 0;
	//criando uma variavel temporaria para auxiliar no calculo
	int temp2;

	//inversao para facilitar
	if(b > a){
		swap_den(&a, &b);
	}

	//se forem multiplos, o mmc e' o maior
	if(a % b == 0){
		mmc = a;
		return mmc;
	}

	//se nao forem, procura o mmc
	for (int i = 2; i <= b; i++){
		temp2 = a*i;
		if (temp2 % b == 0){
			mmc = temp2;
		}
	}
	return mmc;
}

//funcao recursiva para realizar o escalonamento da matriz
void escalonamento(int linha, int coluna, fracao** coords, int linha_inicial, int coluna_inicial){

	//inicializando o mmc
	int minimo = 0;

	//checando as condicoes para executar a funcao
	if ((coluna_inicial < 3) && (linha_inicial < linha-1)){
		for (int i = (linha_inicial+1); i < linha; i++){
			//checando se e' necessario realizar o swap (p/ tornar o pivo != 0), se for, entra
			if (coords[linha_inicial][coluna_inicial].a == 0 && coords[i][coluna_inicial].a != 0){
				//realizando troca numero a numero
				for(int j = coluna_inicial; j < coluna; j++)
					swap(&coords[linha_inicial][j].a, &coords[i][j].a, &coords[linha_inicial][j].b, &coords[i][j].b);
			}		
		}

		pivo pivo;//declarando a variavel pivo numa struct

		//recebendo os numeros do pivo
		pivo.a = coords[linha_inicial][coluna_inicial].a;
		pivo.b = coords[linha_inicial][coluna_inicial].b;

		//declarando a variavel para salvar a linha inicial antes de fazer o escalonamento
		salvar salvar[4];

		//salvando a linha inicial
		for (int j = coluna_inicial; j < coluna; j++){
			salvar[j].a = coords[linha_inicial][j].a;
			salvar[j].b = coords[linha_inicial][j].b;
		}
		
		//realizando o calculo do numero a ser colocado na matriz em escalonamento
		for (int i = linha_inicial+1; i < linha; i++){
			//declarando a variavel que realiza o calculo da parte de multiplicacao
			int multiplicador_a = (coords[i][coluna_inicial].a * pivo.b);
			int multiplicador_b = (coords[i][coluna_inicial].b * pivo.a);
			for (int j = coluna_inicial; j < coluna; j++){
				//multiplicando a linha inicial
				coords[linha_inicial][j].a = coords[linha_inicial][j].a * multiplicador_a; 
				coords[linha_inicial][j].b = coords[linha_inicial][j].b * multiplicador_b;
				//fazendo o calculo do mmc
				minimo = mmc(coords[linha_inicial][j].b, coords[i][j].b);
				//fazendo a conta do numerador
				coords[linha_inicial][j].a = (minimo/coords[linha_inicial][j].b) * coords[linha_inicial][j].a;
				coords[i][j].a = (minimo/coords[i][j].b) * coords[i][j].a;
				//recebendo o denominador com o mmc
				coords[i][j].b = minimo;
				coords[linha_inicial][j].b = minimo;
				//com o mesmo denominador, fazendo a subtracao do numerador
				coords[i][j].a = coords[i][j].a - coords[linha_inicial][j].a;
				//recolocando as linhas iniciais
				coords[linha_inicial][j].a = salvar[j].a;
				coords[linha_inicial][j].b = salvar[j].b;
			}
		}

		//verificando se os objetos se intersectam
		for (int i = 0; i < linha; i++){
			if (coords[i][0].a == 0 && coords[i][1].a == 0 && coords[i][2].a == 0 && coords[i][3].a != 0){
				printf("nao\n");
				return;
			}
		}
		//realizando a recursao
		escalonamento(linha, coluna, coords, linha_inicial+1, coluna_inicial+1);
	}
	//caso a recursao chegue ao final e nao ha' a = b = c = 0 e d != 0, logo se intersectam
	else{
		printf("sim\n");
		return;
	}
}

int main(void){

	int linha, coluna;//linhas da matriz
	char tipo1, tipo2;//usadas para ver se e' reta ou plano
	fracao** coords;//declarando uma matriz de struct
	int linha_inicial = 0;//linha que vai comecar a realizar o escalonamento 
	int coluna_inicial = 0;//coluna que vai comecar a realizar o escalonamento

	//escaneando linha e coluna da matriz
	scanf("%d%d", &linha, &coluna);
	//verificando se o tipo 1 e' plano ou reta
	scanf(" %c", &tipo1);

	//alocando a matriz de struct na heap
	coords = (fracao**) malloc (linha* sizeof(fracao*));
	for (int i = 0; i < linha; i++){
		coords[i] = (fracao*) malloc ((coluna) * sizeof(fracao));
	}

	//se for reta, entra para escanear as coordenadas
	if (tipo1 == 'r'){
		for (int i = 0; i < 2; i++){
			for (int j = 0; j < (coluna); j++){
				scanf("%d%d", &coords[i][j].a, &coords[i][j].b);
			}
		}
	}
	//se for plano, entra aqui para escanear as coordenadas
	else if(tipo1 == 'p'){
		for (int j = 0; j < (coluna); j++){
			scanf("%d%d", &coords[0][j].a, &coords[0][j].b);
		}
	}

	//verificando se o tipo 2 e' reta ou plano
	scanf(" %c", &tipo2);

	//se o tipo 1 for reta e o 2 tb, entra para escanear as coordenadas do tipo 2
	if (tipo2 == 'r' && tipo1 == 'r'){
		for (int i = 2; i < 4; i++){
			for (int j = 0; j < (coluna); j++){
				scanf("%d%d", &coords[i][j].a, &coords[i][j].b);
			}
		}
	} 

	//se o tipo 1 for plano e o tipo 2 tb, entra para escanear as coordenadas do tipo 2
	else if(tipo2 == 'p' && tipo1 == 'p'){
		for (int j = 0; j < (coluna); j++){
			scanf("%d%d", &coords[1][j].a, &coords[1][j].b);
		}	
	}

	//se o tipo 1 for reta e o 2 for plano, entra para escanear as coordenadas do tipo 2 
	else if(tipo1 == 'r' && tipo2 == 'p'){
		for (int j = 0; j < (coluna); j++){
			scanf("%d%d", &coords[2][j].a, &coords[2][j].b);
		}
	}

	//se o tipo 1 for plano e o tipo 2 reta, entra para escanear as coordenadas do tipo 2 
	else if(tipo1 == 'p' && tipo2 == 'r'){
		for (int i = 1; i < 3; i++){
			for (int j = 0; j < (coluna); j++){
				scanf("%d%d", &coords[i][j].a, &coords[i][j].b);
			}
		}
	}

	//chamando a funcao para escalonar a matriz escaneada
	escalonamento(linha, coluna, coords, linha_inicial, coluna_inicial);

	//printando a matriz escalonada com simplificacoes
	for (int i = 0; i < linha; i++){
		for (int j = 0; j < (coluna); j++){
			//se a divisao resultar denominador 1, divide
			if (coords[i][j].a % coords[i][j].b == 0){
				coords[i][j].a = coords[i][j].a/coords[i][j].b;
				coords[i][j].b = coords[i][j].b/coords[i][j].b;
				printf("%d\t", coords[i][j].a);
			}
			//se nao, so' printa
			else{
				printf("%d/%d\t", coords[i][j].a, coords[i][j].b);

			}	
		}
		printf("\n");
	}

	//liberando a memoria alocada na heap
	for (int i = 0; i < linha; i++){
		free(coords[i]);
	}

	free(coords);
	
	return 0;

}

