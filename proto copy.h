#ifndef PROTO_H
#define PROTO_H


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define N 10

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

void	init_colors_cases();
void	print_grille(Grille g);
void afficherGrilleNcurses(const Grille *g, int x, int y);

#endif //PROTO_H
