#include "proto.h"

/*Ce fichier contient les fonctions qui vérifient à chaque 
mouvement du joueur si les conditions de victoire sont réunies.
Ainsi le jeu détecte la victoire automatiquement dès que le joueur trouve la solution*/



/*
Fonction	: queen_ligne
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); l pour la ligne à vérifier
Traitement	: Compte le nombre de Reine sur une ligne l
Retour		: compteur est le nombre de Reine trouvées
*/
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



/*
Fonction	: queen_col
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); c pour la colonne à vérifier
Traitement	: Compte le nombre de Reine sur une colonne c
Retour		: compteur est le nombre de Reine trouvées
*/
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



/*
Fonction	: nb_region
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h)
Traitement	: Trouve l'id région maximum (car chaque région est identifié par un numéro. cf. fichiers textes du dossier grilles)
Retour		: max est l'id région maximum trouvé
*/
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



/*
Fonction	: queen_region
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); region_id est le numéro d'une région
Traitement	: Compte le nombre de Reine dans une région identifiée par region_id dans une grille g
Retour		: compteur est le nombre de Reine trouvées
*/
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



/*
Fonction	: queen_adjacente
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); c pour la colonne à vérifier
Traitement	: Vérifie qu'aucune Reine dans la grille g n'est adjacente avec une autre Reine
Retour		: Retourne 0 si un cas d'adjacence est trouvé, 1 sinon.
*/
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



/*
Fonction	: validation
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h)
Traitement	: Utilise les fonctions de vérification précédentes pour vérifier les conditions de victoire
Retour		: Retourne 0 si la grille n'est pas encore valide et 1 si la grille est validée (VICTOIRE)
*/
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