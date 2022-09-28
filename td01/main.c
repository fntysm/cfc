#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("here is one\n\n");
        fgets(buffer, MAX,file);
        printf("%s", buffer);
    }
    fclose(file);
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
    fclose(fileH1);
    fclose(fileH2);
    fclose(fileH3);
    return fileH3;
}
/**Donner un programme C qui compte le nombre d'occurrence d'un mot donné dans un fichier texte.**/
int nbOcc(char mot[MAX]){
    FILE *file;
    char limits[] = "*\t\n,;:.=\"()_{}<>!?&' /";
    char fichier[MAX/2];
    char buffer[MAX];
    int i=0;
    char *motgot;
    printf("\nentrer le nom du fichier: ");
    scanf("%s",&fichier);
    file = fopen(fichier,"r");
    if(file==NULL){
        printf("\nINEXISTANT FILE\n");
        return -1;
    }
    while(!(feof(file))){
            fgets(buffer, MAX,file);
            motgot = strtok(buffer,limits);
            while(motgot!=NULL){
                if(strcmp(motgot,mot)==0){
                    i++;
                }
                 motgot = strtok(NULL, limits);
            }
            motgot = strtok(NULL, limits);
        }
    fclose(file);
    return i;
}
/**Ecrire un programme C qui calcul le nombre de lignes dans un fichier texte.**/
int calculLignes(){
    FILE *file;
    char fichier[MAX/2];
    char buffer[MAX];
    int i=0;
    printf("\nentrer le nom du fichier: ");
    scanf("%s",&fichier);
    file = fopen(fichier,"r");
    if(file==NULL){
        printf("\nINEXISTANT FILE\n");
    }
    while(!feof(file)){
        fgets(buffer, MAX,file);
        i++;
    }
    fclose(file);
    printf("le nombre de lignes: %d", i);
    return i;
}
/**crypter un fichier avec une clé secrète (un mot donné)**/
void cryptageFich(){
    FILE *file1, *file2;
    char *fichier1, *fichier2;
    int n; /**n la taille de la clé**/
    int choice;
    printf("\nquelle type de fichier voulez vous crypter: \n0 pour un fichier binaire\n1 pour un fichier texte\n");
    scanf("%d",&choice);
    printf("Nom du fichier : ");
    scanf(" %s", &fichier1);
    printf("Nom du fichier crypted: ");
    scanf(" %s", &fichier2);
    switch(choice){
        case 0:{
            file1=fopen(fichier1,"rb");
            file2=fopen(fichier2,"wb");
            fclose(fichier1);
            fclose(fichier2);
        }
        break;
        case 1:{
            file1=fopen(fichier1,"r");
            file2=fopen(fichier2,"w");
            fclose(fichier1);
            fclose(fichier2);
        }
        break;
    }
}
int main()
{
    return 0;
}
