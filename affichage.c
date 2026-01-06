#include "proto.h"

void init_colors_cases(void)
{
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
}


void afficherGrilleNcurses(const Grille *g, int x, int y)
{
    for (int i = 0; i < g->taille; i++)
    {
        for (int j = 0; j < g->taille; j++)
        {
            int region = g->regions[i][j];

            /* sécurité */
            if (region < 0) region = 0;
            if (region > 7) region = 7;

            int pair = 1 + region;
            int is_cursor = (i == x && j == y);

            /* --- Choix des attributs --- */
            if (is_cursor)
            {
                if (pair == 7)  /* région noire: pair 7 = noir/noir */
                {
                    /* curseur visible: fond blanc */
                    attron(COLOR_PAIR(8)); /* noir sur blanc */
                }
                else
                {
                    /* curseur classique: reverse sur la couleur de région */
                    attron(COLOR_PAIR(pair));
                    attron(A_REVERSE);
                }
            }
            else
            {
                /* case normale */
                attron(COLOR_PAIR(pair));
            }

            /* --- Dessin de la case --- */
            mvprintw(i, j * 3, "   ");

            if (g->etat[i][j] == QUEEN)
                mvaddch(i, j * 3 + 1, 'Q');
            else if (g->etat[i][j] == MARK)
                mvaddch(i, j * 3 + 1, 'X');

            /* --- Reset attributs --- */
            if (is_cursor)
            {
                if (pair == 7)
                {
                    attroff(COLOR_PAIR(8));
                }
                else
                {
                    attroff(A_REVERSE);
                    attroff(COLOR_PAIR(pair));
                }
            }
            else
            {
                attroff(COLOR_PAIR(pair));
            }
        }
    }
}


// void afficherGrilleNcurses(const Grille *g, int x, int y)
// {
//     for (int i = 0; i < g->taille; i++)
//     {
//         for (int j = 0; j < g->taille; j++)
//         {
//             int region = g->regions[i][j];

//             /* sécurité */
//             if (region < 0) region = 0;
//             if (region > 7) region = 7;

//             int pair = 1 + region;
//             int is_cursor = (i == x && j == y);

//             attron(COLOR_PAIR(pair));
//             if (is_cursor)
//                 attron(A_REVERSE);

//             mvprintw(i, j * 3, "   ");

//             if (g->etat[i][j] == QUEEN)
// 				mvaddch(i, j * 3 + 1, 'Q');
// 			else if (g->etat[i][j] == MARK)
// 				mvaddch(i, j * 3 + 1, 'X');
//             if (is_cursor)
//                 attroff(A_REVERSE);
//             attroff(COLOR_PAIR(pair));
//         }
//     }
// }
