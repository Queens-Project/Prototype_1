#include "proto.h"

/*Check que les huit cases autour soient libre*/
int	check_adjacent(int col, int line, Grille g)
{
	//check les 3 cases au dessus du placement
	if (g.etat[line-1][col-1] == 1)
		return (0);//il y a déjà une reine
	if (g.etat[line-1][col] == 1)
		return (0);
	if (g.etat[line-1][col+1] == 1)
		return (0);

	//check les cases à droite et à gauche du placement
	if (g.etat[line][col-1] == 1)
		return (0);
	if (g.etat[line+1][col+1] == 1)
		return (0);

	//check les 3 cases en dessous du placement
	if (g.etat[line+1][col-1] == 1)
		return (0);
	if (g.etat[line+1][col] == 1)
		return (0);
	if (g.etat[line+1][col+1] == 1)
		return ();

	return (1); //Check OK
}


/*Check qu'il n'y ai pas déjà une reine placée dans la région*/
int	check_region(int col, int line, Grille g)
{
	for (int i=0; i<g.taille; i++)
	{
		for (int j=0; j<g.taille; j++)
		{
			if (g.region[i][j] == g.region[line][col] && (i!=line || j!=col)) //Si on se trouve dans la même région
				if (g.etat[i][j] == QUEEN) //Si il y a une autre reine
					return (0);//il y a déjà une reine
		}
	}
	return (1) //Check OK
}


/*Check qu'il n'y ai pas déjà une reine sur la même ligne*/
int	check_line(int col, int line, Grille g)
{
	for (int i=col-1; i==0; i--)
	{
		if (g.etat[line][i] == 1)
			return (0); 
	}
	for (int i=col+1; i<g.taille; i++)
	{
		if (g.etat[line][i] == 1)
			return (0);
	}
	return (1);//Check OK
}


/*Check qu'il n'y ai pas déjà une reine sur la même colonne*/
int	check_column(int col, int line, Grille g)
{
	for (int i=line-1; i==0; i--)
	{
		if (g.etat[i][col] == 1)
			return (0);
	}
	for (int i=line+1; i<g.taille; i++)
	{
		if (g.etat[i][col] == 1)
			return (0);
	}
	return (1);//Check OK
}
