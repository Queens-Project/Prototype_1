#include "proto.h"


void	init_colors_cases()
{
	start_color();

	init_pair(1, COLOR_BLACK,   COLOR_RED);
	init_pair(2, COLOR_BLACK,   COLOR_GREEN);
	init_pair(3, COLOR_BLACK,   COLOR_YELLOW);
	init_pair(4, COLOR_BLACK,   COLOR_BLUE);
	init_pair(5, COLOR_BLACK,   COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK,   COLOR_CYAN);
	init_pair(7, COLOR_BLACK,   COLOR_BLACK);
	init_pair(8, COLOR_BLACK,   COLOR_WHITE);
}



void	afficherGrilleNcurses(const Grille *g, int x, int y)
{
	for (int i = 0; i < g->taille; i++)
	{
		for (int j = 0; j < g->taille; j++)
		{
			int	region = g->regions[i][j];      // 0..7 en pratique
			int	pair = 1 + region ;        // COLOR_PAIR(1..8)

			if (i==x && j==y)
				attron(COLOR_PAIR(8));
			else
				attron(COLOR_PAIR(pair));
			mvprintw(i, j*3,"   ");
			if (i==x && j==y)
				mvprintw(i,j*3, " X ");
			attroff(COLOR_PAIR(pair));
			attroff(COLOR_PAIR(8));
		}
	}
}

