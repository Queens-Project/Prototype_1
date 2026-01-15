#include "proto.h"

/*Ce fichier contient la partie principale de l'algorithme de jeu,
ainsi que les fonctions utiles au jeu.*/



/*
Fonction	: queen_error_msg
Auteur		: Patata_Games
Param		: err est un message d'erreur (cf. proto.h)
Traitement	: Prend un message d'erreur 'brut' et renvoie un message 'soigné' prêt à étre affiché à l'utilisateur
Retour		: Une chaîne de caractère qui est un message d'erreur compréhensible par le joueur
*/
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



/*
Fonction	: placer_queen
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour ligne; col pour colonne
Traitement	: Place une Reine aux coordonnées (line,col) si possible
Retour		: Retourne err, unn message brut interprétable par le jeu (cf. structure QueenError dans proto.h)
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



/*
Fonction	: placer_marque
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h); line pour ligne; col pour colonne
Traitement	: Place une marque aux coordonnées (line, col) dans la grille g si possible
Retour		: Retourne err, unn message brut interprétable par le jeu (cf. structure QueenError dans proto.h)
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




void	draw_timer(time_t debut, int x_offset)
{
	int	rows, cols;
	getmaxyx(stdscr, rows, cols);

	int	elapsed = (int)(time(NULL) - debut);
	int	mm = elapsed / 60;
	int	ss = elapsed % 60;

	char	buf[32];
	snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);

	int	x = cols - (int)strlen(buf) - x_offset;
	if (x < 0) x = 0;

	attrset(A_NORMAL);
	attron(A_REVERSE | A_BOLD);
	mvprintw(0, x, "%s", buf);
	attroff(A_REVERSE | A_BOLD);
	attrset(A_NORMAL);
}



/*
Fonction	: handle_pause
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h) ; debut est le pointeur sur le temps de début de partie
Traitement	: Met le jeu en pause : affiche une grille blanche, fige le chronomètre et attend la reprise ou la sortie
Retour		: Retourne 1 si le joueur reprend la partie, 0 si le joueur quitte
*/
int	handle_pause(Grille *g, time_t *debut)
{
	time_t	pause_start = time(NULL);
	int		frozen_elapsed = (int)(pause_start - *debut);

	while (1)
	{
		clear();
		afficherGrilleBlanche(g);

		draw_timer(time(NULL) - frozen_elapsed, 70);

		mvprintw(g->taille + 1, 0, "=== PAUSE ===   P: reprendre | ECHAP: quitter");
		refresh();

		int	ch = getch();
		if ((ch) == KEY_ESC)
			return 0; /* quitter */
		if (ch == 'p' || ch == 'P')
			break; /* reprendre */
	}
	*debut += (time(NULL) - pause_start); /* exclue la durée de pause */
	return 1;
}



/*
Fonction	: game_loop
Auteur		: Patata_Games
Param		: g est une grille de type Grille (cf. proto.h) ; debut est le pointeur sur le temps de début de partie
Traitement	: Boucle principale de jeu : gère les déplacements, placements (reines/marques), pause, affichage et détection de victoire
Retour		: Retourne 1 si victoire, 0 si le joueur quitte
*/
int	game_loop(Grille *g, time_t *debut)
{
	int	line = 0;
	int	col = 0;

	keypad(stdscr, TRUE);

	while (1)
	{
		clear();
		afficherGrilleNcurses(g, line, col);

		draw_timer(*debut, 70);

		mvprintw(g->taille + 1, 0, "Fleches: bouger | Espace/Entree: Reine | X: marque \n\nECHAP: quitter | P: Pause");
		refresh();

		int	ch = getch();

		if ((ch) == KEY_ESC)
			return 0;

		if (ch == 'p' || ch == 'P')
		{
			if (!handle_pause(g, debut))
				return 0;
			continue;
		}

		if (ch == KEY_UP && line > 0) line--;
		else if (ch == KEY_DOWN && line < g->taille - 1) line++;
		else if (ch == KEY_LEFT && col > 0) col--;
		else if (ch == KEY_RIGHT && col < g->taille - 1) col++;

		else if (ch == 'x' || ch == 'X')
		{
			QueenError err = placer_marque(g, line, col);
			if (err != PLACEMENT_OK)
			{
				mvprintw(g->taille + 3, 0, "Marque impossible: %s\n", queen_error_msg(err));
				mvprintw(g->taille + 4, 0, "Appuie sur une touche...\n");
				refresh();
				getch();
			}
		}
		else if (ch == ' ' || ch == '\n')
		{
			QueenError err = placer_queen(g, line, col);
			if (err != PLACEMENT_OK)
			{
				mvprintw(g->taille + 3, 0, "Placement refusé: %s\n", queen_error_msg(err));
				mvprintw(g->taille + 4, 0, "Appuie sur une touche...\n");
				refresh();
				getch();
			}
			else if (validation(g))
			{
				clear();
				afficherGrilleNcurses(g, line, col);
				mvprintw(g->taille + 3, 0, "VICTOIRE ! Appuie sur une touche...");
				refresh();
				getch();
				return 1;
			}
		}
	}
}