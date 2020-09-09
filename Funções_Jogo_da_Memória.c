#include "Funções_Jogo_da_Memória.h"

int opcao1 = 0;
unsigned char **matriz, *vetor1Base, *vetor2Base;
int tamanhoMatriz;
unsigned char tabelaJogo[10][10];
int lin1, col1, lin2, col2;  //coordenadas dos dois itens escolhidos na tabela
int duracao, pontuacaoTime;  //Variaveis para mostrar duracao do jogo e quanto o jogador perdeu devido ao tempo
int novaPontuacao = 0;
int fim = 0;  //indicará que o jogo pode acabar (quando for 1)

int carregarDadosJogo(TUSUARIO* jogadores) {  //Busca os dados do jogo de um arquivo
	int numJogadores = 0, controle=0;
	FILE *fptr;  /* ponteiro para arquivo */
	char string[300];
	if((fptr = fopen("dados.txt", "r")) == NULL) {
		printf("Nao foi possivel abrir o arquivo dados.txt");
		exit(-1);
	}
	while(fgets(string,299,fptr) != NULL) {
		if (controle == 0) {
			jogadores[numJogadores].nome = (char*) malloc(strlen(string) + 1);
			limpaString(string);
			strcpy(jogadores[numJogadores].nome, string);
			controle = 1;
		} else if (controle == 1) {
			jogadores[numJogadores].senha = (char*) malloc(strlen(string) + 1);
			limpaString(string);
			strcpy(jogadores[numJogadores].senha, string);
			controle = 2;
		} else {
			jogadores[numJogadores].pontuacao = atoi(string);		
			controle = 0;
			numJogadores++;
		}
	}
	fclose(fptr);

	return numJogadores;
}

void solicitarDadosNovoUsuario(TUSUARIO* novo) {  //Cadastro de novos usuários
	printf("\n\tInforme o nome do usuario: ");
	fflush(stdin);
	novo->nome = meugets();
	printf("\n\tInforme a senha do usuario: ");
	fflush(stdin);
	novo->senha = meugets();
	novo->pontuacao = 0;
}

void criarMatrizAleatoria(int n) {  //Cria a matriz aleatória desejada dependendo do nivel
	int i, j, cont;
	unsigned char valor, pos_l, pos_c;

	vetor1Base = (unsigned char*) malloc((n*n/2)*sizeof(unsigned char));  //Metade do vetor de base aleatório
	vetor2Base = (unsigned char*) malloc((n*n)*sizeof(unsigned char));   //Vetor de base aleatório completo

	matriz = (unsigned char**) malloc(n*sizeof(unsigned char*));
	for (i=0; i<n; i++)
		matriz[i] = (unsigned char*) malloc(n*sizeof(unsigned char));

	tamanhoMatriz = n;

	srand(time(0));

	for (i=0; i<(n*n/2); i++) {   //Gera metade do vetor de base aleatório
		valor = rand();
		valor = (valor % 200) + 50;
		vetor1Base[i] = valor;
		printf("%c ",vetor1Base[i]);
	}
	for (i=0; i<n*n ; i++)   //Duplica os valores do vetor de base para gerar um vetor completo
		vetor2Base[i] = vetor1Base[i];
	for (i=(n*n/2); i<n*n; i++)
		vetor2Base[i] = vetor1Base[i % (n*n/2)];

	for (i=0; i<n; i++) {   //Coloca todos os valores iniciais da matriz = 0
		for (j=0; j<n; j++) {
			matriz[i][j] = 0;
		}
	}
	for (i=0; i<(n*n); i++) {  //Altera os valores da matriz para os valores do vetor2Base
		cont = 0;
		while (cont == 0) {
			pos_l = rand();
			pos_l = (pos_l % n);
			pos_c = rand();
			pos_c = (pos_c % n);
			if (matriz[pos_l][pos_c] == 0) {
				matriz[pos_l][pos_c] = vetor2Base[i];
				cont = 1;
			}
		}
	}
}

char* meugets() {  //Pega strings com alocação dinâmica
	char temp[300], *retorno;
	gets(temp);
	retorno = (char*) malloc(strlen(temp) + 1);
	strcpy(retorno, temp);
	return retorno;
}

int imprimeMenuJogo() {  //Exibe o menu principal do jogo
	int opcao = 0;
	while (opcao < 1 || opcao > 3) {
		system("cls");  //Limpa a tela
		printf("\n\t\t ________________________________");
		printf("\n\t\t|       * MENU PRINCIPAL *       |");
		printf("\n\t\t|                                |\n");
		printf("\t\t| (1) Cadastrar novo jogador     |\n");
		printf("\t\t| (2) Escolher jogador existente |\n");
		printf("\t\t| (3) Sair do jogo e SALVAR      |\n");
		printf("\t\t|________________________________|\n");
		printf("\n\t\tInforme a opcao desejada: ");
		scanf("%d", &opcao);
		system("cls");  //Limpa a tela
	}
	return opcao;
}

int menuNiveis() {
	int nivel;
	system("cls");  //Limpa a tela
	printf("\n\t\t _________________________");
	printf("\n\t\t|        * NIVEIS *       |");
	printf("\n\t\t|                         |\n");
	printf("\t\t| (1) Nivel facil         |\n");
	printf("\t\t| (2) Nivel intermediario |\n");
	printf("\t\t| (3) Nivel dificil       |\n");
	printf("\t\t|_________________________|\n");
	printf("\n\n\t  REGRAS DO JOGO:\n");
	printf("\n\t> A cada jogada errada, o jogador perde 2 pontos.");
	printf("\n\t> Concluindo nivel facil, o jogador ganha 50 pontos.");
	printf("\n\t> Concluindo nivel intermediario, o jogador ganha 100 pontos.");
	printf("\n\t> Concluindo nivel dificil, o jogador ganha 150 pontos.");
	printf("\n\t> A cada 30s passados, o jogador perde 5 pontos.");
	do {
		printf("\n\n\t\tInforme o nivel desejado: ");
		scanf("%d", &nivel);
		printf("\n");
	} while (nivel > 3);
	system("cls");  //Limpa a tela
	return nivel;
}

int iniciarJogo(int pontuacaoInicial, int nivel) {  //Seleciona o nível e monta o jogo
	int novaPontuacao, inicioTime, fimTime;
	duracao = 0;
	pontuacaoTime = 0;
	inicioTime = clock();
	if (nivel == 1) {
		novaPontuacao = montaTabelaJogo(4);
	}
	else if (nivel == 2) {
		novaPontuacao = montaTabelaJogo(6);
	}
	else {
		novaPontuacao = montaTabelaJogo(10);
	}
	system("cls");
	fimTime = clock();
	duracao = (fimTime - inicioTime)/1000;  //Divide por 1000 para obter a duração em segundos
	pontuacaoTime = duracao / 30 * 5;  //Divide a duração por 30 porque tem que perder 5 pontos a cada 30seg.
	return pontuacaoInicial + novaPontuacao - pontuacaoTime;
}

int montaTabelaJogo(int n) {  //JOGO PRINCIPAL***
	int i, j;
	char item1, item2, comparacao, cont;
	criarMatrizAleatoria(n);  //Cria a matriz de itens do jogo!

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			tabelaJogo[i][j] = '.';   //Define tabela de jogo inicial (completamente oculta)
		}
	}
	while (fim == 0) {
		system("cls");
		if (n == 4)
			printf("\n\t\t  0 1 2 3\n");
		else if (n == 6)
			printf("\n\t\t  0 1 2 3 4 5\n");
		else
			printf("\n\t\t  0 1 2 3 4 5 6 7 8 9\n");
		for (i=0; i<n; i++) {
			printf("\t\t%d ", i);
			for (j=0; j<n; j++) {
				printf("%c ", tabelaJogo[i][j]);
			}
		printf("\n");
		}

		do {
			printf("\n\tInforme coordenadas da primeira posicao desejada (formato X,X): ");
			fflush(stdin);
			scanf("%d,%d", &lin1, &col1);
			if (tabelaJogo[lin1][col1] != '.')    //Se for um valor já mostrado ou fora da tabela
				printf("\n\n\t\tPOSICAO INVALIDA!");
		} while (tabelaJogo[lin1][col1] != '.');
		item1 = mostraItemSelecionado(1, n);

		do {
			printf("\n\tInforme coordenadas da segunda posicao desejada (formato X,X): ");
			fflush(stdin);
			scanf("%d,%d", &lin2, &col2);
			if (tabelaJogo[lin2][col2] != '.')    //Se for um valor já mostrado ou fora da tabela
				printf("\n\n\t\tPOSICAO INVALIDA!");
		} while (tabelaJogo[lin2][col2] != '.');
		item2 = mostraItemSelecionado(2, n);

		comparacao = comparaItensSelecionados(item1, item2);
		if (comparacao == 0) {  //Itens iguais
			printf("\n\t\tOs itens sao iguais! MUITO BEM!");
			getch();
		} else if (comparacao == 1) {  //Itens diferentes
			printf("\n\t\tOs itens nao sao iguais! TENTE NOVAMENTE!");
			novaPontuacao -= 2;
			getch();
		}
		cont = 0;
		for (i=0; i<n; i++) {
			for (j=0; j<n; j++) {
				if (tabelaJogo[i][j] == matriz[i][j])
					cont++;   //Conta quantos itens do tabuleiro o jogador já adivinhou para terminar caso tenha adivinhado todos
			}	
		}
		if (cont == n*n) {  //Se o jogador completou o tabuleiro
			fim = 1;
			if (n == 4)
				novaPontuacao += 50;
			else if (n == 6)
				novaPontuacao += 100;
			else
				novaPontuacao += 150;
		}

	}
	return novaPontuacao;
}

int mostraItemSelecionado(int contMostraItem, int n) {
	int i, j;
	system("cls");
	if (n == 4)
		printf("\n\t\t  0 1 2 3\n");
	else if (n == 6)
		printf("\n\t\t  0 1 2 3 4 5\n");
	else
		printf("\n\t\t  0 1 2 3 4 5 6 7 8 9\n");
	if (contMostraItem == 1) {
		for (i=0; i<n; i++) {
			printf("\t\t%d ", i);
			for (j=0; j<n; j++) {
				if (i == lin1 && j == col1) {
					tabelaJogo[i][j] = matriz[i][j];
				} printf("%c ", tabelaJogo[i][j]);
			}
		printf("\n");
		}
		return matriz[lin1][col1];
	} else if (contMostraItem == 2) {
		for (i=0; i<n; i++) {
			printf("\t\t%d ", i);
			for (j=0; j<n; j++) {
				if (i == lin1 && j == col1) {
					tabelaJogo[i][j] = matriz[i][j];
				} else if (i == lin2 && j == col2) {
					tabelaJogo[i][j] = matriz[i][j];
				} printf("%c ", tabelaJogo[i][j]);
			}
		printf("\n");
		}
		return matriz[lin2][col2];
	}
}

int comparaItensSelecionados(int temp1, int temp2) {
	int comparacao;
	if (temp1 == temp2)
		comparacao = 0;  //Itens iguais
	else {
		comparacao = 1;  //Itens diferentes
		tabelaJogo[lin1][col1] = '.';
		tabelaJogo[lin2][col2] = '.';
	}
	return comparacao;
}

void salvarDadosJogo(TUSUARIO* jogadores, int numJogadores) {  //Salva os dados do jogo em um arquivo
	int i=0;
	FILE *fptr;  /* ponteiro para arquivo */
	if((fptr = fopen("dados.txt", "w")) == NULL) {
		printf("Nao foi possivel abrir o arquivo dados.txt");
		exit(-1);
	}
	if (opcao1 == 1)
		numJogadores++;
	for(i=0; i < numJogadores; i++) {
			fputs(jogadores[i].nome,fptr);
			fputs("\n",fptr);
			fputs(jogadores[i].senha,fptr);
			fputs("\n",fptr);
			fprintf(fptr, "%d", jogadores[i].pontuacao);		
			fputs("\n",fptr);
		}
	fclose(fptr);
}

void limpaString(char* string) {  //Evita que o \n do cadastro volte para a tela
	int size = strlen(string);
	while (string[size-1] == '\n' || string[size-1] == '\r') {
		string[size-1] = '\0';
		size--;
	}
}