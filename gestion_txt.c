#include "proto.h"

/*Ouvre un fichier et le retourne*/
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


/*lit le fichier, rempli g->taille. Retourne 1 si pas d'erreur, 0 sinon*/
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


/*Lit le fichier. Rempli g->regions avec chaque case. Met chaque case à l'état VIDE.*/
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


int lireGrilleFromFichier(const char *nomFichier, Grille *g)
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


//Main de test
// int	main()
// {
// 	Grille g;

// 	if (lireGrilleFromFichier("8_1.txt", &g))
// 	{
// 		printf("Grille lue avec succès ! Taille = %d\n", g.taille);
// 	}
// 	else
// 		printf("Erreur lors de la lecture du fichier.\n");

// 	print_grille(g);
// 	return (0);
// }


/*autre main de test*/
int main(void)
{
	Grille g;

	if (!lireGrilleFromFichier("8_01.txt", &g)) {
		fprintf(stderr, "Erreur lors de la lecture de la grille.\n");
		return 1;
	}

	initscr();
	noecho();
	curs_set(0);

	init_colors_cases();
	game_loop(&g);


	endwin();

	return (0);
}