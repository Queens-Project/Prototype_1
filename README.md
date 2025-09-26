# QUEENS PROJECT

---

Dans ce projet, je pense qu'on va utiliser des structures pour enregistrer les joueurs, les grilles de jeu et l'état du jeu en général.
C'est une des notions que nous n'avons pas encore vu en cours.

Si jamais vous voulez apprendre avant de le voir en cours, voici un début.

## **1. Déclaration d'une structure**

Regardez-moi cette belle déclaration :

```c
struct enreg
{
    int	numero;
    int	quantite;
    float	prix;
} ;
```

Cette déclaration définit un **modèle de structure**.

Ce modèle s'appelle *enreg* et il précise le nom et le type de chacun des champ contenu dans la structure (*numero*, *quantite* et *prix*).

Une fois qu'on a défini un modèle comme celui-ci, on peut déclarer des variables dont le type sera cette structure.

Par exemple :

```
struct enreg article_1;
```

Une variable comme *article_1* va réserver un emplacement de type *enreg* destiné à contenir deux int (*numero* et *quantite*) et un flottant (*prix*).

De manière semblable, on peut déclarer plusieurs variables en même temps :

```
struct enreg article_1, article_2;
```

Par abus de langage, on dit que article_1 et article_2 sont des structures.

## 2. Utilisation d'une structure

En C, on peut utiliser une structure de deux manières différentes :

    - en travaillant individuellement sur chacun de ses champs

    - en travaillant de manière globale sur l'ensemble de la structure

### 2.1. Utilisation des champs d'une structure

Chaque champ d'une structure peut être manipulé comme n'importe quelle variable du type correspondant.

Exemples d'utilisation des champs d'une structure :

```
article_1.numero = 25; //affecte la valeur 25 au champ 'numero' de la variable article_1

printf("%f", artile_1.prix); //affiche la valeur du champ 'prix' de la structure article_1

article_1.quantite++; //incrémente de 1 la valeur du champ 'quantite' de la structure article_1
```

Comme on le voit dans l'exemple, pour utiliser un champ de la structure article_1, on utilise l'opérateur "." entre le nom de la structure et le nom du champ.

Ainsi : "nom_structure.nom_du_champ" donne accès au champ désiré.
