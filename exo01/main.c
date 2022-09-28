#include <stdio.h>
#include <stdlib.h>
#define MAX 2048
/**Donner un programme C qui affiche le contenu d'un fichier texte à l'écran.**/
void lireFichier(){
    FILE *file;
    char fichier[MAX/2];
    char buffer[MAX];
    printf("\nentrer le nom du fichier: ");
    scanf("%s",&fichier);
    printf("\nle contenu du fichier: \n\n");
    file = fopen(fichier,"r");
    if(file==NULL){
        printf("\nINEXISTANT FILE\n");
    }
    while(!(feof(file))){
        fgets(buffer, MAX,file);
        printf("%s", buffer);
    }
    printf("\n\n");
}
/**Donner un programme C qui concatène 2 fichiers textes.**/
FILE* merge2files(char file1[MAX/2], char file2[MAX/2]){
    FILE* fileH1;
    FILE* fileH2;
    FILE* fileH3;
    char file3[MAX/2];
    char c;
    fileH1 = fopen(file1,"r");
    fileH2 = fopen(file2,"r");
    if((fileH1==NULL)||(fileH2==NULL)){
        printf("INEXISTANT FILE(S)");
        return;
    }
    strcat(file1,file2);
    strcpy(file3,"new_");
    strcat(file3,file1);
    fileH3 = fopen(file3,"w");
    while(feof(fileH1)!=EOF){
        c=fgetc(fileH1);
        fputc(c,fileH3);
    }
    fprintf(fileH3,"\n");
    while(feof(fileH2)!=EOF){
        c=fgetc(fileH2);
        fprintf(fileH2,"\n");
        fputc(c,fileH3);
    }
    return fileH3;
}
int main()
{
    return 0;
}
