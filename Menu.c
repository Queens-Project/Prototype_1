#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <assert.h>
#include "proto.h"

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
    FILE *f = fopen("regle_du_jeu.txt", "r");
    char line[256];
    int y = 0;
    int tap = 0;

    clear();
	flushinp();

    if (!f)
    {
        mvprintw(0, 0, "Erreur d'ouverture du fichier regle_du_jeu.txt");
        mvprintw(2, 0, "Appuyez sur une touche pour revenir");
        refresh();
        getch();
        return;
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    while (fgets(line, sizeof(line), f) && y < rows - 2)
    {
        mvprintw(y++, 0, "%s", line);
    }

    fclose(f);

    mvprintw(y + 1, 0, "Appuyez sur Q pour revenir au menu");
    refresh();

    while ((tap = getch()) != 'q' && tap != 'Q')
        ;

	if (tap == 'q' || tap == 'Q')
	{
		clear();
		refresh();
	}
}

void menu()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int tap;

    while (1)
    {
        clear();
        mvprintw(0,0,"=== MENU PRINCIPAL ===");
        mvprintw(2,0,"(N) Nouvelle Partie");
        mvprintw(3,0,"(S) Afficher les Scores");
        mvprintw(4,0,"(R) Regles du Jeu");
        mvprintw(5,0,"(Q) Quitter");
        refresh();

        tap = getch();

        switch (tap)
        {
            case 'n':
            case 'N':
                clear();
                refresh();
                crea_pseudo();
                break;

            case 's':
            case 'S':
                clear();
                refresh();
                afficher_scores();
                break;

            case 'r':
            case 'R':
                clear();
                refresh();
                regles_du_jeu();
                break;

            case 'q':
            case 'Q':
                endwin();
                return;

            default:
                mvprintw(9, 0, "Touche invalide. Appuyez sur une touche...");
                refresh();
                getch();
                break;
        }
    }
}

int	main()
{
	menu();
	return 0;
}
