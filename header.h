#ifndef FORCA_HEADER_H
#define FORCA_HEADER_H
//==============================================================================================================
typedef struct tipo_dicionario dicionario;  
typedef struct tipo_forca forca;
typedef struct tipo_ranking ranking;
//==============================================================================================================
int char_pos(char x);
char char_unpos(int x);
dicionario* alocar_dicionario(char word[], char dica1[], char dica2[]);
forca* alocar_forca(dicionario* word);
ranking* alocar_ranking(char name[], int score);
void insert_dicionario(dicionario* W[], char word[], char dica1[], char dica2[]);
dicionario* insert_W(dicionario* W, dicionario* novo);
dicionario* read_word(dicionario* W[], int x);
void remove_word(dicionario* W[], char x[]);
dicionario* find_word(dicionario* W[], char x[]);
void show_dicionario(dicionario* W[]);
int forca_add_char(forca* F, char guess);
int forca_add_word(forca* F, char guess[]);
ranking* rank_update(ranking* N, char name[], int score);
void insertionsort_score(ranking* N, int n);
void insertionsort_name(ranking* N, int n);
void show_forca(forca* F);
void show_used(forca* F);
void show_discovered(forca* F);
int int_length(int x);
ranking* find_rank(ranking* N, char name[]);
ranking* add_rank(ranking* N, char name[], int score);
void nextgame(forca* F, dicionario* word);
void endgame(ranking* N, int points);
int check_discovered(forca* F);
//==============================================================================================================
#endif