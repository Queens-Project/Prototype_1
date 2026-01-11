#include "proto.h"

/*
Ce fichier contient les fonctions qui gère le système de scores du jeu :
 - Calcul du score
 - Tri des scores
 - Chargement/Sauvegarde dans un fichier texte
 - Affichage des scores en menu ou en fin de partie
*/



/*
Affiche la liste des meilleurs scores à l'écran.
Attend que l'utilisateur appuie sur Q pour revenir au menu.
*/
void	afficher_scores()
{
	Score	scores[MAX_SCORES];
	int		count = load_scores(scores);

	int		y = 0;
	int		tap = 0;
	clear();

	mvprintw(y++, 0, "=== SCORES ===");
	y++;

	for (int i = 0; i < count; i++)
	{
		mvprintw(y++, 0,
            "%d. %-20s | %dX%d | %5d pts | %d:%02d",
            i + 1,
            scores[i].pseudo,
            scores[i].taille,
            scores[i].taille,
            scores[i].score,
            scores[i].temps / 60,
            scores[i].temps % 60
		);
	}

	mvprintw(y + 2, 0, "Appuyez sur Q pour revenir");
	refresh();

	while (tap != 'q' && tap != 'Q')
		tap = getch();

	clear();
	refresh();
}


/*
Charge les scores depuis le fichier "score.txt" dans le tableau fourni.
Supporte deux formats de sauvegarde différent (et recalcule si nécessaire).
Retourne le nombre de scores chargés.
*/
int	load_scores(Score scores[])
{
	FILE	*f = fopen("score.txt", "r");

	if (!f) return 0;

	int		count = 0;
	char	line[256];

	while (count < MAX_SCORES && fgets(line, sizeof(line), f))
	{
		int		rank, taille, minutes, secondes, score;
		char	pseudo[25];

		// Nouveau format : rank pseudo taille score min sec  (6 champs)
		if (sscanf(line, "%d %24s %d %d %d %d", &rank, pseudo, &taille, &score, &minutes, &secondes) == 6)
		{
			strcpy(scores[count].pseudo, pseudo);
			scores[count].taille = taille;
			scores[count].temps = minutes * 60 + secondes;
			scores[count].score = score;
			count++;
		}
		// Ancien format : rank pseudo taille min sec  (5 champs)
		else if (sscanf(line, "%d %24s %d %d %d", &rank, pseudo, &taille, &minutes, &secondes) == 5)
		{
			strcpy(scores[count].pseudo, pseudo);
			scores[count].taille = taille;
			scores[count].temps = minutes * 60 + secondes;
			scores[count].score = calculer_score(taille, scores[count].temps); // recalcul
			count++;
		}
	}
	fclose(f);
	return count;
}


/*
Ajoute un score dans le tableau en mémoire.
Utile avant un tri.
*/
void	add_score(Score scores[], int *count, const char *pseudo, int temps)
{
	strcpy(scores[*count].pseudo, pseudo);
	scores[*count].temps = temps;
	(*count)++;
}


/*
Calcule le score final en fonction de la taille de la grille et du temps réalisé.
Plus la grille est grande et plus le temps est faible, plus le score sera élevé.
Utilise une fonction mathématique (accessible via le lien Géogébra transmis dans le rapport).
*/
int calculer_score(int taille, int temps)
{
	const double	alpha = 3.2;  /* poids de la taille (plus grand => grandes grilles favorisées) */
	const double	beta  = 1.1;  /* poids du temps  (plus grand => vitesse plus importante) */
	const double	t0    = 20.0; /* amortit les runs très courts */
	const double	K     = 1000.0;

	double	num = K * pow((double)taille, alpha);
	double	den = pow((double)temps + t0, beta);

	int		score = (int)(num / den);
	if (score < 0) score = 0;
	return score;
}


/*
Trie le tableau de scores par score décroissant.
En cas d'égalité, le meilleur est celui avec le temps le plus faible.
*/
void	sort_scores(Score scores[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (scores[j].score > scores[i].score || (scores[j].score == scores[i].score && scores[j].temps < scores[i].temps))
			{
				Score	tmp = scores[i];
				scores[i] = scores[j];
				scores[j] = tmp;
			}
		}
	}
}


/*
Sauvegarde les scores dans le fichier "score.txt" au format texte.
Ecrase le fichier existant afin de conserver un classement propre et trié.
*/
void	save_scores(Score scores[], int count, int taille)
{
	(void)taille; // si tu ne l'utilises pas, évite warning

	FILE	*f = fopen("score.txt", "w");
	if (!f) return;

	for (int i = 0; i < count; i++)
	{
		fprintf(f, "%d %s %d %d %d %d\n",
            i + 1,
            scores[i].pseudo,
            scores[i].taille,
            scores[i].score,
            scores[i].temps / 60,
            scores[i].temps % 60
		);
	}

	fclose(f);
}

/*
Ajoute un nouveau score (pseudo/temps/taille), recalcule le score, trie le classement
et sauvegarde le résultat en conservant au maximum MAX_SCORES.
Utilise les fonctions définies au-dessus pour faire ça.
*/
void	update_scores(const char *pseudo, int temps, int taille)
{
	Score	scores[MAX_SCORES + 1];
	int	count = load_scores(scores);

	strcpy(scores[count].pseudo, pseudo);
	scores[count].temps = temps;
	scores[count].taille = taille;
	scores[count].score = calculer_score(taille, temps);
	count++;

	sort_scores(scores, count);

	if (count > MAX_SCORES)
		count = MAX_SCORES;

	save_scores(scores, count, taille);
}


/*
Lance une partie :
--> Choix de la taille
--> Sélection aléatoire d'une grille
--> Chargement de la grille
--> Execution du jeu.
Si victoire, met à jour le classement et affiche les scores en fin de partie.
*/
void	lancer_partie(const char *pseudo)
{
	Grille	g;
	int		taille = choisir_taille_grille();
	// Génération d'un nombre aléatoire entre 1 et 10
	srand(time(NULL));
	int		num_grille = rand() % 10 + 1;

	// Construction du chemin du fichier
	char	chemin[256];
	snprintf(chemin, sizeof(chemin), "grilles/%d/g%d_%02d.txt", taille, taille, num_grille);

	if (!lireGrilleFromFichier(chemin, &g))
	{
		clear();
		mvprintw(0, 0, "Erreur chargement grille : %s", chemin);
		refresh();
		getch();
		return;
	}

	init_colors_cases();

	time_t	debut = time(NULL);

	if (game_loop(&g, &debut) == 1)
	{
		time_t	fin = time(NULL);
		int		temps = (int)(fin - debut);
		update_scores(pseudo, temps, g.taille);
		refresh();
		afficher_scores();
	}
}



void	afficher_scores_in_game(Grille *g)
{
	FILE	*f = fopen("score.txt", "r");
	char	line[256];
	int		y = g->taille + 6;
	int		tap;

	flushinp();

	if (!f)
	{
		mvprintw(g->taille + 5, 0, "Erreur d'ouverture du fichier score.txt");
		refresh();
		getch();
		return;
	}

	refresh();
	mvprintw(g->taille + 5, 0, "=== SCORES ===");

	while (fgets(line, sizeof(line), f))
	{
		mvprintw(y++, 0, "%s", line);
	}
	fclose(f);
}
