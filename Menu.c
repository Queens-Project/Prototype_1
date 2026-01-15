#include "proto.h"

/*
Ce fichier gère l’ensemble des menus du jeu (menu principal, choix 
de la taille de la grille, affichage des règles) et les interactions 
utilisateur associées avant le lancement d’une partie.
*/



/*
Fonction	: choisir_taille_grille()
Auteur		: Patata_Games
Param		: NONE
Traitement	: Permet au joueur de sélectionner une taille de grille (entre 5 et N) via le clavier, avec une valeur par défaut
Retour		: Retourne la taille choisie (entier)
*/
int	choisir_taille_grille(void)
{
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
		if (ch == '\n' || ch == KEY_ENTER)
			return taille;
		if (ch == 'm' || ch == 'M')
			break;
	}

	while (1)
	{
		char	buf[8];

		clear();
		mvprintw(0, 0, "Choisissez une taille de grille (entre 5 et %d)", N);
		mvprintw(2, 0, "Entrez un nombre puis appuyez sur ENTREE : ");
		refresh();

		echo();
		getnstr(buf, (int)sizeof(buf) - 1);
		noecho();

		int	v = atoi(buf);
		if (v >= 5 && v <= N)
			return v;

		mvprintw(4, 0, "Taille invalide. Appuyez sur une touche...");
		refresh();
		getch();
	}
}


/*
Fonction	: crea_pseudo()
Auteur		: Patata_Games
Param		: NONE
Traitement	: Demande au joueur un pseudo au clavier, remplace les espaces par des '_' puis lance une partie
Retour		: NONE
*/
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



/*
Fonction	: strip_crlf
Auteur		: Patata_Games
Param		: s est une chaîne de caractères modifiable
Traitement	: Supprime les caractères de fin de ligne (\n et/ou \r) en plaçant un '\0' au premier CR/LF rencontré
Retour		: NONE
*/
void	strip_crlf(char *s)
{
	s[strcspn(s, "\r\n")] = '\0';
}



/*
Fonction	: regles_du_jeu
Auteur		: Patata_Games
Param		: NONE
Traitement	: Ouvre et affiche le fichier "regle_du_jeu.txt" dans la console avec défilement si nécessaire, puis attend ECHAP pour revenir
Retour		: NONE
*/
void	regles_du_jeu(void)
{
	FILE	*f = fopen("regle_du_jeu.txt", "r");
	int	rows, cols;
	char	line[512];

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

	if (rows < 6 || cols < 30)
	{
		mvprintw(0, 0, "Fenetre trop petite (%dx%d). Agrandissez le terminal.", rows, cols);
		mvprintw(2, 0, "Appuyez sur une touche pour revenir...");
		refresh();
		getch();
		fclose(f);
		return;
	}

	erase();
	refresh();

	scrollok(stdscr, TRUE);
	idlok(stdscr, TRUE);// <-- permet à ncurses d'optimiser le scroll

	int	max_text_rows = rows - 2;
	int	y = 0;

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
		clrtoeol();/*<-- supprime les restes d'une ancienne ligne plus longue */
		printw("%s", line);
		y++;
	}

	fclose(f);

	/* Ligne d'aide en bas */
	move(rows - 1, 0);
	clrtoeol();
	mvprintw(rows - 1, 0, "Appuyez sur ECHAP pour revenir au menu");
	refresh();

	int	ch;
	while (1)
	{
		ch = getch();
		if((ch) == KEY_ESC)
		break;
	}
}


/*
Fonction	: menu
Auteur		: Patata_Games
Param		: NONE
Traitement	: Initialise ncurses, affiche le menu principal et gère les choix utilisateur (nouvelle partie, scores, règles, quitter)
Retour		: NONE
*/
void	menu()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	set_escdelay(25);


	int	tap;

	while (1)
	{
		clear();
		mvprintw(0,0,"=== MENU PRINCIPAL ===");
		mvprintw(2,0,"- N ------ Nouvelle Partie");
		mvprintw(3,0,"- S ------ Afficher les Scores");
		mvprintw(4,0,"- R ------ Regles du Jeu");
		mvprintw(5,0,"- ECHAP -- Quitter");
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

			case KEY_ESC:
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



/*
Fonction	: main()
Auteur		: Patata_Games
Param		: NONE
Traitement	: Point d'entrée du programme : lance le menu principal
Retour		: Retourne 0 à la fin de l'exécution
*/
int	main()
{
	menu();
	return 0;
}
