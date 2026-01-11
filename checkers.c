#include "proto.h"

/*Ce fichier contient les fonctions qui vérifient qu'un 
placement de reine voulu par le joueur est valide*/


/*Vérifie que le placement donné en argument est situé 
dans la grille (et pas en dehors des frontières)*/
int	inside_grille(Grille *g, int line, int col)
{
	return (g && line>=0 && col>=0 && line<g->taille && col < g->taille);
}

/*Vérifie que les 8 cases adjacentes ne contiennent 
pas de reine. Si OK alors renvoie 1, 0 sinon.*/
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


/*Vérifie qu'il n'y ait pas déjà une reine 
placée dans la même région*/
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


/*Vérifie qu'il n'y a pas déjà une reine sur la même ligne.*/
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


/*Vérifie qu'il n'y ait pas déjà une reine sur la même colonne. */
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


/*Détermine si le placement de reine demandé par le joueur est valide.
Utilise toutes les fonctions de vérifications précédentes.*/
QueenError placement_queen(int col, int line, Grille *g)
{
	if (!inside_grille(g, line, col))
		return ERROR_OUT_OF_BOUNDS;

	if (g->etat[line][col] == QUEEN)
		return ERROR_OCCUPIED;

	if (!check_adjacent(col, line, g))
		return ERROR_ADJACENT;

	if (!check_line(col, line, g))
		return ERROR_LINE;

	if (!check_column(col, line, g))
		return ERROR_COLUMN;

	if (!check_region(col, line, g))
		return ERROR_REGION;

	return PLACEMENT_OK;
}