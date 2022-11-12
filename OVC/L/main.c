#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    Tbloc buf;
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
             sprintf(buf.Tab,"%s","");  // initialisation du buffer a chaine vide
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
void alloc_bloc(LnOVC *fichier)
{

        Tbloc *buf=malloc(sizeof(Tbloc));       // allocation du Buffer
        LireDir(fichier,Fentete(fichier,3),buf);   // lecture du bloc correspondant a la queue
        buf->next=Fentete(fichier,1)+1;         // mise a jour dui suvant de la queue au bloc correspondant a la nouvelle queue
        ecriredir(fichier,Fentete(fichier,3),buf);// ecriture du bloc de queue dans le fichier
        aff_entete(fichier,3,Fentete(fichier,1)+1);// mise a jour du numero du bloc correspondant a la nouvelle queue dan sl'entete
        buf->next=-1;                         // mise a jour du suivant a nill
        sprintf(buf->Tab,"%s","");               // vider la chaine du buffer
        ecriredir(fichier,Fentete(fichier,3),buf);// ecriture du buffer dans le bloc representatnt la nouvelle queue
        aff_entete(fichier,1,Fentete(fichier,1)+1);// incrémentation du nombre de bloc alloués
}

// transformer un nombre en chaine de caractère sur longueur de caractère//
void turn_to_string(char chaine[], int n, int longueur)
{

    int k;
    for(k=longueur-1;k>=0;k--)          // on commence par le poids faible du nombre qui va etre mi a chaine[longueur-1]
    {
        chaine[k]=(n%10)+'0';           // extraction chiffre par chiffre  grace au mod par 10 et ajout du code ascii du zero afoin d'obtenir le code ascii correspondant au chiffre obtenu
        n=n/10;                        // on passe au chiffre suivant
    }
    chaine[longueur]='\0';             // fin de la chaine construite
}

//fonction qui permet de construire la chaine correspondant a la forme de l'enregistrememnt//
void concat(char chaine[], int cle, char info[])  //           a inserer dans le ficheir a partir de la cle et de l'info
{

    char ch_f[100];
    turn_to_string(ch_f,cle,5);                   // transformation de la cle en chaine sur 5 positions
    strcat(ch_f,info);                            // concaténation de cle et info
    turn_to_string(chaine,strlen(info),3);        // construction du debut de la chaine finale en commençant par la taille de l'info
    strcat(chaine,"f");                           // mise a jour du champs effacé
    strcat(chaine,ch_f);                          // constructu=ion de la chaine finale avec l'ordre suivant taille efface cle info
}
//----------------------------------- fonction qui permet de récuperer une chaine de longueur
 // n dans le bloc i  lu dans buffer a partir de la position j-------------//
void recuperer_chaine(LnOVC *fichier,int n , int *i, int *j, char chaine[],Tbloc *buf) //
   {

    int k=0;
    sprintf(chaine,"%s","");
    for(k=0;k<n;k++)                    // boucle de n itérations correspondant a la longueur de la chaine
    {                                   // indice de parcours de la chaine resultata et j celui du tableau
        if((*j)<=98)                    // si le kièmem caractère de la chaine correspondant au j ime car du buffer  est toujour dans le bloc i
        {                               // le caractère 99 correspond a la fin de la chaine dans le tableau
            chaine[k]=buf->Tab[(*j)];   // recuperation du caractère dans la position k de la chaine
            (*j)++;                      // deplacement d'une position dans le buffer
        }
        else                             // si le k ièeme caractère a recuperer sort du buffer
        {
            (*i)=buf->next;           // on passe au suivant du bloc i dans la liste
            LireDir(fichier,(*i),buf);   // lecture
            chaine[k]=buf->Tab[0];       // recuperation du premier caractère du tableau de carractère correspondant au ka ième car de la chaine
            (*j)=1;                      //positionnement eu second caractère dans le nouveau buffer
        }
    }
   chaine[k]='\0';                        // fin de la chaine obtenue
}

//----------------------------------- // procedure qui permet d'ecrire n caractère du buffer----------------------------------------------//
void ecrire_chaine(LnOVC *fichier,int n , int *i, int *j, char chaine[],Tbloc *buf)
{
    int *cpt;
    int k=0;
    (*cpt)=0;     // nombre de bloc ajoutés
    for(k=0;k<n;k++)   // k pourn le deplacement dans la chaine
    {
        if((*j)<=98)  //si je suis toujours dans le bloc conserné
        {
            buf->Tab[*j]=chaine[k]; // affectation des caractères de la chaine dans le buffer un a un
            (*j)++;                  // deplacement da,s le buffer
        }
        else                          // si la chaine a inserer depasse le buffer
         {
            ecriredir(fichier,*i,buf);  // ecriture du precedent buffer dans le fichier
            alloc_bloc(fichier);        // alocation d'un nouveau bloc afin de recevoir le reste de la chaine
            buf->Tab[0]=chaine[k];      // ecrtiture du kiem caractère de la chaine dans la position 0
            (*j)=1;                     // passage a la position 1
            (*i)=Fentete(fichier,3);     // deplacement dans les bloc du ficher
            (*cpt)++;                   // incrementation du nombre de bloc alloues
        }
    }
    buf->Tab[*j]='\0';                  // fin de la chaine

}

/**fonctions de base**/
void recherche_LnOVC(LnOVC *fichier,int cle,int trouv,int i, int j){
    Tbloc *buf;
    char *longueur, *efface, *clef;
    i = Fentete(fichier,1);
    j=0;
    trouv = 0;
    LireDir(fichier,i,buf);
    while((!(trouv)&&(i!=Fentete(fichier,2)))||((i==Fentete(fichier,2))&&(j<Fentete(fichier,3)))){
        recuperer_chaine(fichier,3,i,j,longueur,buf);
        recuperer_chaine(fichier,1,i,j,efface,buf);
        recuperer_chaine(fichier,20,i,j,clef,buf);
        if((strcmp(cle,clef)==0)&&(efface="N")){
            trouv = 1;
        }else{
            j = j + atoi(longueur) - 20 ;
            if(j>b){
                j = j - b;
                i = buf->next;
                LireDir(fichier,i,buf);
            }
        }
    }
}
void insertion_LnOVC(LnOVC *fichier,int cle,char *enreg){
    int i,j, trouv;
    int chaine_longueur;
    Tbloc *buf;
    recherche_LnOVC(fichier,cle,trouv,i,j);
    if(trouv==1){
        printf("insertion impossible");
        return;
    }else{
        i=Fentete(fichier,2);
        j=Fentete(fichier,3);
        chaine_longueur = strlen(enreg);
        ecrire_chaine(fichier,3,chaine_longueur,i,j,buf);
        ecrire_chaine(fichier,1,"N",i,j,buf);
        ecrire_chaine(fichier,strlen(enreg),enreg,i,j,buf);
        buf->next = -1;
        ecriredir(fichier,i,buf);
        if(i!=Fentete(fichier,2)){
            aff_entete(fichier,2,i);
        }
        aff_entete(fichier,3,j);
    }
    fermer(fichier);
}
int nb_pos(int a)
{
    int cpt=1;
    while((a/10)!=0)
    {
        a=a/10 ;
        cpt++ ;
    }
   return(cpt);
}
int cle_correct(int cle)
{
  int  correct=0 ;
  if(nb_pos(cle)>5)

  {
      puts("cle n'est pas correct") ;
  }
  return(correct) ;

}

void creation_fichier(LnOVC *fichier,int n) // procedure de création du ficher
{
    int k=0,cle;
    char *info=malloc(sizeof(char)*50);
    aff_entete(fichier,1,1);
    for(k=0;k<n;k++)
    {
        printf("\n\n");
        printf("\tveuillez introduire la cle que vous voulez inserer\n");
        scanf("%d",&cle);
        printf("\t veuillez introduire l'info que vous voulez inserer\n\n");
        scanf("%s",info);
        if(cle_correct(cle))
           {
            insertion_LnOVC(fichier,cle,info);
           }
    }
}
int main()
{   LnOVC *fh;
    fh = ouvrir("fichier1.bin",'n');
    creation_fichier(fh,3);
    fermer(fh);
    return 0;
}
