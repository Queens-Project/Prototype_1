# QUEENS PROJECT

---

## ✨​​ Le but du jeu ✨
Comme vous l'aurez compris, on veut faire un jeu où :
 - On a une grille de taille N * N (N allant de 5 à 10)
 - Il faut poser N reines sur cette grille
 - Une reine ne peut pas être dans la même ligne, la même colonne, ni être collée à une autre
 - La grille est découpée en N régions, donc pour gagner il faut réussir à placer une reine par région
 - Le joueur bouge un curseur dans la grille avec les flèches, pose ou retire une reine, et valide quand il pense avoir fini

Tout ça doit s'afficher dans le terminal (console) avec couleurs, cases, et interactions clavier


## 👓​ Quels sont les éléments à gérer ? 👓​
1. 🔢 Les données du jeu : la grille, les régions, les positions des reines
2. 📺 L'affichage : dessiner la grille dans la console, avec des couleurs et des cases, etc.
3. ⌨️ Les interactions : touches du clavier pour bouger, poser une reine, valider
4. ✅❌ Les règles : vérifier si les placements sont corrects
5. 💯 Le système de scores : enregistrer les meilleurs temps

--->  On va donc créer un programme structuré autour de ces cinqs grands morceaux.

## 🔢 Représenter les données
Quand le jeu tourne il doit "se souvenir" de plusieurs choses :
-  ce qu'il y a sur chaque case (vide, reine, ou marque)
-  à quelle région appartient chaque case,
-  où est le curseur (pour savoir quelle case surligner à l'affichage)
### 🏁 La grille de jeu
Ici, on va créer une structure pour représenter une la grille
```c
typedef struct {
  int  taille;
  int  regions[MAXN][MAXN];
  EtatCase  place[MAXN][MAXN];
} Grille;
```
Avec EtatCase qui est un petit "type" qu'on définit ainsi :
```c
typedef enum {
  VIDE = 0,
  QUEEN = 1,
  MARK = 2,
} EtatCase;
```
Cela veut dire que pour le programme :
- 0 = vide
- 1 = reine
- 2 = marque

Ainsi le programme peut savoir à tout moment ce qu'il y a sur chaque case.
