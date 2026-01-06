#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <assert.h>
#include "proto.h"



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
}



void	regles_du_jeu()
{
		FILE	*f = fopen("regle_du_jeu.txt", "r");
		char	line[256];

		int y = 0;
		int tap = 0;
		clear();

		if (!f)
		{
			mvprintw(0, 0, "Erreur d'ouverture du fichier regles.txt");
			mvprintw(2, 0, "Appuyez sur une touche pour revenir");
			refresh();
			getch();
			return;
		}

		while (fgets(line, sizeof(line), f))
		{
			mvprintw(y++, 0, "%s", line);
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
		}
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
