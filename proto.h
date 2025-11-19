#ifndef PROTO_H
#define PROTO_H


#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int  taille;
  int  regions[MAXN][MAXN];
  EtatCase  place[MAXN][MAXN];
} Grille;

typedef enum {
  VIDE = 0,
  QUEEN = 1,
  MARK = 2,
} EtatCase;

char	*choose_path(int i);
int	fill_my_struct(char *path, Grille g);

#endif //PROTO_H

