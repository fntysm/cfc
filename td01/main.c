#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2048
/**déclaration de la structure d'enregistrement: exercice 6**/
struct champs{
    char ville[11];
    char date[6];
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
        return fileH1;
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
void textfilecrypted(char* key){
    int n; // la taille de la clé
    n = strlen(key);
    FILE* fH;
    char* fichier;
    char* buffer;
    printf("\nveuillez entrer le nom du fichier a crypter: ");
    scanf("%s",&fichier);
    fH = fopen(fichier,"a+");
    if(fH==NULL){
        printf("\nINEXISTANT FILE\n");
        return;
    }
    while(!(feof(fH))){
        fgets(buffer,sizeof(buffer),fH);
        fseek(fH,0,SEEK_CUR);
        fputs(key,fH);
    }
    fclose(fH);
}
int addstructs()
{
    FILE *f;
    char nomf[30];
    int choice;
    printf("cliquer 0 pour construire et 1 pour ajouter a un fichier existant: ");
    scanf("%d",&choice);
    switch(choice){
        case 0:{
            printf("\nDonnez le nom du fichier a construire : ");
            scanf("%s", nomf);
            f = fopen( nomf, "wb" );
        }
            break;
        case 1:{
            printf("\nDonnez le nom du fichier pour lui rajouter un nouveau enregistrement : ");
            scanf("%s", nomf);
            f = fopen( nomf, "ab" );
        }
            break;
    }
    if ( f == NULL )
       {
       printf("INEXISTANT FILE\n");
       return 0;
       }
    printf("\n\ndonnez une ville, sa date et sa temperature : ");
    scanf("%s %s %f", enreg.ville,enreg.date,&enreg.temperature);

    while ( enreg.ville[0] != '0' )
    {
       fwrite(&enreg, sizeof(enreg), 1, f);
       printf("\n\ndonnez une ville, sa date et sa temperature : ");
       scanf("%s %s %f", &enreg.ville, &enreg.date,&enreg.temperature);
    }
    return 0;
}
/**manipulation d'un fichier binaire contenant les mesures de températures effectuées sur différentes villes
à différentes dates.**/
void exo6(){
    char fichier[MAX/2];
    char *buffer;
    int choice;
    FILE* FH;
    printf("\n\nDonner le nom du fichier: ");
    scanf("%s", &fichier);
    FH = fopen(fichier,"rb");
    if(FH==NULL){
       printf("INEXISTANT FILE");
       return;
    }
    printf("\nQue ce que vous voulez faire?\n1- connaitre la temperature minimale, maximale et moyenne pour une ville donnee \n");
    printf("2- modifier la temperature d'une ville donnee a une date donnee\n3- supprimer tous les enregistrements d'une ville");
    printf("\nchoisir l'operation que vous souhaitez appliquer: ");
    scanf("%d",&choice);
    switch(choice){
        case 1:{
            printf("\nvous avez choisi de connaitre la temperature minimale, maximale et moyenne pour une ville donnee");
            char* ville;
            int max=60;
            int min=60;
            int n,prem,moy,k;
            prem = 0;
            n = 0;
            moy = 0;
            k = 0;
            printf("\nDonnez la ville: ");
            scanf("%s",&ville);
            while(!feof(FH)){
                    n=fread(&enreg,sizeof(enreg),1,FH);
                    if(n!=0){
                       if(strcmp(enreg.ville,ville)==0){
                           printf("premier search strcmp");
                         if(!(prem)){
                            max=enreg.temperature;
                            min=enreg.temperature;
                            prem=1;
                                 }
                         else{
                          if(enreg.temperature>max){
                             max=enreg.temperature;
                            }
                          else{
                            if(enreg.temperature<min){
                             min = enreg.temperature;
                              }
                              }
                             }
                   moy+=enreg.temperature;
                   k++;
                     }
                   }
                          }
             moy /= k;
        }break;
        case 2:{
            float newTemp, temp;
            char * ville;
            printf("\nvous avez choisi de modifier la temperature d'une ville donnee a une date donnee");
            struct champs data;
            printf("Donnez la ville: ");
            scanf("%s",&ville);
            printf("La temperature: ");
            scanf("%f",&temp);
            printf("Donnez la temperature: ");
            scanf("%f",&newTemp);
         /*   while(fread(&data,sizeof(enreg),1,FH)==1){
                if((!(strcmp(data.ville,ville)))&&(!(strcmp(data.temperature,temp)))){
                    data.temperature = newTemp;
                    fseek(F,sizeof(enreg),SEEK_CUR);
                    fwrite(data,sizeof(enreg),1,FH);
                }
            }*/
        }break;
        case 3:{
//      fread(buffer,sizeof(enreg),F);
            char * ville;
            printf("\nvous avez choisi de supprimer tous les enregistrements relatifs a une ville donnee");
            FILE* f=fopen("newfich.bin","wb");
            while(fread(&enreg,sizeof(enreg),1,f)){
                if(strcmp(enreg.ville,ville)!=0){
                    fwrite(&enreg,sizeof(enreg),1,f);
                }
            remove(FH);
            rename("newfich.bin",fichier);
            }
        }break;
    }
    // fread(buffer,taille_elt,nbr_elt,F);
    fclose(FH);
    return;
}
int main()
{
    exo6();
    return 0;
}
