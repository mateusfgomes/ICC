#include <stdio.h>
#include <stdlib.h>

/*
Mateus Ferreira Gomes

Instituto de Ciencias Matematicas e de Computacao da Universidade de Sao Paulo (Campus Sao Carlos)

No USP: 10734773
*/

//funcao flood-fill que passa preenchendo uma matriz paralela a' matriz mapa
void flood(int* count, int posicaox, int posicaoy, int pos[9][9], int map[9][9]){
	if (posicaox >= 0 && posicaox < 9 && posicaoy >= 0 && posicaoy < 9 && map[posicaox][posicaoy] != 1 && pos[posicaox][posicaoy] == 0){ 	
		*count = *count + 1;		
		pos[posicaox][posicaoy] = *count;
		flood(count, posicaox+1, posicaoy, pos, map);
		flood(count, posicaox-1, posicaoy, pos, map);
		flood(count, posicaox, posicaoy-1, pos, map);
		flood(count, posicaox, posicaoy+1, pos, map);
	}

	return;
}

int main(void){

	int i, j; //para rodar os for's
	int map[9][9]; //mapa
	int posicao[9][9];//matriz para ser percorrida
	int posicaox1, posicaox2, posicaoy1, posicaoy2;  //x's e y's das posicoes de 1 e 2
	int** posrec = NULL; //ponteiro para alocacao de uma matriz na memoria heap
	int Nrec; //numero de recursos
	int P = 0; //numero de celulas transponiveis
	int contador; //numero que aumenta enquanto o flood-fill percorre a matriz

	//zerando a matriz da posicao de 1
	for (i = 0; i < 9; i++){
		for (j = 0; j < 9; j++){
			posicao[i][j] = 0;
		}
	}


	//recebendo a matriz do mapa
	for (i = 0; i < 9; i++){
		for (j = 0; j < 9; j++){
			scanf("%d", &map[i][j]);
		}
	}

	//pegando as posicoes dos jogadores
	scanf("%d %d", &posicaox1, &posicaoy1);
	scanf("%d %d", &posicaox2, &posicaoy2);
	scanf("%d", &Nrec); //escaneando o numero de recursos

	//inicializando e1 e e2,
	double e1;
	double e2;

	//alocando uma matriz na memoria heap para armazenar a localizacao dos recursos
	posrec = (int**)calloc(Nrec, sizeof(int*));
	
	for (i = 0; i < Nrec; i++){																
		posrec[i] = (int*) calloc(2, sizeof(int));
	}
	
	//armazenando a posicao do recurso na memoria
	for (i = 0; i < Nrec; i++){
		for (j = 0; j < 2; j++){
			scanf("%d", &posrec[i][j]);
		}
	}

	for(int i = 0; i < 9; i++){
		for (j = 0; j < 9; j++){
			if (map[i][j] == 0){
				P++;
			}
		}
	}
	
	contador = 0;
	
	//calculando as N distancias ate os n recursos possiveis (essa funcao flood-fill se executa uma vez so' de modo a preencher uma matriz paralela
	//a matriz mapa com a distancia da base do jogador ate qualquer ponto do mapa considerando a movimentacao dada (SUL, NORTE, OESTE, LESTE))
	flood(&contador, posicaox1, posicaoy1, posicao, map);
	
	//inicializando a variavel que armazenara' a parte do somatorio do problema
	double somatorio = 0;
	//inicializando a variavel que armazenara' (retirando da matriz ja percorrida pelo flood-fill) as distancias do jogador ate os N recursos
	int e = 0;

	//realizando o somatorio da equacao
	for (int i = 0; i < Nrec; i++){
		//realizando o numerador da fracao do somatorio
		e = posicao[posrec[i][0]][posrec[i][1]];
		//somando
		somatorio += e / (double)P;
	}	

	//realizando o calculo da expressao dada, para o jogador 1
	e1 = (1 / (double) Nrec)*somatorio;
	printf("%lf\n", e1);

	//zerando a matriz posicao percorrida pelo flood-fill anterior de modo a prepara'-la para o proximo flood-fill
	for(int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			posicao[i][j] = 0;
		}
	}

	//resetando como 0 a variavel para recalcular o posicionamento
	contador = 0;

	//o mesmo que ocorreu com o jogador 1 ate a linha 129
	flood(&contador, posicaox2, posicaoy2, posicao, map);

	somatorio = 0;
	e = 0;

	for (int i = 0; i < Nrec; i++){
		e = posicao[posrec[i][0]][posrec[i][1]];
		somatorio += e / (double)P;
	}	

	e2 = (1 / (double) Nrec)*somatorio;
	
	printf("%lf\n", e2);

	//testando os e's para ver qual jogador possui vantagem
	if (e2 > e1){
		printf("O jogador 1 possui vantagem\n");
	} else if(e1 > e2){
		printf("O jogador 2 possui vantagem\n");
	}
	else{
		printf("O mapa eh balanceado\n");
	}

	//liberando o espaco alocado na memoria HEAP para armazenar a posicao dos recursos
	for (i = 0; i < Nrec; i++){																
		free(posrec[i]);
	}
	
	free(posrec);

	return 0;
}
