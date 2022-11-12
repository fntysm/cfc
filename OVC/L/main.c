#include <stdio.h>
#include <stdlib.h>
#define b 100
/**déclarations des structures**/
typedef struct un_bloc{
    char Tab[b];
    int next;
}Tbloc;
typedef struct Tbloc buffer;
typedef struct maillon
{
    int val;
    int suivant;
}maillon;
typedef struct Entete{
    int premierBloc;
    int dernierBloc;
    int posLibre; // la première position libre dans le dernier bloc
    int nbrPerdues; // nombre de caractères perdues suite aux suppressions logiques
}entete;

typedef struct LnOVC{
    FILE *fh;
    entete enteTe;
}LnOVC;

/**la machine abstraite**/
void aff_entete(LnOVC *fichier,int i, int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->enteTe.premierBloc=valeur;
        }break;
         case 2:
        {
            fichier->enteTe.dernierBloc=valeur;
        }break;
         case 3:
        {
            fichier->enteTe.posLibre=valeur;
        }break;
         case 4:
        {
            fichier->enteTe.nbrPerdues=valeur;
        }break;
    };
}
int Fentete(LnOVC *fichier, int i)
{

    switch(i)
    {
        case 1:
        {
            return(fichier->enteTe.premierBloc);
        }break;
         case 2:
        {
            return(fichier->enteTe.dernierBloc);
        }break;
         case 3:
        {
            return(fichier->enteTe.posLibre);
        }break;
         case 4:
        {
            return(fichier->enteTe.nbrPerdues);
        }break;
    };
}
void LireDir(LnOVC *fichier, int i , buffer* buf)
{
 // first step : positionnement au debut du bloc numero i
 fseek(fichier->fh,(sizeof(entete)+sizeof(Tbloc)*(i-1)),SEEK_SET);
 // second step : lecture d'un bloc de caractère correspondant a la taille du bloc dans le buffer
 fread(buf,sizeof(Tbloc),1,fichier->fh);
 // third step : repositionnement au début du fichier
 rewind(fichier->fh);

}
void ecriredir(LnOVC *fichier, int i, buffer *buf)
{
     // positionnement au début du bloc numero i
     fseek(fichier->fh,sizeof(entete)+sizeof(Tbloc)*(i-1),SEEK_SET);
     // écriture du contenu du buffer dans le bloc numero i du fichier
     fwrite(buf,sizeof(Tbloc),1,fichier->fh);

}
LnOVC *ouvrir(char nomf[], char mode)
{

    LnOVC *fichier=malloc(sizeof(LnOVC)); // allocation  de la structure
    Tbloc *buf;
    if( (mode=='A') ||  (mode == 'a') )   // mode ancien
    {
        fichier->fh=fopen(nomf,"rb+"); // ouverture du fichier en mode binaire lecture et ecriture
        fread(&(fichier->enteTe),sizeof(entete),1,fichier->fh);  // chargement de l'entete enregistrée en debut de fichier
    }
    else
    {
        if( (mode=='N') ||  (mode == 'n') )  // mode nouveau
        {
             fichier->fh=fopen(nomf,"wb+");  // ouverture du fichier en mode binaire  ecriture
             aff_entete(fichier,1,1);  // mettre tete au premier bloc
             aff_entete(fichier,2,1);  // de meme pour la queue puisque au debut tete est queue sont confondues
             aff_entete(fichier,3,1);  // le premier caractère du ficheir correspond a laposition libre puisqu'il est nouveau
             aff_entete(fichier,4,0);  // aucun caractère n'a encore été supprimé
             fwrite(&(fichier->enteTe),sizeof(entete),1,fichier->fh);// enregistrement de l'entete dans le fichier
             buf.next=-1;   // le suivant du premier bloc a NIL
             sprintf(buf.tab,"%s","");  // initialisation du buffer a chaine vide
             ecriredir(fichier,1,&buf); // ecriture du premier bloc dans le fichier


        }
        else    // format d'ouverture incorrecte
        {
            printf("format d'ouverture impossible");
        }
    }
    return(fichier);  // renvoyer la structure créée
}

void fermer(LnOVC *fichier)  // procedure de fermeture du fichier
{
    rewind(fichier->fh); // repositionnement du curseur en debut de fichier
    fwrite(&(fichier->enteTe),sizeof(entete),1,fichier->fh); // sauvegarde de la dernière version de l'entete de la strucuture L7OVC
    rewind(fichier->fh);// repositionnement du curseur en debut de fichier
    fclose(fichier->fh);// fermeture du fichier

}
int main()
{
    printf("Hello world!\n");
    return 0;
}
