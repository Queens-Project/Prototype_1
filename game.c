#include "proto.h"

const char	*queen_error_msg(QueenError err)
{
	if (err == PLACEMENT_OK)
		return "OK";
	if (err == ERROR_OUT_OF_BOUNDS)
		return "Hors de la grille";
	if (err == ERROR_OCCUPIED)
		return "Case deja occupee";
	if (err == ERROR_ADJACENT)
		return "Reine adjacente";
	if (err == ERROR_LINE)
		return "Reine deja sur la ligne";
	if (err == ERROR_COLUMN)
		return "Reine deja sur la colonne";
	if (err == ERROR_REGION)
		return "Reine deja dans la region";
	return ("Erreur inconnue");
}


/*Place une reine ou la retire
	- Si déjà une reine --> retire la reine
	- Sinon --> vérification de placement possible et si oui alors place la reine
	*/
QueenError	placer_queen(Grille *g, int line, int col)
{
	if (!inside_grille(g, line, col))
		return ERROR_OUT_OF_BOUNDS;

	if (g->etat[line][col] == QUEEN)
	{
		g->etat[line][col] = VIDE;
		return PLACEMENT_OK;
	}

	QueenError err = placement_queen(col, line, g);
	if (err == PLACEMENT_OK)
		g->etat[line][col] = QUEEN;

	return (err);
}


/*Place une marque ou la retire
	- Si déjà une marque --> retire la marque
	- Si vide --> place une marque
	- Si Reine --> refus
	*/
QueenError	placer_marque(Grille *g, int line, int col)
{
	if (!inside_grille(g, line, col))
		return ERROR_OUT_OF_BOUNDS;

	if (g->etat[line][col] == QUEEN)
		return ERROR_OCCUPIED;

	if (g->etat[line][col] == MARK)
		g->etat[line][col] = VIDE;
	else
		g->etat[line][col] = MARK;

	return PLACEMENT_OK;
}



int	game_loop(Grille *g, time_t *debut)
{
	int	running = 1;
	int	line = 0;
	int	col = 0;
	

	keypad(stdscr, TRUE);

	while (running)
	{
		clear();
		afficherGrilleNcurses(g, line, col);

		int	rows, cols;
		getmaxyx(stdscr, rows, cols);

		int elapsed = (int)(time(NULL) - *debut);
		int	mm = elapsed / 60;
		int	ss = elapsed % 60;

		char	buf[32];
		snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);

		int	x = cols - (int)strlen(buf) - 70;
		if (x < 0)
			x = 0;

		attrset(A_NORMAL);
		attron(A_REVERSE | A_BOLD);

		int	rc = mvprintw(0, x, "%s", buf);

		attroff(A_REVERSE | A_BOLD);
		attrset(A_NORMAL);

		mvprintw(g->taille+1, 0,
             "Fleches: bouger | Espace/Entree: Reine | X: marque \nQ: quitter | P: Pause");

		refresh();
		int	ch = getch();

		if (ch == 'q' || ch == 'Q')
			return 0;

		/* --- PAUSE --- */
		else if (ch == 'p' || ch == 'P')
		{
			/* On fige le temps et on masque la grille */
			time_t pause_start = time(NULL);
			int elapsed_frozen = (int)(pause_start - *debut);

			while (1)
			{
				clear();
				afficherGrilleBlanche(g);

				int rows2, cols2;
				getmaxyx(stdscr, rows2, cols2);

				int mm2 = elapsed_frozen / 60;
				int ss2 = elapsed_frozen % 60;
				char buf2[32];
				snprintf(buf2, sizeof(buf2), "%02d:%02d", mm2, ss2);

				int x2 = cols2 - (int)strlen(buf2) - 70;
				if (x2 < 0) x2 = 0;

				attrset(A_NORMAL);
				attron(A_REVERSE | A_BOLD);
				mvprintw(0, x2, "%s", buf2);
				attroff(A_REVERSE | A_BOLD);
				attrset(A_NORMAL);

				mvprintw(g->taille + 1, 0, "=== PAUSE ===  P: reprendre | Q: quitter");
				refresh();

				int chp = getch();
				if (chp == 'q' || chp == 'Q')
					return 0;
				if (chp == 'p' || chp == 'P')
					break;
			}

			/* On decale le debut pour exclure la duree de pause */
			*debut += (time(NULL) - pause_start);
			continue;
		}


		else if (ch == KEY_UP && line>0)
			line--;
		else if (ch == KEY_DOWN && line < g->taille - 1)
			line++;
		else if (ch == KEY_LEFT && col > 0)
			col--;
		else if (ch == KEY_RIGHT && col < g->taille - 1)
			col++;

		else if (ch == 'x' || ch == 'X')
		{
			QueenError err = placer_marque(g, line, col);
			if (err != PLACEMENT_OK)
			{
				mvprintw(g->taille+3, 0, "Marque impossible: %s\n", queen_error_msg(err));
				mvprintw(g->taille+4, 0, "Appuie sur une touche...\n");
				refresh();
				getch();
			}
		}

		else if (ch ==' ' || ch=='\n')
		{
			QueenError err = placer_queen(g, line, col);

			if (err != PLACEMENT_OK)
			{
				mvprintw(g->taille+3, 0, "Placement refusé: %s\n", queen_error_msg(err));
				mvprintw(g->taille+4, 0, "Appuie sur une touche...\n");
				refresh();
				getch();
			}
			else
			{
				if (validation(g))
				{
					clear();
					afficherGrilleNcurses(g, line, col);
					mvprintw(g->taille+3, 0, "VICTOIRE ! Appuie sur une touche...");
					refresh();
					getch();
					return 1;
				}
			}
		}
	}
	return (0);
}