#ifndef PROTO_H
#define PROTO_H


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include <time.h>
#include <string.h>


#define N 8

//sructures kilian
typedef struct {
    char pseudo[25];
    int temps;
} Score;

// structures clem
typedef enum {
	VIDE = 0,
	QUEEN = 1,
	MARK = 2,
} EtatCase;

typedef struct {
	int			taille;
	int			regions[N][N];
	EtatCase	etat[N][N];
} Grille;

typedef enum {
	PLACEMENT_OK = 0,      // aucun problème
	ERROR_OUT_OF_BOUNDS,   // hors de la grille
	ERROR_OCCUPIED,        // case déjà occupée
	ERROR_ADJACENT,        // reine adjacente
	ERROR_LINE,            // reine sur la ligne
	ERROR_COLUMN,          // reine sur la colonne
	ERROR_REGION           // reine dans la région
} QueenError;


int	inside_grille(Grille *g, int line, int col);
int	check_adjacent(int col, int line, Grille *g);
int	check_region(int col, int line, Grille *g);
int	check_line(int col, int line, Grille *g);
int	check_column(int col, int line, Grille *g);
QueenError	placement_queen(int col, int line, Grille *g);

int	validation(Grille *g);

void init_colors_cases(void);
void afficherGrilleNcurses(const Grille *g, int x, int y);

int	game_loop(Grille *g, time_t debut);
QueenError	placer_queen(Grille *g, int line, int col);
QueenError	placer_marque(Grille *g, int line, int col);
const char	*queen_error_msg(QueenError err);

int load_random_grid_for_size(int taille, Grille *g);
int lireGrilleFromFichier(const char *nomFichier, Grille *g);


// partie kilian
void menu();
void crea_pseudo();
void load_game();
void afficher_scores();
void regles_du_jeu();
int choisir_taille_grille(void);
void lancer_partie(const char *pseudo);

void update_scores(const char *pseudo, int temps);
int load_scores(Score scores[]);
void add_score(Score scores[], int *count, const char *pseudo, int temps);
void sort_scores(Score scores[], int n);
void save_scores(Score scores[], int count);

#endif //PROTO_H