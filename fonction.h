#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <assert.h>

typedef struct {
    char pseudo[25];
    int temps;
} Score;

// PROTOTYPES
void menu();
void crea_pseudo();
void load_game();
void afficher_scores();
void regles_du_jeu();

int fake_game_loop(char *pseudo);
void update_scores(const char *pseudo, int temps);
int load_scores(Score scores[]);
void add_score(Score scores[], int *count, const char *pseudo, int temps);
void sort_scores(Score scores[], int n);
void save_scores(Score scores[], int count);