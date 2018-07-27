#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

// em C:			
//	a = 97	|	A = 65
//  b = 98	|	B =	66
//	(...)	|	(...)
//  y = 121	|	Y = 89
//	z = 122	|	Z = 90
//
//		então x = X+32

//==============================================================================================================
struct tipo_dicionario {
	char word[20];
	char dica1[100];
	char dica2[100];
	dicionario* prev;
	dicionario* next;
};
//==============================================================================================================
struct tipo_forca {
	dicionario* dict;
	int disc[20];
	int used[26];
	int hints;
	int tries;
	int points;
};
//==============================================================================================================
struct tipo_ranking {
	char name[20];
	int score;
	ranking* prev;
	ranking* next;
};
//==============================================================================================================
int char_pos(char x){ //transforma maiuscula em minuscula e retorna a "posição" da letra no alfabeto. ex: a=0, b=1
	if (x < 97) x += 32;
	return x - 97;
}
//==============================================================================================================
char char_unpos(int x){ //transforma a "posição" da letra no alfabeto em um "char";
	return x + 97;
}
//==============================================================================================================
dicionario* alocar_dicionario(char word[], char dica1[], char dica2[]){ 
	dicionario* novo = (dicionario*)calloc(1,sizeof(dicionario));

	strcpy(novo->word, word);
	strcpy(novo->dica1, dica1);
	strcpy(novo->dica2, dica2);

	return novo;
}
//==============================================================================================================
forca* alocar_forca(dicionario* word){	
	forca* novo = (forca*)calloc(1,sizeof(forca));
	novo->dict = word;
	novo->tries = 7;
	novo->hints = 2;

	return novo;
}
//==============================================================================================================
ranking* alocar_ranking(char name[], int score){
	ranking* novo = (ranking*)calloc(1,sizeof(ranking));

	strcpy(novo->name, name);
	novo->score = score;

	return novo;
}
//==============================================================================================================
void insert_dicionario(dicionario* W[], char word[], char dica1[], char dica2[]){  //insere uma palavra e suas dica no dicionario
	int posit = char_pos(word[0]);
	dicionario* novo = alocar_dicionario(word, dica1, dica2);
	if(W[posit] == NULL) W[posit] = novo;
	else{
		dicionario* aux = W[posit];
		while(aux->next != NULL) aux = aux->next;
		aux->next = novo;
		novo->prev = aux;
	}
}
//==============================================================================================================
dicionario* read_word(dicionario* W[], int x){ //le uma palavra do dicionario e a coloca no final da lista
	dicionario* aux = W[x], *aux2 = W[x];
	if(aux != NULL){
		while(aux2->next != NULL) aux2 = aux2->next;
		if(aux != aux2){
			W[x] = aux->next;
			aux2->next = aux;
			aux->next = NULL;
		}
	}
	return aux;
}
//==============================================================================================================
void show_dicionario(dicionario* W[]){  //mostra o dicionario completo
	dicionario* aux;
	printf("\n");
	printf("Dicionário: \n");
	printf("\n");
	for(int i = 0; i < 26; i++){
		aux = W[i];
		printf("- %c: \n", char_unpos(i));
		while(aux != NULL){
			printf(" - %s\n", aux->word);
			printf("  - %s\n", aux->dica1);
			printf("  - %s\n", aux->dica2);
			aux = aux->next;
		}	
	}
}
//==============================================================================================================
int forca_add_char(forca* F, char guess){	//verifica o palpite (letra) e verifica se todas as letras foram descobertas (check_discovered)
	int found = 0;
	if(F->used[char_pos(guess)] == 0){
		F->used[char_pos(guess)] = 1;
		for(int i = 0; i < 20; i++){
			if(F->dict->word[i] == guess){
				F->disc[i] = 1;
				F->points += 2;
				found++;
			}
		}

		if (found == 0) {
			F->points -=2;
			F->tries--;
		}
	}else printf("Letra já usada.\n");

	found = check_discovered(F);
	if(found) printf("Todas as letras descobertas!\n");
	return found;
}
//==============================================================================================================
int forca_add_word(forca* F, char guess[]){  //verifica o palpite (palavra)
	int length = strlen(guess), found = 0, cmp = strcmp(F->dict->word, guess);
	if(cmp == 0){
		for(int i = 0; i < length; i++){
			if(F->disc[i] == 1) found++;
		}
		if(found*2 <= length) F->points +=10;
		else F->points +=5;
		printf("Acertou!\n");
		return 1;
	}else{
		F->points -= 4;
		printf("Errou!\n");
		return 0;
	}
}
//==============================================================================================================
ranking* rank_update(ranking *N, char name[], int score){ //adiciona e atualiza (ordena) o ranking
	ranking *aux = N;
	int n = 0;
	while(aux != NULL){
		n++;
		aux = aux->next;
	}
	aux = find_rank(N, name);
	if(aux == NULL){
		N = add_rank(N, name, score);
		n++;
	}else if(score > aux->score){
		aux->score = score;
	}
	insertionsort_name(N,n);  
	insertionsort_score(N,n);
	return N;
}
//==============================================================================================================
void insertionsort_score(ranking* N, int n){ //insertionsort para a pontuacao(int)
	int i;
	ranking *key = N, *a = N;

	for(i = 1; i < n; i++){
		key = N->next;
		a = key->prev;

		if(key->next == NULL) key->prev->next = NULL;
		else{
			key->next->prev = key->prev;
			key->prev->next = key->next;
		}

		while((a != NULL) && (a->score > key->score)){
			a = a->prev;
		}

		if(a == NULL){
			key->next = N;
			N->prev = key;
			N = key;	
		}else{
			key->next = a->next;
			key->prev = a;
			a->next = key;
			key->next->prev = key;
		}
	}

}
//==============================================================================================================
void insertionsort_name(ranking* N, int n){ //insertionsort para os nome (string)
	int i, j;
	ranking *key = N, *a = N;

	for(j = 19; j >= 0; j--){
		for(i = 1; i < n; i++){
			key = N->next;
			a = key->prev;

			if(key->next == NULL) key->prev->next = NULL;
			else{
				key->next->prev = key->prev;
				key->prev->next = key->next;
			}

			while((a != NULL) && (a->name[j] < key->name[j])){
				a = a->prev;
			}

			if(a == NULL){
				key->next = N;
				N->prev = key;
				N = key;	
			}else{
				key->next = a->next;
				key->prev = a;
				a->next = key;
				key->next->prev = key;
			}
		}
	}
}
//==============================================================================================================
void show_used(forca* F){		//mostra as letras utilizadas (palpites dados)
	printf("Letras \"Usadas\": ");
	for(int i = 0; i < 26; i++){
		if(F->used[i] == 1) printf("%c ", char_unpos(i));
	}
	printf("\n");
}
//==============================================================================================================
void show_discovered(forca* F){	//mostra a palavra com as letras que foram descobertas (ex: g e l _ d e _ _ _  = geladeira)
	int len = strlen(F->dict->word);
	printf("\n\t");
	for(int i = 0; i < len; i++){
		if(F->disc[i] == 1) printf("%c ", F->dict->word[i]);
		else printf("_ ");
	}
}
//==============================================================================================================
void remove_word(dicionario* W[], char x[]){   //remove uma palavra do dicionário
	int pos = char_pos(x[0]);
	dicionario* aux = find_word(W, x);
	if(aux != NULL){
		if(aux->next == NULL && aux->prev == NULL){
			W[pos] = NULL;
		}else if(aux->prev == NULL){
			W[pos] = aux->next;
			aux->next->prev = NULL;
		}else if(aux->next == NULL){
			aux->prev->next = NULL;
		}else{
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
		}
		free(aux);
		printf("Palavra removida.\n");
	}else{
		printf("Palavra não encontrada.\n");
	}
}
//==============================================================================================================
dicionario* find_word(dicionario* W[], char x[]){ //verifica se uma palavra existe no dicionario e retorna ela
	int found = 1, pos = char_pos(x[0]);
	dicionario* aux = W[pos];
	while(aux != NULL){
		found = strcmp(aux->word, x);
		if(found == 0) break;
		aux = aux->next;
	}
	return aux;
}
//==============================================================================================================
void show_ranking(ranking* N){ //mostra o ranking
	ranking* aux = N;
	int len, j, i = 0;
	printf("o=========o=======RANKING========o========o\n");
	printf("| POSICAO |        NOME:         | PONTOS |\n");
	while(aux != NULL){
		printf("| %d      | %s", i, aux->name);
		len = strlen(aux->name);
		for(j = len; j <= 20; j++) printf(" ");
		printf(" | %d", aux->score);
		len = int_length(aux->score);
		for(j = len; j <= 20; j++) printf(" ");
		printf(" |\n");
		i++;
		aux = aux->next;
	}
	printf("|         |                      |        |\n");
	printf("o=========o======================o========o\n");
	printf("\n");
}
//==============================================================================================================
int int_length(int x){ //verifica o numero de digitos de um int exemplo: 1234 tem tamanho 4, 32 tem tamanho 2.
	int len = 0;
	if(x == 0) len++;
	while (x != 0) {
		x = x/10;
		len++;	
	}
	return len;
}
//==============================================================================================================
void show_forca(forca* F){ //mostra o estado atual da forca
	int i;
	printf("Pontos: %d\n", F->points);
	printf("Tentativas restantes: %d\n", F->tries);
	printf("Letras usadas: ");
	for(i = 0; i<26; i++){
		if(F->used[i] == 1) printf("%c ", char_unpos(i));
	}
	printf("\n");
	printf("Palavra: ");
	for(i = 0; i < strlen(F->dict->word);  i++){
		if(F->disc[i] == 1) printf("%c ", F->dict->word[i]);
		else printf("_ ");
	}
	printf("\n");
	if(F->hints < 2){
		printf("Dicas: \n");
		printf("  %s\n", F->dict->dica1);
	}
	if(F->hints < 1) printf("  %s\n", F->dict->dica2);
}
//==============================================================================================================
ranking* add_rank(ranking* N, char name[], int score){ //adiciona alguem ao ranking
	ranking* novo = alocar_ranking(name, score);
	if (N == NULL) N = novo;
	else{
		ranking* aux = N;
		while(aux->next != NULL) aux = aux->next;
		aux->next = novo;
		novo->prev = aux;
	}
	return N;
}
//==============================================================================================================
ranking* find_rank(ranking* N, char name[]){ //verifica se alguem ja esta no ranking
	ranking* aux = N;
	while(aux != NULL && strcmp(name, aux->name) != 0) aux = aux->next;
	return aux;
}
//==============================================================================================================
void endgame(ranking* N, int points){ //finaliza o jogo
	printf("\nGAME OVER\n\n");
	char word[20];
	printf("Sua Pontuacao: %d\n", points);
	printf("Nome: ");
	scanf(" %s", word);
	N = rank_update(N, word, points);
}
//==============================================================================================================
void nextgame(forca* F, dicionario* word){ //reinicia o jogo com uma nova palavra, mantendo a pontuacao ("continua o jogo" quando o jogador acerta a palavra)
	
	F->dict = word;
	F->tries = 7;
	F->hints = 2;
	for(int i = 0; i < 26; i++)	F->used[i] = 0;
	for(int i = 0; i < 20; i++)	F->disc[i] = 0;
}
//==============================================================================================================
int check_discovered(forca* F){  //verifica se todas as letras de uma palavra foram descobertas
	for(int i = 0; i < strlen(F->dict->word); i++) if(F->disc[i] == 0) return 0;
	return 1;
}