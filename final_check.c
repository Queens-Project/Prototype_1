#include "proto.h"


/*Compte le nombre de reine placées sur une ligne*/
int	queen_ligne(Grille *g, int l)
{
	int	compteur = 0;
	for (int c=0; c<g->taille; c++)
	{
		if (g->etat[l][c] == QUEEN)
			compteur++;
	}
	return (compteur);
}

/*Compte le nombre de reine placées sur une colonne*/
int	queen_col(Grille *g, int c)
{
	int compteur = 0;
	for (int l=0; l<g->taille; l++)
	{
		if (g->etat[l][c] == QUEEN)
			compteur++;
	}
	return (compteur);
}

/*Compte le nombre de régions*/
int	nb_region(Grille *g)
{
	int	max = g->regions[0][0];
	for (int l=0; l<g->taille; l++)
	{
		for (int c=0; c<g->taille; c++)
		{
			if (g->regions[l][c] > max)
				max = g->regions[l][c];
		}
	}
	return (max);
}

/*Compte le nombre de reine placées dans une région*/
int	queen_region(Grille *g, int region_id)
{
	int	compteur = 0;
	for (int l=0; l<g->taille; l++)
	{
		for (int c=0; c<g->taille; c++)
		{
			if (g->regions[l][c] == region_id && g->etat[l][c] == QUEEN)
				compteur++;
		}
	}
	return (compteur);
}


/**/
int	queen_adjacente(Grille *g)
{
	for (int line=0; line <g->taille; line++)
	{
		for (int col=0; col<g->taille; col++)
		{
			if (g->etat[line][col] == QUEEN)
			{
				for (int dl=-1; dl<=1; dl++)
				{
					for (int dc=-1; dc<=1; dc++)
					{
						if (!(dl==0 && dc==0))
						{
							int nl=line+dl;
							int nc=col+dc;

							if (nl>=0 && nl<g->taille && nc>=0 && nc <g->taille)
							{
								if (g->etat[nl][nc] == QUEEN)
									return (0);
							}
						}
					}
				}
			}
		}
	}
	return (1);
}


int	validation(Grille *g)
{
	if (!g || g->taille <=0)
		return (0);

	if (!(queen_adjacente(g)))
		return (0);

	for (int l=0; l<g->taille; l++)
		if (queen_ligne(g, l) != 1)
			return (0);
	
	for (int c=0; c<g->taille; c++)
		if (queen_col(g, c) != 1)
			return (0);

	int	max_id = nb_region(g);
	if (max_id<0)
		return (0);
	
	for (int id=0; id<=max_id; id++)
		if (queen_region(g, id) != 1)
			return (0);
	
	return (1);
}