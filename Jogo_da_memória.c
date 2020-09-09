#include "Funções_Jogo_da_Memória.h"

extern int opcao1;
extern unsigned char **matriz, *vetor1Base, *vetor2Base;
extern int tamanhoMatriz;
extern int duracao, pontuacaoTime;

void main() {
	TUSUARIO jogadores[50];
	int numJogadores = 0;
	char *nomeTemp, *senhaTemp, denovo;
	int opcao = 0, i, achou, nivel;

	numJogadores = carregarDadosJogo(jogadores);

	while(opcao != 3) {
		opcao = imprimeMenuJogo();
		if (opcao == 1) {
			opcao1 = 1;
			//cadastro novo usuario
			solicitarDadosNovoUsuario(&jogadores[numJogadores]);
			printf("\n\n\tBEM VINDO, %s! Vamos iniciar o jogo!\n", jogadores[numJogadores].nome);
			getch();
			do {
				nivel = menuNiveis();
				jogadores[numJogadores].pontuacao = iniciarJogo(jogadores[numJogadores].pontuacao, nivel);
				printf("\n\n\t\tPARABENS, %s!\n\n\t\tSua pontuacao agora eh %d!\n", jogadores[numJogadores].nome, jogadores[numJogadores].pontuacao);
				printf("\n\tSeu jogo durou %.2f seg e voce perdeu %d pontos devido ao tempo.\n", (float)duracao, pontuacaoTime);
				printf("\n\n\t\tGostaria de jogar novamente? (s/n) ");
				fflush(stdin);
				scanf("%c", &denovo);
			} while (denovo == 's');
		} else if (opcao == 2) {
			//usar um jogador existente
			printf("\n\t\t ______________________________");
			printf("\n\t\t    * JOGADORES EXISTENTES *    ");
			printf("\n\t\t                                \n");
			for (i=0; i<numJogadores; i++) {
				printf("\t\t     %s - Pontuacao: %d      \n", jogadores[i].nome, jogadores[i].pontuacao);
			}
			printf("\t\t ______________________________ \n");
			printf("\n\t\tInforme o nome do jogador: ");
			fflush(stdin);
			nomeTemp = meugets();
			for (i=0; i<numJogadores; i++) {
				if (strcmp(nomeTemp, jogadores[i].nome) == 0) {
					achou = 1;
					printf("\n\t\tJOGADOR LOCALIZADO! Pontuacao atual: %d.\n", jogadores[i].pontuacao);
					printf("\n\t\tInforme a senha para jogar com este usuario: ");
					fflush(stdin);
					senhaTemp = meugets();
					system("cls");  //Limpa a tela
					if (strcmp(senhaTemp, jogadores[i].senha) == 0) {
						printf("\n\n\t\tSENHA CONFIRMADA! Vamos iniciar o jogo!\n");
						getch();
						break;
					} else {
						printf("\n\t\tSENHA NAO CONFIRMADA!\n");
						i = -1;
						getch();
						break;
					}
				}
			}
			if (achou != 1) {
				printf("\n\t\tJOGADOR NAO LOCALIZADO! Cadastre-se para jogar.\n");
				i = -1;
				getch();
			}
			if (i != -1) {  //Iniciar jogo!
				do {
					nivel = menuNiveis();
					jogadores[i].pontuacao = iniciarJogo(jogadores[i].pontuacao, nivel);
					printf("\n\n\t\tPARABENS, %s!\n\n\t\tSua pontuacao agora eh %d!\n", jogadores[i].nome, jogadores[i].pontuacao);
					printf("\n\tSeu jogo durou %.2f seg e voce perdeu %d pontos devido ao tempo.\n", (float)duracao, pontuacaoTime);
					printf("\n\n\t\tGostaria de jogar novamente? (s/n) ");
					fflush(stdin);
					scanf("%c", &denovo);
				} while (denovo == 's');
			}
		} else if (opcao == 3) {
			//salvar informacoes para arquivo
			salvarDadosJogo(jogadores, numJogadores);
			
			//sair do jogo / limpar memória
			for (i=0; i<numJogadores; i++) {
				free(jogadores[i].nome);
				free(jogadores[i].senha);
			}
			for (i=0; i<tamanhoMatriz; i++)
				free(matriz[i]);
			free(matriz);
			free(vetor1Base);
			free(vetor2Base);
		}
	}
}