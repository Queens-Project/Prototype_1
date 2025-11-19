#include "proto.h"


/*This function take an int as parameter for the size of the grid.
It returns the filepath for the good size of map.*/
char	*choose_path(int i)
{
	if (i==5)
		return ("./5x5.txt");
	if (i==6)
		return ("./6x6.txt");
	if (i==7)
		return ("./7x7.txt");
	if (i==8)
		return ("./8x8.txt");
	return (1);
}

/*This function opens a file.txt, choose a map in the 
file and fill a Grille structure with the datas*/
int	fill_my_struct(char *path, Grille g)
{
	FILE	*fichier;
	char	ligne[100];

	fichier = fopen(path, "r+");

	if (fichier == NULL)
	{
		printf("Impossible d'ouvrir le fichier.\n");
		return (1);
	}


}