#include "proto.h"

/*
Sauvegarde multi-slots (un fichier par pseudo) dans le dossier "saves/".
Le fichier contient : pseudo, chemin de la grille utilisée, taille, temps écoulé,
position du curseur, et état des cases (VIDE/QUEEN/MARK).

Format (texte, versionné) :

QUEENS_SAVE_V1
PSEUDO=...
GRID_PATH=...
N=...
ELAPSED=...
CURSOR=l,c

GRID:
..Q..X..
...

Rappel : les régions sont reconstruites via GRID_PATH (lireGrilleFromFichier).
*/

#include <ctype.h>

#ifdef _WIN32
# include <direct.h>
# define MKDIR(path) _mkdir(path)
#else
# include <sys/stat.h>
# include <sys/types.h>
# define MKDIR(path) mkdir((path), 0777)
#endif

#define SAVE_DIR   "saves"
#define SAVE_MAGIC "QUEENS_SAVE_V1"

static void ensure_save_dir_exists(void)
{
    /* MKDIR retourne -1 si le dossier existe déjà : ce n'est pas bloquant */
    MKDIR(SAVE_DIR);
}

static void sanitize_pseudo(const char *in, char *out, size_t cap)
{
    /* Autorise [a-zA-Z0-9_-], remplace espaces par '_', sinon ignore. */
    size_t j = 0;

    if (!in || cap == 0)
        return;

    for (size_t i = 0; in[i] && j + 1 < cap; i++)
    {
        unsigned char c = (unsigned char)in[i];
        if (c == ' ')
            out[j++] = '_';
        else if (isalnum(c) || c == '_' || c == '-')
            out[j++] = (char)c;
        /* sinon on ignore */
    }
    out[j] = '\0';

    if (out[0] == '\0')
    {
        strncpy(out, "player", cap - 1);
        out[cap - 1] = '\0';
    }
}

static void build_save_path(const char *pseudo, char *out, size_t cap)
{
    char slot[64];
    sanitize_pseudo(pseudo, slot, sizeof(slot));
    snprintf(out, cap, SAVE_DIR "/%s.save", slot);
}

static char etat_to_char(EtatCase e)
{
    if (e == QUEEN) return 'Q';
    if (e == MARK)  return 'X';
    return '.';
}

static int char_to_etat(char ch, EtatCase *out)
{
    if (ch == 'Q') { *out = QUEEN; return 1; }
    if (ch == 'X') { *out = MARK;  return 1; }
    if (ch == '.') { *out = VIDE;  return 1; }
    return 0;
}

static int write_index_if_needed(const char *pseudo, const char *save_path)
{
    /*
    Index portable : un slot par ligne "pseudo|path".
    Si pseudo déjà présent, on ne réécrit pas la ligne.
    */
    char index_path[256];
    snprintf(index_path, sizeof(index_path), SAVE_DIR "/index.txt");

    FILE *f = fopen(index_path, "r");
    int exists = 0;

    if (f)
    {
        char line[512];
        while (fgets(line, sizeof(line), f))
        {
            char *sep = strchr(line, '|');
            if (!sep) continue;
            *sep = '\0';
            line[strcspn(line, "\r\n")] = '\0';

            if (strcmp(line, pseudo) == 0)
            {
                exists = 1;
                break;
            }
        }
        fclose(f);
    }

    if (exists)
        return 1;

    f = fopen(index_path, "a");
    if (!f)
        return 0;

    fprintf(f, "%s|%s\n", pseudo, save_path);
    fclose(f);
    return 1;
}

int save_game_slot(const char *pseudo, const char *grid_path, const Grille *g,
                   time_t debut, int cursor_line, int cursor_col)
{
    if (!pseudo || pseudo[0] == '\0' || !grid_path || grid_path[0] == '\0' || !g)
        return 0;

    ensure_save_dir_exists();

    char save_path[1024];
    build_save_path(pseudo, save_path, sizeof(save_path));

    char tmp_path[1024];
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", save_path);

    FILE *f = fopen(tmp_path, "w");
    if (!f)
        return 0;

    int elapsed = (int)(time(NULL) - debut);
    if (elapsed < 0) elapsed = 0;

    fprintf(f, "%s\n", SAVE_MAGIC);
    fprintf(f, "PSEUDO=%s\n", pseudo);
    fprintf(f, "GRID_PATH=%s\n", grid_path);
    fprintf(f, "N=%d\n", g->taille);
    fprintf(f, "ELAPSED=%d\n", elapsed);
    fprintf(f, "CURSOR=%d,%d\n\n", cursor_line, cursor_col);
    fprintf(f, "GRID:\n");

    for (int i = 0; i < g->taille; i++)
    {
        for (int j = 0; j < g->taille; j++)
            fputc(etat_to_char(g->etat[i][j]), f);
        fputc('\n', f);
    }

    fflush(f);
    fclose(f);

    /* Renommage (best effort) */
    remove(save_path);
    if (rename(tmp_path, save_path) != 0)
    {
        remove(tmp_path);
        return 0;
    }

    /* Index non bloquant : si ça échoue, la sauvegarde existe quand même */
    write_index_if_needed(pseudo, save_path);

    return 1;
}

int load_game_slot(const char *pseudo, char *grid_path_out, size_t grid_path_cap,
                   Grille *g_out, int *elapsed_out, int *cursor_line_out, int *cursor_col_out)
{
    if (!pseudo || pseudo[0] == '\0' || !grid_path_out || grid_path_cap == 0 ||
        !g_out || !elapsed_out || !cursor_line_out || !cursor_col_out)
        return 0;

    char save_path[256];
    build_save_path(pseudo, save_path, sizeof(save_path));

    FILE *f = fopen(save_path, "r");
    if (!f)
        return 0;

    char line[512];

    if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
    line[strcspn(line, "\r\n")] = '\0';
    if (strcmp(line, SAVE_MAGIC) != 0) { fclose(f); return 0; }

    char grid_path[256] = {0};
    int n = 0, elapsed = 0, cl = 0, cc = 0;

    /* Lire header KEY=VALUE jusqu'à "GRID:" */
    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0')
            continue;
        if (strcmp(line, "GRID:") == 0)
            break;

        if (strncmp(line, "GRID_PATH=", 10) == 0)
            strncpy(grid_path, line + 10, sizeof(grid_path) - 1);
        else if (strncmp(line, "N=", 2) == 0)
            n = atoi(line + 2);
        else if (strncmp(line, "ELAPSED=", 8) == 0)
            elapsed = atoi(line + 8);
        else if (strncmp(line, "CURSOR=", 7) == 0)
            sscanf(line + 7, "%d,%d", &cl, &cc);
    }

    if (n < 5 || n > N) { fclose(f); return 0; }
    if (grid_path[0] == '\0') { fclose(f); return 0; }

    /* Reconstruire régions via le fichier de grille */
    if (!lireGrilleFromFichier(grid_path, g_out))
    {
        fclose(f);
        return 0;
    }
    if (g_out->taille != n)
    {
        fclose(f);
        return 0;
    }

    /* Lire N lignes d'état */
    for (int i = 0; i < n; i++)
    {
        if (!fgets(line, sizeof(line), f)) { fclose(f); return 0; }
        line[strcspn(line, "\r\n")] = '\0';
        if ((int)strlen(line) < n) { fclose(f); return 0; }

        for (int j = 0; j < n; j++)
        {
            EtatCase e;
            if (!char_to_etat(line[j], &e)) { fclose(f); return 0; }
            g_out->etat[i][j] = e;
        }
    }

    fclose(f);

    strncpy(grid_path_out, grid_path, grid_path_cap - 1);
    grid_path_out[grid_path_cap - 1] = '\0';

    *elapsed_out = (elapsed < 0) ? 0 : elapsed;
    *cursor_line_out = cl;
    *cursor_col_out = cc;

    return 1;
}

/*
Entrée menu : charger une partie depuis le pseudo.
*/
void load_game(void)
{
    char pseudo[25];

    clear();
    mvprintw(0, 0, "Charger une sauvegarde");
    mvprintw(2, 0, "Entrez le pseudo du slot : ");
    refresh();

    echo();
    getnstr(pseudo, 24);
    noecho();

    for (int i = 0; pseudo[i]; i++)
        if (pseudo[i] == ' ')
            pseudo[i] = '_';

    Grille g;
    char grid_path[256];
    int elapsed = 0, line = 0, col = 0;

    if (!load_game_slot(pseudo, grid_path, sizeof(grid_path), &g, &elapsed, &line, &col))
    {
        clear();
        mvprintw(0, 0, "Aucune sauvegarde valide pour ce pseudo.");
        mvprintw(2, 0, "Appuyez sur une touche pour revenir au menu...");
        refresh();
        getch();
        return;
    }

    init_colors_cases();

    time_t debut = time(NULL) - elapsed;

    if (game_loop(&g, &debut, &line, &col, pseudo, grid_path) == 1)
    {
        time_t fin = time(NULL);
        int temps = (int)(fin - debut);
        update_scores(pseudo, temps, g.taille);
        refresh();
        afficher_scores();
    }
}
