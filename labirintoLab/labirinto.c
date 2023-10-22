/*Algoritmo do labirinto, baseado no algorítmo de "Busca em Profundidade", feito por Pedro Gabriel de Morais Ribeiro, aluno do 2º semestre do curso de Ciência da Computação, período 2019.2.*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

FILE * arq;

void labirinto(int l, int c);
void gerarLab(int l, int c, int * lab, int pos_l, int pos_c);
void leituraLab(int l, int c);
void percorrerLab(int l, int c, char * lab, int pos_l, int pos_c);
void embaralhar(int * lado);

int main(void) {
	int l, c; //Varíaveis para número de linhas e colunas

	srand(time(NULL)); //Seed para produção de números aleatórios na função rand

	printf("Digite dois valores maiores que 10 para a altura e largura do labirinto, respectivamente: ");
	scanf("%d", &l);
	scanf("%d", &c);

	while(l < 10 || c < 10){ //Condições para altura e largura do labirinto, que devem ser ambas maiores que 10
		printf("Entrada invalida. Por favor, insira um valor valido para altura e largura do labirinto (maior ou igual a 10): ");
		scanf("%d", &l);
		scanf("%d", &c);
	}

	labirinto(l, c); //Função onde o labirinto será criado e printado em um arquivo .txt
	leituraLab(l, c); //Função onde o labirinto será printado e percorrido, além de ter sua solução printada na tela e no arquivo .txt

	return 0;
}

void labirinto(int l, int c) {
	int * matLab = (int*)malloc((2 * l + 1) * (2 * c + 1) * sizeof(int));
	int inicio_l = 2 * (rand() % l) + 1; //Valor aleatório da linha de onde começará a criação do labirinto
	int inicio_c = 2 * (rand() % c) + 1; //Valor aleatório da coluna de onde começará a criação do labirinto

	for(int i = 0; i < 2 * l + 1; i++) {
		for(int j = 0; j < 2 * c + 1; j++) {
			matLab[(2 * c + 1) * i + j] = 1; //A matriz "lab" será primeiramente preenchida por "paredes"
		}
	}

	matLab[(2 * c + 1) * inicio_l + inicio_c] = 0; //Posição onde iniciará a geração do labirinto

	gerarLab(l, c, matLab, inicio_l, inicio_c); //Aberta a função "gerarLab", na qual irá percorrer a matriz, substituindo parte das "paredes" por "espaços vazios"

	matLab[(2 * c + 1)] = 0; //Marcação da entrada e saída na matriz
	matLab[(2 * c + 1) * (2 * l - 1) + 2 * c] = 0;

	arq = fopen("labirinto.txt", "w"); //O arquivo "labirinto.txt" é criado para inserir a matriz do labirinto, sendo impresso em forma de caracteres específicos para diferenciar as paredes e espaços vazios.

	for(int i = 0; i < (2 * l + 1); i++) {
		for(int j = 0; j < (2 * c + 1); j++) {
			if(matLab[(2 * c + 1) * i + j] == 0){
				fprintf(arq, " ");
			}
			else {
				if(j % 2 == 0) {
					if(i % 2 == 0)
						fprintf(arq, "+");
					else
						fprintf(arq, "|");
				}
				else {
					fprintf(arq, "-");
				}
			}
		}
		fprintf(arq, "\n");
	}
	fclose(arq);
}

void gerarLab(int l, int c, int * lab, int pos_l, int pos_c) {
	int lado[4]; //0 - CIMA, 1 - ESQUERDA, 2 - BAIXO, 3 - DIREITA

	for(int i = 0; i < 4; i++) //Primeiramente, preenche-se a matriz "lado", que indicará os lados que deverão ser percorridos, de 0 à 3.
		lado[i] = i;

	embaralhar(lado); //Embaralham-se os valores da matriz, de maneira semelhante à distribuição em tabelas hash

	for(int i = 0; i < 4; i++) { //Loop para testar cada uma das possibilidades de percorrimento do labirinto
		switch(lado[i]) {
		case 0: //CIMA
			if(pos_l != 1) { //Caso a posição em relação à altura esteja uma linha abaixo do limite superior, o labirinto não será percorrido para cima para não ultrapassar os limites (parede superior).
				if(lab[(2 * c + 1) * (pos_l - 2) + pos_c] != 0){
					lab[(2 * c + 1) * (pos_l - 2) + pos_c] = 0; //É deixado um espaço em branco indicando o caminho do labirinto.
					lab[(2 * c + 1) * (pos_l - 1) + pos_c] = 0;
					gerarLab(l, c, lab, pos_l - 2, pos_c);
				}
			}
			break;

		case 1: //ESQUERDA
			if(pos_c != 1) { //Caso a posição em relação à largura esteja uma coluna à direita dos limites do labirinto, ele não será percorrido para a esquerda para não ultrapassar os limites (parede esquerda).
				if(lab[(2 * c + 1) * pos_l + (pos_c - 2)] != 0){
					lab[(2 * c + 1) * pos_l + (pos_c - 2)] = 0;
					lab[(2 * c + 1) * pos_l + (pos_c - 1)] = 0;
					gerarLab(l, c, lab, pos_l, pos_c - 2);
				}
			}
			break;

		case 2: //BAIXO
			if(pos_l != 2 * l - 1) { //Caso a posição em relação à altura esteja uma linha acima do limite inferior, o labirinto não será percorrido para baixo para não ultrapassar os limites (parede inferior).
				if(lab[(2 * c + 1) * (pos_l + 2) + pos_c] != 0){
					lab[(2 * c + 1) * (pos_l + 2) + pos_c] = 0; //É deixado um espaço em branco indicando o caminho do labirinto.
					lab[(2 * c + 1) * (pos_l + 1) + pos_c] = 0;
					gerarLab(l, c, lab, pos_l + 2, pos_c);
				}
			}
			break;

		case 3: //DIREITA
			if(pos_c != 2 * c - 1) { //Caso a posição em relação à largura esteja uma coluna à esquerda dos limites do labirinto, ele não será percorrido para a direita para não ultrapassar os limites (parede direita).
				if(lab[(2 * c + 1) * pos_l + (pos_c + 2)] != 0){
					lab[(2 * c + 1) * pos_l + (pos_c + 2)] = 0;
					lab[(2 * c + 1) * pos_l + (pos_c + 1)] = 0;
					gerarLab(l, c, lab, pos_l, pos_c + 2);
				}
			}
			break;
		}
	}
}

void leituraLab(int l, int c) {
	arq = fopen("labirinto.txt", "rb"); //O arquivo do labirinto é aberto em modo leitura
	fseek(arq, 0, SEEK_END);
	unsigned int len = ftell(arq);
	fseek(arq, 0, SEEK_SET);
	char * lab = (char *)malloc(len);

	if(lab) {
		fread(lab, 1, len, arq);
	}

	printf("Labirinto:\n%s", lab);

	fclose(arq); //O arquivo é fechado para ser aberto novamente em modo "adicionar"

	arq = fopen("labirinto.txt", "a");

	percorrerLab(l, c, lab, 1, 0); //A função que percorre o labirinto em busca da saída é iniciada

	fprintf(arq, "\n"); //O resultado da função "percorrerLab" é impresso no arquivo "labirinto.txt"

	fprintf(arq, "%s", lab);

	fclose(arq);
}

void percorrerLab(int l, int c, char * lab, int pos_l, int pos_c) {
	//A função "percorrerLab" funciona de maneira semelhante à função que cria o labirinto
	lab[(2 * c + 2) * pos_l + pos_c] = '#'; //A posição atual recebe o caractere "#", que marca o caminho percorrido para a saída

	printf("Resolucao do labirinto:\n%s", lab); //O percorrimento do labirinto é impresso na tela toda vez que a função é chamada novamente

	usleep(100000);

	for(int i = 0; i < 4; i++) {
		switch(i) {
		case 0: //Baixo
			if(lab[(2 * c + 2) * (pos_l + 1) + pos_c] == ' ')
				if(lab[(2 * c + 2) * (2 * l - 1) + (2 * c)] != '#')
					percorrerLab(l, c, lab, pos_l + 1, pos_c);
			break;

		case 1: //Direita
			if(lab[(2 * c + 2) * pos_l + (pos_c + 1)] == ' ')
				if(lab[(2 * c + 2) * (2 * l - 1) + (2 * c)] != '#')
					percorrerLab(l, c, lab, pos_l, pos_c + 1);
			break;

		case 2: //Cima
			if(lab[(2 * c + 2) * (pos_l - 1) + pos_c] == ' ')
				if(lab[(2 * c + 2) * (2 * l - 1) + (2 * c)] != '#')
					percorrerLab(l, c, lab, pos_l - 1, pos_c);
			break;

		case 3: //Esquerda
			if(lab[(2 * c + 2) * pos_l + (pos_c - 1)] == ' ')
				if(lab[(2 * c + 2) * (2 * l - 1) + (2 * c)] != '#')
					percorrerLab(l, c, lab, pos_l, pos_c - 1);
			break;
		}
	}

	if(lab[(2 * c + 2) * (2 * l - 1) + (2 * c)] != '#'){
		lab[(2 * c + 2) * pos_l + pos_c] = '\\';
		printf("Resolucao do labirinto:\n%s", lab); //O percorrimento do labirinto é impresso na tela toda vez que a função é chamada novamente
		usleep(50000);
	}
}

void embaralhar(int * lado) { //Função para embaralhar os valores do vetor "lado", com o intuito randomizar a criação do labirinto
	for(int i = 0; i < 4; i++) {
		int j = rand() % 4; // A variável "j" recebe um valor aleatório entre 0 e 3 para alterar a posição dos valores do vetor
		int aux = lado[i]; //Troca de valores
		lado[i] = lado[j];
		lado[j] = aux;
	}
}

