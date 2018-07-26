#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "def.c" 
//#include "header.h"
/*
lista:
	
*/

dicionario* W[26];
forca* F;
ranking* N;
char a;

int menu(int menus){
	int op, i;


	if(menus == 0){
		printf("\n");
		printf("o=====================JOGO DA FORCA=====================o\n");
		printf("|                                                       |\n");
		printf("|  0. Sair.                                             |\n");
		printf("|  1. Iniciar Jogo.                                     |\n");
		printf("|  2. Mostrar Dicionario.                               |\n");
		printf("|  3. Adicionar Palavra.                                |\n");
		printf("|  4. Remover Palavra.                                  |\n");
		printf("|  5. Consultar Palavra.                                |\n");
		printf("|  6. Ver Ranking de Pontuacao.                         |\n");
		printf("|                                                       |\n");
		printf("o=======================================================o\n");
		printf("\n");
		printf("Opcao escolhida: ");
		scanf(" %d", &op);
	}
	if(menus == 1){
		show_forca(F);
		printf("o=======================================================o\n");
		printf("|                                                       |\n");
		printf("|  0. Encerrar Jogo                                     |\n");
		printf("|  1. Palpite: Letra.                                   |\n");
		printf("|  2. Palpite: Palavra.                                 |\n");
		printf("|  3. Pedir Dica. (%d)                                   |\n", F->hints);
		printf("|                                                       |\n");
		printf("o=======================================================o\n");

		printf("Opcao escolhida: ");
		scanf(" %d", &op);

	}

	return op;
}

void main(){
	srand(time(NULL));
	int menus = 0, op = 1, random, i = 0, found = 0;
	char word[20], dica1[100], dica2[100], letter;
	dicionario* aux = NULL;
	while(op){
		op = menu(menus);
		if (menus == 1){
			switch(op){
				case 0:
					printf("Certeza que deseja sair?(s/n)\n");
					scanf(" %c", &letter);
					if(letter == 's') F->tries = 0;
					op--;
				break;

				case 1:
					printf("Letra: ");
					scanf(" %c", &letter);
					found = forca_add_char(F, letter);
					if(found){
						random = rand()%27;
						while(aux == NULL && i < 26) {
							aux = read_word(W, (random + i)%27);
							i++;
						}
						nextgame(F, aux);
					}
				break;

				case 2:
					printf("Palavra: ");
					scanf(" %s", word);
					found = forca_add_word(F, word);
					if(found){
						random = rand()%27;
						while(aux == NULL && i < 26) {
							aux = read_word(W, (random + i)%27);
							i++;
						}
						nextgame(F, aux);
					}
				break;

				case 3:
					if(F->hints > 0){
						F->hints--;
						F->points -= 3;
					}else{
						printf("Acabaram as dicas.\n");
					}
				break;
			}
			if(F->tries == 0){
				menus = 0;
				op = 6;
				endgame(N, F->points);
			}	
		}
		if(menus == 0){
			switch(op){
					
				case 1:
					random = rand()%27;
					while(aux == NULL && i < 26) {
						aux = read_word(W, (random + i)%27);
						i++;
					}
					if(i < 26){
						F = alocar_forca(aux);
						menus = 1;
					}else{
						printf("Dicionario vazio, coloque pelo menos 1 palavra no dicionario\n");
					}
					i = 0;
				break;
					
				case 2:
					show_dicionario(W);
				break;
					
				case 3:
					printf("Palavra: \n");
					scanf(" %s", word);
					printf("Dica 1: \n");
					scanf(" %s", dica1);
					printf("Dica 2: \n");
					scanf(" %s", dica2);
					insert_dicionario(W, word, dica1, dica2);
				break;
					
				case 4:
					printf("Palavra: \n");
					scanf(" %s", word);
					remove_word(W, word);
				break;
					
				case 5:
					printf("Palavra: \n");
					scanf(" %s", word);
					aux = find_word(W, word);

					if(aux != NULL){
						printf("\n");
						printf("Palavra: %s\n", aux->word);
						printf("Dica 1: %s\n", aux->dica1);
						printf("Dica 2: %s\n", aux->dica2);
					}else{
						printf("Palavra não encontrada.\n");
					}
				break;

				case 6:
					show_ranking(N);
				break;
			}
		}
	}
}