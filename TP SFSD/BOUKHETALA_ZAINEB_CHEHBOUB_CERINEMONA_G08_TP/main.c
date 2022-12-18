#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define TailleBLC 100
#define Max_enreg 20
#define b 3
#define MatMax 16
#define MAX 50
#define TL 9
#define TC 6
#define TID 6
#define TNP 25

/**déclaration des structures**/
/**le bloc**/
typedef struct Tbloc{
char chaine[TailleBLC];
}Tbloc;
/**le buffer**/
typedef  Tbloc buffer;
/**l'entete**/
typedef struct Entete{
int nb_Tbloc;
int pos_libre;
int nb_insert;
int nb_sup;
}Entete;
/**le fichier TOVC**/
typedef struct TOVC{
FILE *fichier;
Entete entete;
}TOVC;
/**l'enreg**/
typedef struct TEnreg
{
    char longEnreg[TL];
    char numID[TID];
    char classID[TC];
    char NomPrenom[TNP];
    char genre[1];
    char Teff[1];
    char tabNotes[MatMax];
}   Enreg;

/**la machine abstraite**/

TOVC* ouvrir(char nom[256],char mode)
 {
     TOVC* f=malloc(sizeof(TOVC));
     if(mode=='A')
     {
         f->fichier=fopen(nom,"rb+");
         fread(&(f->entete),sizeof(struct Entete),1,f->fichier);
     }
     else{
            if(mode=='N')
            {
         f->fichier=fopen(nom,"wb+");
         f->entete.nb_Tbloc=1;
         f->entete.pos_libre=0;
         f->entete.nb_insert=0;
         f->entete.nb_sup=0;
            }
     }
     return f;
 }


void fermer(TOVC *f)
 {
     rewind(f->fichier); //positionner le curseur au debut du fichier
     fwrite(&(f->entete),sizeof(struct Entete),1,f->fichier);
     fclose(f->fichier);
     free(f);
 }


void liredir(TOVC *f,int num_Tbloc,buffer *buf)
 {
     if(num_Tbloc<=(f->entete).nb_Tbloc)
     {

       fseek(f->fichier,sizeof(Entete)+(num_Tbloc-1)*sizeof(struct Tbloc),SEEK_SET);
       fread(buf,sizeof(struct Tbloc),1,f->fichier);

     }

 }
void ecriredir(TOVC *f,int num_Tbloc,buffer *buf)
 {
      if(num_Tbloc<=(f->entete).nb_Tbloc)
     {
       fseek(f->fichier,sizeof(Entete)+(num_Tbloc-1)*sizeof(struct Tbloc),SEEK_SET);
       fwrite(buf,sizeof(struct Tbloc),1,f->fichier);
     }
 }
 int entete_fich(TOVC *f, int i)
 {
     if(i==1) return f->entete.nb_Tbloc;
     if(i==2) return f->entete.pos_libre;
     if(i==3) return f->entete.nb_insert;
     if(i==4) return f->entete.nb_sup;
 }

void aff_Entete(TOVC *f , int i,int val)
{
      if (i==1) f->entete.nb_Tbloc=val;
      if (i==2) f->entete.pos_libre=val;
      if (i==3) f->entete.nb_insert=val;
      if (i==4) f->entete.nb_sup=val;
}
int alloc_Tbloc(TOVC *f)
{
       aff_Entete(f,1,entete_fich(f,1)+1);
       return entete_fich(f,1);
}
/**fin de machine abstraite**/
/**fonctions qu'on utilise presque partout**/
/**ecrire chaine**/
void ecrire_chaine(char name[256], char chaine[256], int longu, char mode){
     TOVC *fichier=ouvrir(name,mode);
     buffer buf;
     int i = entete_fich(fichier,1);
     int j = entete_fich(fichier,2);
     int *s=i;
     int *r=j;
       for(int k=0;k<longu;k++)
        {
        if(j<TailleBLC)
        {
           buf.chaine[j]=chaine[k];
           j++;
        }
        else{
        ecriredir(fichier,i,&buf);
        alloc_Tbloc(fichier);
        i++;
        buf.chaine[0]=chaine[k];
        j=1;
        }
        }
    aff_Entete(fichier,1,i);
    aff_Entete(fichier,2,j);
    ecriredir(fichier,i,&buf);
    aff_Entete(fichier,3,entete_fich(fichier,3)+1);
    printf("\nj: %d",entete_fich(fichier,2));
    fermer(fichier);
    lire_chaine(name,chaine,longu,&s,&r);
}

/**lire chaine**/
void lire_chaine(char name[256], char chaine[256], int longu, int *s, int *r)
{
    TOVC *f=ouvrir(name,'A');
    int i = *s;
    int j = *r;
    int m;
    buffer buf;
    liredir(f,i,&buf);
    for (int k = 0; k < longu; k++)
    {
        if ((j < TailleBLC) || (i == entete_fich(f, 1)))
        {
            if ((i == entete_fich(f, 1)) && (j == entete_fich(f, 2)))
            {
                break;
            }
            chaine[k] = buf.chaine[j];
            j++;
        }
        else
        {
            i++;
            liredir(f, i, &buf);
            chaine[k] = buf.chaine[0];
            j = 1;
        }
        m = k;
    }
    chaine[m + 1] = '\0';
    if (j == TailleBLC)
    {
        i++;
        liredir(f, i, &buf);
        j = 0;
    }
    *s = i;
    *r = j;
    fermer(f);
}
void affichage(char nom[256])
  {

      int i=1;
      int j=0;
      buffer buf;
       int stop=0;
       TOVC *f=ouvrir(nom,'A');
       printf("i: %d et j : %d",entete_fich(f,1),entete_fich(f,2));
     while(i<=entete_fich(f,1))
     {
         j=0;
         liredir(f,i,&buf);
         printf("\nbloc =%d : ",i);
         while((j<TailleBLC))
         {

             printf("%c",buf.chaine[j]);
             j++;

         }
         printf("\n");
         i++;
     }
     fermer(f);
  }
/**fonctions relatives à l'énoncé du TP**/
// read a specific line from a file
void readLine(char fileName[MAX/2], int lineNum, char buffer[MAX]){
    FILE *fH;
    int lines = 0;
    fH = fopen(fileName, "r");
    if (fH == NULL){
            printf("\nINEXISTANT FILE\n");
            return;
    };
    while(!(feof(fH)) && (lines<=lineNum)){
        lines++;
        fscanf(fH, "%s", buffer);
        //fgets(buffer, MAX, fH);
    };
    fclose(fH);
}
Enreg generer_enreg(){
    Enreg e;
    int n1,n2,studentID,salle,annee,longu,classeID,k;
    int t=0;
    char classID[3]; char anneeString[3]; char salleString[2]; char genre; char noms[MAX];
    char prenoms[MAX]; char studentIDchar[4]; int tabnotes[MatMax]; int note;
    char tabnotesChar[MatMax];
    char noteChar[MatMax]; int a=0; const char s[2] = ";";char *token;
            studentID = 1000 + rand() % 10000;
            sprintf(studentIDchar,"%d", studentID);
            strcpy(e.numID,studentIDchar);
            salle = rand() % 10;
            annee = 1 + rand() % 6;
            sprintf(salleString,"%d", salle);
            sprintf(anneeString,"%d", annee);
            strcat(anneeString,salleString);
            // on a effectuer la clé à sa place
            classeID = atoi(anneeString);
            if(classeID / 60 == 1){
                anneeString[0] = 'P';
            }
            strcpy(e.classID,anneeString);
            n1 = rand() % 9;
            n2 = rand() % 11;
            readLine("noms.txt",n1,noms);
            readLine("prenoms.txt",n2,prenoms);
            strcpy(e.NomPrenom,noms);
            token = strtok(prenoms, s);
            while( (token != NULL) && t<2 ) {
            if(t==0){strcat(e.NomPrenom,prenoms);}
            if(t==1){strcpy(&e.genre,token);}
            token = strtok(NULL, s);
            t++;
            }
            if(e.classID[0]!='P'){
                readLine("notes.txt",2,e.tabNotes);
            }else{
                strcpy(e.tabNotes,"NULLNULL\0");
                 }
            int l;
            char longueur[TL];
            l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1+1;
            sprintf(longueur,"%d", l);
            strcpy(e.longEnreg,longueur);
    return e;
}
void generer_chaine_enreg(Enreg e, char chaine[256]){
    strcpy(chaine,e.longEnreg);
    strcat(chaine,e.numID);
    printf("\nclass id 3 cara: %s",e.classID);
    strcat(chaine,e.classID);
    strcat(chaine,"0"); // celui de Teff
    strcat(chaine,e.NomPrenom);
    strcat(chaine,e.genre);
    strcat(chaine,e.tabNotes);
    printf("\nla chaine qu'on va inserer: %s\n",chaine);
}
void chargement_initial(char fileName[256],int n){
     int k;
     Enreg e,e1; buffer buff;
     char chaine[256];
     char chaine1[256];
     srand(time(NULL));
   /*  e=generer_enreg();
     generer_chaine_enreg(e,chaine);
     printf("\non a cette chaine: %s",chaine);*/
     strcpy(chaine,"381478420GuittoneMohamedMA12I07M06T00S11");
  //   ecrire_chaine(fileName,chaine,atoi(e.longEnreg)+2,'N');
     ecrire_chaine(fileName,chaine,strlen(chaine),'N');
     strcpy(chaine,"351569450MahrezZainebFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A');
     strcpy(chaine,"361635260ChehboubKamalMA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A');
     strcpy(chaine,"346945550MahrezKarimMA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A');
     strcpy(chaine,"287075P80BouhadiMalekMNULLNULL");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A');
     strcpy(chaine,"357317300MarradjiMonaFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A');
    }
bool recherche_dicho_fichier(char fileName[256],char classID[2], char name[TNP])
{
    int binf=1; int i;
    TOVC* f=ouvrir(fileName,'A');
    int bsup=entete_fich(f,1);//dernier bloc
    fermer(f);
    printf("\non a bsup: %d",bsup);
    buffer Buffer;
    bool trouv = false;
    bool stop = false;
    int j=0;
    int longueurname; // longueur du name a rechercher
    char *ch1=malloc(sizeof(char)*2); //taille de classeID
    char *longueur=malloc(sizeof(char)*2);
    char *Nomprenom=malloc(sizeof(char)*TNP);
    while (!(trouv) || !(stop) || bsup >= binf)
    {
        i=(binf+bsup)/ 2; //moitier des blocs
        lire_chaine(fileName,longueur,2,&i,&j); //longueur d'un enreg sur 2 carac
        printf("\nlongu qu'on a trouve: %s et i: %d et j: %d",longueur,i,j);
        j=j+4;
        lire_chaine(f,ch1,2,&i,&j); //classeID
        printf("\ncle qu'on a trouve: %s et i: %d et j: %d",ch1,i,j);
        int comp=atoi(ch1);// numclasse de l'enreg
        if (comp==atoi(classID)) //recherche dans les nomprenom
        {
            j++;
            longueurname=strlen(name);
            lire_chaine(f,Nomprenom,longueurname,&i,&j);
            printf("\nnomprenom qu'on a trouve: %s et i: %d et j: %d",Nomprenom,i,j);
            if (strcmp(name,Nomprenom)== 0) {trouv=true ; printf("\nexiste\n"); return trouv;}
            else{
            if (strcomp(name,Nomprenom)==1) { stop = true; printf("\nexiste pas\n"); } // etudiant n'existe pas
            if (strcomp(name,Nomprenom)==-1) // on passe a l'enreg suivant
            {
               j=j+atoi(longueur) ;  // j 1er position de l'enreg suivant

               if (j>TailleBLC) { j=0; i++;} // on a fini le bloc

            }
            }
        }
        else
        {
            if (atoi(classID)<comp) //on monte vers le haut
            {
                bsup=i;
            }
            else
            {
                binf=i;
            }
        }

    }

}
//fonction qui compare deux chaines
int strcomp(char * chaine1,char *chaine2)
{
 int j=0;
 while(chaine1[j] != '\0' && chaine2[j] != '\0')
 {
    if (chaine1[j] != chaine2[j])
    {
      if (chaine1[j] < chaine2[j]) return (1); // exemple B < A
      if (chaine1[j] > chaine2[j]) return (-1);
    }
    j++;
 }
}

int main()
{
buffer buff;
char chaine[256]; char chaine1[256];
srand(time(NULL));
int f=0;
Enreg e,e1; char fileName[256]="zed";
//chargement_initial(fileName,2);
affichage(fileName);
bool trouv;
trouv=recherche_dicho_fichier(fileName,"42","GuittoneMohamed");
printf("\ntrouv: %d",trouv);
return 0;
}
