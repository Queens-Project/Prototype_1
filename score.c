#include "proto.h"

/*
Ce fichier contient les fonctions qui gère le système de scores du jeu :
 - Calcul du score
 - Tri des scores
 - Chargement/Sauvegarde dans un fichier texte
 - Affichage des scores en menu ou en fin de partie
*/




/*
Fonction	: afficher_scores
Auteur		: Patata_Games
Param		: NONE
Traitement	: Charge et affiche le classement des meilleurs scores, permet de revenir au menu ou d'effacer tous les scores
Retour		: NONE
*/
void	afficher_scores(void)
{
	int	tap;

	while (1)
	{
		Score scores[MAX_SCORES];
		int count = load_scores(scores);

		clear();

		int	y = 0;
		mvprintw(y++, 0, "=== SCORES ===");
		y++;

		if (count == 0)
			mvprintw(y++, 0, "(Aucun score)");
		else
		{
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
		}

		mvprintw(y + 2, 0, "ECHAP : revenir      * : effacer tous les scores");
		refresh();
		
		tap = getch();

		if (tap == KEY_ESC)
			break;

		if (tap == '*' || tap == 42)
		{
			clear_scores();
			mvprintw(y + 4, 0, "Scores effaces.");
			refresh();
			flushinp();
		}
	}

	clear();
	refresh();
}



/*
Fonction	: load_scores()
Auteur		: Patata_Games
Param		: scores est un tableau de Score à remplir (cf. proto.h)
Traitement	: Lit le fichier "score.txt" et charge les scores en mémoire (gère deux formats et recalcule si nécessaire)
Retour		: Retourne le nombre de scores chargés
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
		// Cet 'ancien format' est le format demandé par le cahier des charges
		// Notre 'nouveau format' prend en charge un score calculé en points
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
Fonction	: add_score()
Auteur		: Patata_Games
Param		: scores est un tableau de Score ; count est le nombre actuel d'entrées ; pseudo est le nom du joueur ; temps est le temps réalisé (en secondes)
Traitement	: Ajoute une entrée au tableau de scores (pseudo/temps) avant un éventuel tri
Retour		: NONE
*/
void	add_score(Score scores[], int *count, const char *pseudo, int temps)
{
	strcpy(scores[*count].pseudo, pseudo);
	scores[*count].temps = temps;
	(*count)++;
}



/*
Fonction	: calculer_score()
Auteur		: Patata_Games
Param		: taille est la taille de la grille ; temps est le temps réalisé (en secondes)
Traitement	: Calcule un score à partir d'une formule mathématique dépendant de la taille et du temps (grille plus grande et temps plus faible => score plus élevé)
Retour		: Retourne le score final (entier positif ou nul)
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
Fonction	: sort_scores
Auteur		: Patata_Games
Param		: scores est un tableau de Score ; n est le nombre d'éléments à trier
Traitement	: Trie les scores par score décroissant ; en cas d'égalité, favorise le temps le plus faible
Retour		: NONE
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
Fonction	: save_scores()
Auteur		: Patata_Games
Param		: scores est un tableau de Score ; count est le nombre d'éléments ; taille est la taille de grille (non utilisé ici)
Traitement	: Sauvegarde le classement dans "score.txt" au format texte en écrasant le fichier existant
Retour		: NONE
*/
void	save_scores(Score scores[], int count, int taille)
{
	(void)taille;

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
Fonction	: update_scores()
Auteur		: Patata_Games
Param		: pseudo est le nom du joueur ; temps est le temps réalisé (en secondes) ; taille est la taille de la grille
Traitement	: Ajoute le nouveau score, calcule sa valeur, trie le classement, conserve au plus MAX_SCORES entrées et sauvegarde dans le fichier
Retour		: NONE
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
Fonction	: lancer_partie()
Auteur		: Patata_Games
Param		: pseudo est le nom du joueur
Traitement	: Lance une partie complète : choix taille, sélection aléatoire d'une grille, chargement, lancement du jeu et mise à jour des scores en cas de victoire
Retour		: NONE
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



/*
Fonction	: clear_scores()
Auteur		: Patata_Games
Param		: NONE
Traitement	: Efface tous les scores en réinitialisant le fichier "score.txt"
Retour		: NONE
*/
void	clear_scores(void)
{
	FILE	*f = fopen("score.txt", "w");
	if (f != NULL)
		fclose(f);
}
