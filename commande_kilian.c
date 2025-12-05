#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *f = fopen("score.txt", "r");
    if (!f) {
        printf("Erreur d'ouverture du fichier\n");
        return 1;
    }


    // fonction pour recup toute les information numérique des joueurs
    char line[256];

    while (fgets(line, sizeof(line), f)) {

        char *p = line;
        int index, minutes, secondes;
        char nom_joueur[50];

        // --- Récupération du 1er nombre ---
        while (!isdigit(*p)) p++;     // avancer jusqu'à un chiffre
        index = atoi(p);              // convertir

        while (isdigit(*p)) p++;      // sauter le nombre

        // --- Récupération du nom du joueur ---
        while (isspace(*p)) p++;      // sauter les espaces
        while (!isalpha(*p)) p++; // avancer jusqu'a la prochaine lettre
        nom_joueur[0] = '\0';         // initialiser la chaîne

        while (isalpha(*p)){
            strncat(nom_joueur, p, 1);
            p++;}

        // --- Récupération du 2ᵉ nombre ---
        while (!isdigit(*p)) p++;
        minutes = atoi(p);

        while (isdigit(*p)) p++;

        // --- Récupération du 3ᵉ nombre ---
        while (!isdigit(*p)) p++;
        secondes = atoi(p);

        // Affichage de contrôle
        printf("index=%d, minutes=%d, secondes=%d\n", index, minutes, secondes);
        printf("Nom du joueur: %s\n", nom_joueur);
    }

    fclose(f);
    return 0;
}