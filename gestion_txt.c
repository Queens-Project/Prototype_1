#include "proto.h"

/*
Fonction	: ouvrirFichier
Auteur		: Patata_Games
Param		: nomFichier est une chaîne de caractères correspondant au chemin/nom du fichier à ouvrir
Traitement	: Ouvre le fichier en lecture ("r") et renvoie le pointeur de fichier associé
Retour		: Retourne un pointeur FILE* valide si succès, NULL sinon
*/
FILE	*ouvrirFichier(const char *nomFichier)
{
	FILE	*f=fopen(nomFichier, "r");
	if (!f)
	{
		printf("Erreur ouverture fichier\n");
		return (NULL);
	}
	return (f);
}



/*
Fonction	: lectureTaille()
Auteur		: Patata_Games
Param		: f est un fichier déjà ouvert ; g est une grille de type Grille (cf. proto.h)
Traitement	: Lit la taille de la grille depuis le fichier et vérifie qu'elle respecte les bornes (entre 5 et N)
Retour		: Retourne 1 si la lecture et la validation réussissent, 0 sinon
*/
int	lectureTaille(FILE *f, Grille *g)
{
	if (fscanf(f, "%d", &g->taille) != 1)
	{
		fprintf(stderr, "Format incorrect : taille manquante\n");
		return (0);
	}

	if (g->taille < 5 || g->taille > N)
	{
		fprintf(stderr, "Taille invalide (%d). La taille doit être entre 5 et %d.\n", g->taille, N);
		return (0);
	}
	return (1);
}



/*
Fonction	: lectureGrille
Auteur		: Patata_Games
Param		: f est un fichier déjà ouvert ; g est une grille de type Grille (cf. proto.h)
Traitement	: Lit la grille de régions depuis le fichier, remplit g->regions et initialise g->etat à VIDE
Retour		: Retourne 1 si la lecture réussit, 0 sinon
*/
int	lectureGrille(FILE *f, Grille *g)
{
	for (int i=0; i<g->taille; i++)
	{
		char	ligne[(g->taille)+2];

		if (fscanf(f, "%s", ligne) != 1)
		{
			fprintf(stderr, "Format incorrect : ligne %d manquante\n", i);
			return (0);
		}

		for (int j=0; j<g->taille; j++)
		{
			g->regions[i][j] = ligne[j]-'0';
			g->etat[i][j] = VIDE;
		}
	}
	return (1);
}



/*
Fonction	: lireGrilleFromFichier
Auteur		: Patata_Games
Param		: nomFichier est le chemin du fichier de grille ; g est une grille de type Grille (cf. proto.h)
Traitement	: Ouvre le fichier, lit la taille puis le contenu de la grille et remplit la structure g
Retour		: Retourne 1 si tout s'est bien passé, 0 sinon
*/
int	lireGrilleFromFichier(const char *nomFichier, Grille *g)
{
	FILE	*f = ouvrirFichier(nomFichier);
	if (!f)
		return (0);

	if (!lectureTaille(f, g)) {
		fclose(f);
		return (0);
	}

	if (!lectureGrille(f, g))
	{
		fclose(f);
		return (0);
	}

	fclose(f);
	return (1);
}
