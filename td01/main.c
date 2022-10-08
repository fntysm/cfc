#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2048
/**déclaration de la structure d'enregistrement: exercice 6**/
struct champs{
    char ville[50];
    char date[10];
    float temperature;
}enreg;
/**Donner un programme C qui affiche le contenu d'un fichier texte à l'écran.**/
void lireFichier(char fichier[MAX/2]){
    FILE* file;
    char buffer[MAX];
    printf("\nle contenu du fichier: \n\n");
    file = fopen(fichier,"r");
    if(file==NULL){
        printf("\nINEXISTANT FILE\n");
        return;
    }
    while(!(feof(file))){
        fgets(buffer, MAX,file);
        printf("%s", buffer);
    }
    //une autre solution: c=fgetc(file)!=EOF
    fclose(file);
    printf("\n\n");
    return;
}
/**Donner un programme C qui concatène 2 fichiers textes.**/
FILE* merge2files(){
    FILE* fileH1;
    FILE* fileH2;
    FILE* fileH3;
    char file1[MAX/2];
    char file2[MAX/2];
    char file3[MAX/2];
    char c;
    printf("\nentrer le nom du 1er fichier: ");
    scanf("%s",&file1);
    printf("\nentrer le nom du 2eme fichier: ");
    scanf("%s",&file2);
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
    while(!(feof(fileH1))){
        c=fgetc(fileH1);
        if(c!=EOF){
           fputc(c,fileH3);
        }
    }
    fprintf(fileH3,"\n");
    while(!(feof(fileH2))){
        c=fgetc(fileH2);
        if(c!=EOF){
           fputc(c,fileH3);
        }
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
    char *tokens;
    printf("\nentrer le nom du fichier: ");
    scanf("%s",&fichier);
    file = fopen(fichier,"r");
    if(file==NULL){
        printf("\nINEXISTANT FILE\n");
        return -1;
    }
    while(!(feof(file))){
            fgets(buffer, sizeof(buffer),file);
            tokens = strtok(buffer,limits);
            while(tokens!=NULL){
                if(strcmp(tokens,mot)==0){
                    i++;
                }
            tokens = strtok(NULL, limits);
            }
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
    char ligne[MAX];
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
            if((file1==NULL)||(file2==NULL)){
                printf("\n\nFICHIER INEXISTANT\n\n");
                return 1;
            }
            fclose(fichier1);
            fclose(fichier2);
        }
        break;
        case 1:{
            file1=fopen(fichier1,"r");
            file2=fopen(fichier2,"w");
            if((file1==NULL)||(file2==NULL)){
                printf("\n\nFICHIER INEXISTANT\n\n");
                return 1;
            }
          /**  while(!(feof(fichier1))){
                if(fgets(ligne, sizeof(ligne), fichier1)){
                        return;
            }
            }**/
            fclose(fichier1);
            fclose(fichier2);
        }
        break;
    }
}

int creerunfichierbinaireexo6()
{
    FILE *f;
    char nomf[30];
    printf("Donnez le nom du fichier à construire : ");
    scanf(" %s", nomf);
    f = fopen( nomf, "wb" );
    if ( f == NULL )
       {
       printf("INEXISTANT FILE\n");
       return 0;
       }
    printf("donnez une ville, sa date et sa temperature : ");
    scanf("%s %s %f", enreg.ville,enreg.date,&enreg.temperature);

    while ( enreg.ville[0] != '0' )
    {
       fwrite(&enreg, sizeof(enreg), 1, f);
       printf("donnez une ville, sa date et sa temperature : ");
       scanf(" %s %s %f", enreg.ville, enreg.date,&enreg.temperature);
    }
    return 0;
}
/**manipulation d'un fichier binaire contenant les mesures de températures effectuées sur différentes villes
à différentes dates.**/
void exo6(){
    FILE* fH;
    char fichier[MAX/2];
    printf("\n\nDonner le nom du fichier: ");
    scanf("%s",fichier);
    fH = fopen(fichier,"rb");
    if(fH==NULL){
        printf("\nINEXISTANT FILE\n");
        return;
    }
    fclose(fH);
}
int main()
{
    int i;
    i = nbOcc("esi");
    printf("nombre d'occurences: %d",i);
    return 0;
}
