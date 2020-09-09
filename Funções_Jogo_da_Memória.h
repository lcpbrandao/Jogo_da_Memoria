#ifndef JOGO_DA_MEMORIA
	#define JOGO_DA_MEMORIA

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
		char *nome;
		char *senha;
		int pontuacao;
	} TUSUARIO;

int carregarDadosJogo(TUSUARIO* jogadores);

void solicitarDadosNovoUsuario(TUSUARIO* novo);

void criarMatrizAleatoria(int n);

char* meugets();

int imprimeMenuJogo();

int menuNiveis();

int iniciarJogo(int pontuacaoInicial, int nivel);

int montaTabelaJogo(int n);

int mostraItemSelecionado(int contMostraItem, int n);

int comparaItensSelecionados(int temp1, int temp2);

void salvarDadosJogo(TUSUARIO* jogadores, int numJogadores);

void limpaString(char* string);

#endif