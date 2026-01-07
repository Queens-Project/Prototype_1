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


static void strip_crlf(char *s)
{
    /* Supprime \r et \n (CRLF Windows ou LF Linux) */
    s[strcspn(s, "\r\n")] = '\0';
}

void regles_du_jeu(void)
{
    FILE *f = fopen("regle_du_jeu.txt", "r");
    int rows, cols;
    char line[512];

    clear();
    refresh();
    flushinp();

    getmaxyx(stdscr, rows, cols);

    if (!f)
    {
        mvprintw(0, 0, "Erreur d'ouverture du fichier regle_du_jeu.txt");
        mvprintw(2, 0, "Appuyez sur une touche pour revenir...");
        refresh();
        getch();
        return;
    }

    /* Terminal trop petit : message clair */
    if (rows < 6 || cols < 30)
    {
        mvprintw(0, 0, "Fenetre trop petite (%dx%d). Agrandissez le terminal.", rows, cols);
        mvprintw(2, 0, "Appuyez sur une touche pour revenir...");
        refresh();
        getch();
        fclose(f);
        return;
    }

    /* Évite les artefacts : on part d'un écran propre */
    erase();
    refresh();

    /* Autorise le scroll si le texte dépasse */
    scrollok(stdscr, TRUE);
    idlok(stdscr, TRUE);      /* permet à ncurses d'optimiser le scroll */

    int max_text_rows = rows - 2;   /* dernière ligne réservée au prompt */
    int y = 0;

    while (fgets(line, sizeof(line), f))
    {
        strip_crlf(line);

        /* Si la ligne est plus longue que l'écran, on la tronque proprement */
        if ((int)strlen(line) > cols - 1)
            line[cols - 1] = '\0';

        /* Si on arrive en bas, on scrolle automatiquement */
        if (y >= max_text_rows)
        {
            scroll(stdscr);
            y = max_text_rows - 1;
            move(y, 0);
            clrtoeol();
        }

        move(y, 0);
        clrtoeol();                 /* IMPORTANT : supprime les restes d'une ancienne ligne plus longue */
        printw("%s", line);
        y++;
    }

    fclose(f);

    /* Ligne d'aide en bas */
    move(rows - 1, 0);
    clrtoeol();
    mvprintw(rows - 1, 0, "Appuyez sur Q pour revenir au menu");
    refresh();

    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') { }
}


void	menu()
{
	setlocale(LC_ALL, "");
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
		mvprintw(3,0,"(S) Afficher les Scores");
		mvprintw(4,0,"(R) Règles du Jeu");
		mvprintw(5,0,"(Q) Quitter");
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
