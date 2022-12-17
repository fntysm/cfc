#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TailleBLC 256
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
int nb;
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
/**affichage du fichier**/
void affichage(char nom[256])
  {

    int i=1;
    int j=0;
    buffer buf;
    int stop=0;
    TOVC *f=ouvrir(nom,'A');
     while(i<=entete_fich(f,1) && !stop)
     {
         j=0;
         liredir(f,i,&buf);
         printf("Tbloc =%d",i);

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
void ecrire_enreg(char name[256], Enreg e, char mode){
    char chaine[256];
    strcpy(chaine,e.longEnreg);
    strcat(chaine,e.numID);
    strcat(chaine,e.classID);
    strcat(chaine,"0"); // celui de Teff
    strcat(chaine,e.NomPrenom);
    strcat(chaine,e.genre);
    strcat(chaine,e.tabNotes);
    printf("\nla chaine qu'on va inserer: %s et sa longu: %d\n",chaine,atoi(e.longEnreg)+2);
    ecrire_chaine(name,chaine,atoi(e.longEnreg)+2,mode);
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
void chargement_initial(char fileName[256],int n){
     int f,k; char chaine[256];
     Enreg e; buffer buff;
     srand(time(NULL));
     for(f=0;f<n;f++){
            e=generer_enreg();
            strcpy(chaine,e.longEnreg);
            strcat(chaine,e.numID);
            strcat(chaine,e.classID);
            strcat(chaine,"0"); // celui de Teff
            strcat(chaine,e.NomPrenom);
            strcat(chaine,e.genre);
            strcat(chaine,e.tabNotes);
            printf("\nla chaine qu'on va inserer: %s et sa longu: %d\n",chaine,atoi(e.longEnreg)+2);
            if(f==0){
               printf("\n\non va inserer l'etudiant: %s",e.NomPrenom);
               ecrire_chaine(fileName,chaine,atoi(e.longEnreg)+2,'N');
            }else{
               printf("\n\non va inserer l'etudiant: %s",e.NomPrenom);
               ecrire_chaine(fileName,chaine,atoi(e.longEnreg)+2,'A');
            }
     }
}

int main()
{
chargement_initial("zed",3);
int s=1;
int r=0;
char chaine[256];
TOVC *f=ouvrir("zed",'A');
int d=entete_fich(f,2);
printf("\nyek d : %d",d);
fermer(f);
lire_chaine("zed",chaine,d,&s,&r);
printf("\n%s",chaine);
lire_chaine("zed",chaine,d,&s,&r);
printf("\n%s",chaine);
lire_chaine("zed",chaine,d,&s,&r);
printf("\n%s",chaine);
/*ecrire_chaine("zed","abcde",5,'N');
ecrire_chaine("zed","fuandk",6,'A');
int s=1;
int r=0;
char chaine[256];
lire_chaine("zed",chaine,11,&s,&r);
printf("\n%s",chaine);*/
    return 0;
}
