#include "proto.h"

/*Ce fichier contient les fonctions qui vérifient qu'un 
placement de reine voulu par le joueur est valide*/



/*
Fonction	: inside_grille
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'un point rentré en paramètre par ses coordonnées (line,col) se situe dans la grille
Retour		: NONE
*/
int	inside_grille(Grille *g, int line, int col)
{
	return (g && line>=0 && col>=0 && line<g->taille && col < g->taille);
}


/*
Fonction	: check_adjacent
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'un point (line,col) rentré en paramètre n'a pas de Reine adjacente
Retour		: 0 si Reine adjacente, 1 sinon
*/
int	check_adjacent(int col, int line, Grille *g)
{
	if (!inside_grille(g, line, col))
		return (0);

	for (int dl=-1; dl<=1; dl++)
	{
		for (int dc=-1; dc<=1; dc++)
		{
			if (dl!=0 || dc!=0)
			{
				int	nl = line + dl;
				int nc = col + dc;
				if (inside_grille(g, nl, nc))
				{
					if (g->etat[nl][nc] == QUEEN)
						return (0);
				}
			}
		}
	}
	return (1);
}



/*
Fonction	: check_region
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'un point (line,col) rentré en paramètre n'a pas de reine dans sa région
Retour		: 0 si Reine dans la région, 1 sinon
*/
int	check_region(int col, int line, Grille *g)
{
	if (!inside_grille(g, line, col))
		return (0);
	
	int	region_id = g->regions[line][col];

	for (int i=0; i<g->taille; i++)
	{
		for (int j=0; j<g->taille; j++)
		{
			if (i!=line || j!=col)
			{
				if (g->regions[i][j] == region_id && g->etat[i][j] == QUEEN)
					return (0);
			}
		}
	}
	return (1);
}



/*
Fonction	: check_line
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'un point (line,col) rentré en paramètre n'a pas de Reine sur la même ligne
Retour		: 0 si une reine sur la même ligne, 1 sinon
*/
int	check_line(int col, int line, Grille *g)
{
	if (!inside_grille(g, line, col))
		return (0);
	
	for (int j=0; j<g->taille; j++)
	{
		if (j != col)
		{
			if (g->etat[line][j] == QUEEN)
				return (0);
		}
	}
	return (1);
}



/*
Fonction	: check_column
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'un point (line,col) rentré en paramètre n'a pas de Reine sur la même colonne
Retour		: 0 si une reine sur la même colonne, 1 sinon
*/
int	check_column(int col, int line, Grille *g)
{
	if (!inside_grille(g, line, col))
		return (0);
	
	for (int i=0; i<g->taille; i++)
	{
		if (i != line)
		{
			if (g->etat[i][col] == QUEEN)
				return (0);
		}
	}
	return (1);
}



/*
Fonction	: check_region
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour la ligne; col pour la colonne
Traitement	: Vérifie qu'une reine peut être placé sur la case de coordonnées (line,col)
Retour		: Si le placement est possible, retourne PLACEMENT_OK
			  Sinon, retourne l'erreur correspondante (exemple : si déjà une autre reine dans la région, retourne ERROR_REGION)
*/
QueenError placement_queen(int col, int line, Grille *g)
{
	QueenError err = PLACEMENT_OK;
	int	print_error = 3;

	if (!inside_grille(g, line, col))
	{
		err = ERROR_OUT_OF_BOUNDS;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	if (g->etat[line][col] == QUEEN)
	{
		err = ERROR_OCCUPIED;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	if (!check_adjacent(col, line, g))
	{
		err = ERROR_ADJACENT;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	if (!check_line(col, line, g))
	{
		err = ERROR_LINE;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	if (!check_column(col, line, g))
	{
		err = ERROR_COLUMN;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	if (!check_region(col, line, g))
	{
		err = ERROR_REGION;
		mvprintw(g->taille + print_error, 0, "Placement refuse: %s\n", queen_error_msg(err));
		print_error++;
	}

	return err;
}