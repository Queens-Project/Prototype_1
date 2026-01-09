#include "proto.h"

void init_colors_cases(void)
{
    start_color();

    /* Paires 1..10 : régions 0..9 */
    /* On essaie d'obtenir 10 couleurs de fond distinctes. */
    int bg[10];

    /* Base (0..7) : rouge, vert, jaune, bleu, magenta, cyan, noir, blanc */
    bg[0] = COLOR_RED;
    bg[1] = COLOR_GREEN;
    bg[2] = COLOR_YELLOW;
    bg[3] = COLOR_BLUE;
    bg[4] = COLOR_MAGENTA;
    bg[5] = COLOR_CYAN;
    bg[6] = COLOR_BLACK;
    bg[7] = COLOR_WHITE;

    /* Deux couleurs supplémentaires si possible */
    if (COLORS >= 16 && can_change_color())
    {
        /* Indices 8 et 9 disponibles en général si COLORS>=16 */
        /* Orange (approx) */
        init_color(8, 1000, 500, 0);
        /* Violet/rose (approx) */
        init_color(9, 700, 0, 700);

        bg[8] = 8;
        bg[9] = 9;
    }
    else
    {
        /* Fallback : on recycle (moins lisible, mais jouable partout) */
        bg[8] = COLOR_RED;
        bg[9] = COLOR_BLUE;
    }

    for (int r = 0; r < 10; r++)
    {
        /* Choix du texte selon fond (lisibilité) */
        int fg = COLOR_BLACK;
        if (bg[r] == COLOR_BLACK || bg[r] == COLOR_BLUE || bg[r] == COLOR_MAGENTA)
            fg = COLOR_WHITE;

        init_pair(1 + r, fg, bg[r]);
    }

    /* Paire 11 : noir sur blanc (pause + curseur sur fond noir) */
    init_pair(11, COLOR_BLACK, COLOR_WHITE);
}



void afficherGrilleNcurses(const Grille *g, int x, int y)
{
    for (int i = 0; i < g->taille; i++)
    {
        for (int j = 0; j < g->taille; j++)
        {
            int region = g->regions[i][j];

            /* sécurité : régions attendues 0..9 pour 10x10 */
            if (region < 0) region = 0;
            if (region > 9) region = 9;

            int pair = 1 + region;
            int is_cursor = (i == x && j == y);

            if (is_cursor)
            {
                /* Si la région est noire (dans notre mapping: region==6),
                   on force un curseur très visible */
                if (region == 6) /* fond noir */
                {
                    attron(COLOR_PAIR(11));
                }
                else
                {
                    attron(COLOR_PAIR(pair));
                    attron(A_REVERSE);
                }
            }
            else
            {
                attron(COLOR_PAIR(pair));
            }

            mvprintw(i, j * 3, "   ");

            if (g->etat[i][j] == QUEEN)
                mvaddch(i, j * 3 + 1, 'Q');
            else if (g->etat[i][j] == MARK)
                mvaddch(i, j * 3 + 1, 'X');

            if (is_cursor)
            {
                if (region == 6)
                    attroff(COLOR_PAIR(11));
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

            /* Fallback visuel si on a dû recycler des couleurs (bg[8], bg[9]) :
               on différencie les régions 8 et 9 par un attribut (uniquement hors curseur). */
            if (!can_change_color() || COLORS < 16)
            {
                if (!is_cursor && (region == 8 || region == 9))
                {
                    /* petit “marquage” discret */
                    attron(A_BOLD);
                    mvaddch(i, j * 3 + 2, '.');
                    attroff(A_BOLD);
                }
            }
        }
    }
}


void afficherGrilleBlanche(const Grille *g)
{
    for (int i = 0; i < g->taille; i++)
    {
        for (int j = 0; j < g->taille; j++)
        {
            attron(COLOR_PAIR(11));
            mvprintw(i, j * 3, "   ");
            attroff(COLOR_PAIR(11));
        }
    }
}
