#ifndef PROTO_H
#define PROTO_H


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


#define N 8


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

int	game_loop(Grille *g);
QueenError	placer_queen(Grille *g, int line, int col);
QueenError	placer_marque(Grille *g, int line, int col);
const char	*queen_error_msg(QueenError err);

int load_random_grid_for_size(int taille, Grille *g);

#endif //PROTO_H