#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <assert.h>
#include "proto.h"
#define MAX_SCORES 45

void afficher_scores()
{
    FILE *f = fopen("score.txt", "r");
    int y = 0;
    int tap = 0;

    clear();

    if (!f)
    {
        mvprintw(0, 0, "Erreur d'ouverture du fichier score.txt");
        refresh();
        getch();
        return;
    }

    mvprintw(y++, 0, "=== SCORES ===");
    y++;

    int rank, taille, minutes, secondes;
    char pseudo[25];

    while (fscanf(f, "%d %24s %d %d %d",
                   &rank,
                   pseudo,
                   &taille,
                   &minutes,
                   &secondes) == 5)
    {
        mvprintw(y++, 0,
                 "%d. %-10s | %dX%d | %d:%02d",
                 rank,
                 pseudo,
                 taille,
                 taille,
                 minutes,
                 secondes);
    }

    fclose(f);

    mvprintw(y + 2, 0, "Appuyez sur Q pour revenir");
    refresh();

    while (tap != 'q' && tap != 'Q')
        tap = getch();

    clear();
    refresh();
}


int load_scores(Score scores[])
{
    FILE *f = fopen("score.txt", "r");
    if (!f) return 0;

    int count = 0;
    int rank, min, sec, taille;
    char pseudo[25];

    while (count < MAX_SCORES &&
           fscanf(f, "%d %24s %d %d %d",
                  &rank,
                  pseudo,
                  &taille,
                  &min,
                  &sec) == 5)
    {
        strcpy(scores[count].pseudo, pseudo);
        scores[count].taille = taille;
        scores[count].temps = min * 60 + sec;
        count++;
    }

    fclose(f);
    return count;
}


void	add_score(Score scores[], int *count, const char *pseudo, int temps)
{
	strcpy(scores[*count].pseudo, pseudo);
	scores[*count].temps = temps;
	(*count)++;
}


void	sort_scores(Score scores[], int n)
{
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n; j++)
			if (scores[j].temps < scores[i].temps)
			{
				Score tmp = scores[i];
				scores[i] = scores[j];
				scores[j] = tmp;
			}
}

void save_scores(Score scores[], int count, int taille)
{
    FILE *f = fopen("score.txt", "w");
    if (!f)
        return;

    for (int i = 0; i < count; i++)
    {
        fprintf(f, "%d %s %d %d %d\n",
            i + 1,
            scores[i].pseudo,
            scores[i].taille,
            scores[i].temps / 60,
            scores[i].temps % 60
        );
    }

    fclose(f);
}


void update_scores(const char *pseudo, int temps, int taille)
{
    Score scores[MAX_SCORES + 1];
    int count = load_scores(scores);

    strcpy(scores[count].pseudo, pseudo);
    scores[count].temps = temps;
    scores[count].taille = taille;
    count++;

    sort_scores(scores, count);

    if (count > MAX_SCORES)
        count = MAX_SCORES;

    save_scores(scores, count, taille);
}



void	lancer_partie(const char *pseudo)
{
	Grille g;
	int	taille = choisir_taille_grille();
	// Génération d'un nombre aléatoire entre 1 et 10
	srand(time(NULL));
	int	num_grille = rand() % 10 + 1;

    // Construction du chemin du fichier
	char	chemin[256];
	snprintf(chemin, sizeof(chemin), "grilles/%d/g%d_%02d.txt", taille, taille, num_grille);

    // Lecture de la grille
	if (!lireGrilleFromFichier(chemin, &g))
	{
		clear();
		mvprintw(0, 0, "Erreur chargement grille : %s", chemin);
		refresh();
		getch();
		return;
	}

	init_colors_cases();

	// lancement de la boucle de jeu & du temps
	time_t debut = time(NULL);

	// si victoire alors on enregistre le score
	if (game_loop(&g, debut) == 1)
	{
		time_t fin = time(NULL);
		int temps = (int)(fin - debut);
		update_scores(pseudo, temps, g.taille);
		afficher_scores_in_game(&g);
	}
	

	
}

void afficher_scores_in_game(Grille *g)
{
	FILE	*f = fopen("score.txt", "r");
	char	line[256];
	int	y = g->taille + 6;
	int	tap;

	flushinp();

	if (!f)
	{
		mvprintw(g->taille + 5, 0, "Erreur d'ouverture du fichier score.txt");
		refresh();
		getch();
		return;
	}

	mvprintw(g->taille + 5, 0, "=== SCORES ===");

	while (fgets(line, sizeof(line), f))
	{
		mvprintw(y++, 0, "%s", line);
	}

	fclose(f);
}
