#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <assert.h>
#include "proto.h"
#define MAX_SCORES 15



void	load_game()
{
	printf("Game loaded\n");
}

int	choisir_taille_grille(void){
	int	taille = 5;
	int	ch;

	clear();
	mvprintw(0, 0, "Taille de grille par defaut : 5x5");
	mvprintw(2, 0, "Appuyez sur ENTREE pour continuer");
	mvprintw(3, 0, "Ou appuyez sur (M) pour modifier la taille");
	refresh();

	while (1)
	{
		ch = getch();

		/* ENTREE → taille par défaut */
		if (ch == '\n' || ch == KEY_ENTER)
			return taille;

		/* Modifier la taille */
		if (ch == 'm' || ch == 'M')
			break;
	}

    /* --- Choix manuel de la taille --- */
	clear();
	mvprintw(0, 0, "Choisissez une taille de grille (5 a 8) : ");
	refresh();

	while (1)
	{
		ch = getch();
		if (ch >= '5' && ch <= '8')
		{
			taille = ch - '0';
			return taille;
		}
	}
}


void	afficher_scores()
{
	FILE	*f = fopen("score.txt", "r");
	char	line[256];

	int	y = 0;
	int	tap = 0;

	clear();

	if (!f)
	{
		mvprintw(0, 0, "Erreur d'ouverture du fichier score.txt");
		mvprintw(2, 0, "Appuyez sur une touche pour revenir");
		refresh();
		getch();
		return;
	}

	mvprintw(y++, 0, "=== SCORES ===");
	y++;

	while (fgets(line, sizeof(line), f))
	{
		char	*p = line;
		int	rank, minutes, secondes;
		char	pseudo[25];
		pseudo[0] = '\0';

		// --- Récupération du rang ---
		while (*p && !isdigit(*p))
			p++;
		rank = atoi(p);
		while (*p && isdigit(*p))
			p++;

		// --- Récupération du nom ---
		while (*p && !isalpha(*p))
			p++;
		while (*p && (isalpha(*p) || *p == '_'))
		{
			strncat(pseudo, p, 1);
			p++;
		}


		// --- Récupération des minutes ---
		while (*p && !isdigit(*p))
			p++;
		minutes = atoi(p);
		while (*p && isdigit(*p))
			p++;

		// --- Récupération des secondes ---
		while (*p && !isdigit(*p))
			p++;
		secondes = atoi(p);

		// --- Affichage ncurses ---
		mvprintw(y++, 0,
                 "%d. %-10s %d min %02d sec",
                 rank, pseudo, minutes, secondes);
	}

	fclose(f);

	mvprintw(y + 2, 0, "Appuyez sur Q pour revenir au menu");
	refresh();

	while (tap != 'q' && tap != 'Q')
	{
		tap = getch();
	}
	if(tap=='q' || tap=='Q'){
		clear();
		refresh();
		menu();
	}
}


int	load_scores(Score scores[])
{
	FILE	*f = fopen("score.txt", "r");
	if (!f) return 0;

	int	count = 0, rank, min, sec;
	char	pseudo[25];

	while (count < MAX_SCORES && fscanf(f, "%d. %24s %d min %d sec", &rank, pseudo, &min, &sec) == 4)
	{
		strcpy(scores[count].pseudo, pseudo);
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


void	save_scores(Score scores[], int count)
{
	FILE	*f = fopen("score.txt", "w");
	if (!f)
		return;

	for (int i = 0; i < count; i++) {
		fprintf(f, "%d. %s %d min %d sec\n",
                i + 1,
                scores[i].pseudo,
                scores[i].temps / 60,
                scores[i].temps % 60);
	}

	fclose(f);
}


void	update_scores(const char *pseudo, int temps)
{
	Score scores[MAX_SCORES + 1];
	int count = load_scores(scores);

	add_score(scores, &count, pseudo, temps);
	sort_scores(scores, count);

	if (count > MAX_SCORES)
		count = MAX_SCORES;

	save_scores(scores, count);
}


void	lancer_partie(const char *pseudo)
{
	Grille g;
	int	taille = choisir_taille_grille();
	// --- Génération d'un nombre aléatoire entre 1 et 10 ---
	srand(time(NULL));
	int	num_grille = rand() % 10 + 1;  // 1 à 10

    // --- Construction du chemin du fichier ---
	char	chemin[256];
	snprintf(chemin, sizeof(chemin), "grilles/%d/g%d_%02d.txt", taille, taille, num_grille);

    // --- Lecture de la grille ---
	if (!lireGrilleFromFichier(chemin, &g))
	{
		clear();
		mvprintw(0, 0, "Erreur chargement grille : %s", chemin);
		refresh();
		getch();
		return;
	}

	init_colors_cases();

	time_t debut = time(NULL);
	game_loop(&g, debut);
	time_t fin = time(NULL);

	int temps = (int)(fin - debut);

	update_scores(pseudo, temps);
}



void	crea_pseudo()
{
	char	pseudo[25];

	clear();
	mvprintw(0, 0, "Entrez votre pseudo : ");
	refresh();

	echo();
	getnstr(pseudo, 24);
	noecho();

	for (int i = 0; pseudo[i]; i++)
		if (pseudo[i] == ' ')
			pseudo[i] = '_';

	lancer_partie(pseudo);
	menu();
}



void	regles_du_jeu()
{
		printf("Game rules\n");
}

void	menu()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int tap = 0;

	while(1)
	{
		clear();
		mvprintw(0,0,"=== MENU PRINCIPAL ===");
		mvprintw(2,0,"(N) Nouvelle Partie");
		mvprintw(3,0,"(C) Charger une Partie");
		mvprintw(4,0,"(S) Afficher les Scores");
		mvprintw(5,0,"(R) Règles du Jeu");
		mvprintw(6,0,"(Q) Quitter");
		refresh();

		tap = getch();

		int	running = 1;
		while(running){
			if(tap=='n' || tap=='N')
			{
				running = 0;
				clear();
				refresh();
				crea_pseudo();
			}
			else if (tap=='c' || tap=='C')
			{
				running = 0;
				clear();
				refresh();
				load_game();
			}
			else if(tap=='s' || tap=='S')
			{
				running = 0;
				clear();
				refresh();
				afficher_scores();
			}
			else if(tap=='r' || tap=='R')
			{
				running = 0;
				clear();
				refresh();
				regles_du_jeu();
			}
			else if(tap=='q' || tap=='Q')
			{
				running = 0;
				endwin();
				return;
			}
		}
	}
}

int	main()
{
	menu();
	return 0;
}
