#include <stdio.h>
#include <stdlib.h>
#define MAX 2048
/**Donner un programme C qui affiche le contenu d'un fichier texte à l'écran.**/
int main()
{
    FILE *file;
    char buffer[MAX];
    char fichier[MAX/2];
    printf("\nentrer le nom du fichier: ");
    scanf("%s",&fichier);
    printf("\nle contenu du fichier: \n\n");
    file = fopen(fichier,"r");
    while(!(feof(file))){
        fgets(buffer, MAX,file);
        printf("%s", buffer);
    }
    printf("\n\n");
    return 0;
}
