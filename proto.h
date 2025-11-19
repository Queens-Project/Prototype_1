#ifndef PROTO_H
#define PROTO_H


#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int  taille;
  int  regions[MAXN][MAXN];
  EtatCase  place[MAXN][MAXN];
} Grille;


#endif //PROTO_H
