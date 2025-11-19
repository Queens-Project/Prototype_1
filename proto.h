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


#endif //PROTO_H

