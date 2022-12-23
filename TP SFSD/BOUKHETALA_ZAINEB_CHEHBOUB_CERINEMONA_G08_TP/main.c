#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define TailleBLC 100 // taille du bloc
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
    char longEnreg[TL]; /**la longueur de l'enregistrement**/
    char numID[TID]; /**l'identifiant de l'étudiant**/
    char classID[TC]; /**l'année de scolarisation+la salle**/
    char NomPrenom[TNP];
    char genre[1];
    char Teff[1];
    char tabNotes[MatMax];
}   Enreg;
typedef struct RechMan /**un enregistrement manipuler les blocs et positions durant la recherche**/
{
    int i;
    int j;
    bool trouv;
}ijtrouv;

/**la machine abstraite**/

TOVC* ouvrir(char nom[256],char mode)
 {
     TOVC* f=malloc(sizeof(TOVC));
     if(mode=='A') /**ouverture en mode ancien**/
     {
         f->fichier=fopen(nom,"rb+");
         fread(&(f->entete),sizeof(struct Entete),1,f->fichier);
     }
     else{
            if(mode=='N') /**ouverture en mode nouveau**/
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
     rewind(f->fichier); /**positionner le curseur au debut du fichier**/
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
void ecrire_chaine(char name[256], char chaine[256], int longu, char mode, int *s, int *r){
     int i = *s;
     int j = *r;
     TOVC *fichier=ouvrir(name,mode);
     buffer buf;
     liredir(fichier,i,&buf);
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
    ecriredir(fichier,i,&buf);
    *s = i;
    *r = j;
    fermer(fichier);
}
void ecrire_chaine2(char name[256], char chaine[256], int longu, char mode, int *s, int *r){
     int i = *s;
     int j = *r;
     TOVC *fichier=ouvrir(name,mode);
     buffer buf;
     liredir(fichier,i,&buf);
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
        liredir(fichier,i,&buf);
        buf.chaine[0]=chaine[k];
        j=1;
        }
        }
    ecriredir(fichier,i,&buf);
    *s = i;
    *r = j;
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
/**une fonction pour afficher le fichier TOVC**/
void affichage(char nom[256])
  {
   int i=1; int j=0; buffer buf; int stop=0;
   TOVC *f=ouvrir(nom,'A');
   printf("\ni: %d et j : %d",entete_fich(f,1),entete_fich(f,2));
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
/**pour lire une ligne spécifique d'un fichier texte (pour choisir aléatoirement les noms et prénoms)**/
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
    };
    fclose(fH);
}
/**une fonction pour génerer un enregistrement selon les critères de l'énoncé du TP**/
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
                int noteligne = 1 + rand() % 11;
                readLine("notes.txt",noteligne,e.tabNotes);
            }else{
                strcpy(e.tabNotes,"NULLNULL000NULL\0");
                 }
            int l;
            char longueur[TL];
            l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1+1;
            sprintf(longueur,"%d", l);
            strcpy(e.longEnreg,longueur);
    return e;
}
const char * generer_chaine_enreg(Enreg e, char chaine[256]){
    strcpy(chaine,e.longEnreg);
    strcat(chaine,e.numID);
    printf("\nclass id 3 cara: %s",e.classID);
    strcat(chaine,e.classID);
    strcat(chaine,"0"); // celui de Teff
    strcat(chaine,e.NomPrenom);
    strcat(chaine,e.genre);
    strcat(chaine,e.tabNotes);
    printf("\nla chaine qu'on va inserer: %s\n",chaine);
    return chaine;
}
void chargement_initial(char fileName[256],int n){
     int k;
     Enreg e,e1; buffer buff;
     char chaine[256];
     char chaine1[256];
     int i = 1;
     int j = 0;
    // srand(time(NULL));
   /*  e=generer_enreg();
     generer_chaine_enreg(e,chaine);
     printf("\non a cette chaine: %s",chaine);*/
  //   ecrire_chaine(fileName,chaine,atoi(e.longEnreg)+2,'N');
     strcpy(chaine,"357075P80BouhadiMalekMNULLNULL000NULL");
     ecrire_chaine(fileName,chaine,strlen(chaine),'N',&i,&j);
     strcpy(chaine,"361635260ChehboubKamalMA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"357317300MarradjiMonaFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"381478420GuittoneMohamedMA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"351569450MahrezZainebFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"346945550MahrezKarimMA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"346945550MahrezLamiaFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     strcpy(chaine,"346945550MahrezMariaFA12I07M06T00S11");
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     TOVC* fichier=ouvrir(fileName,'A');
     aff_Entete(fichier,1,i);
     aff_Entete(fichier,2,j);
     aff_Entete(fichier,3,n);
     fermer(fichier);
     return;
    }

/**fonction qui compare deux chaines (on en aura besoin pour le tri par nomsprenoms**/
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
/**une fonction pour manipuler les i et j (blocs et positions)**/
/**on l'utilise pour sauter directement à une j par n positions vers la fin du fichier**/
void manfonc(int n,int *i,int *j){
    int f=0;
    int s = *i;
    int r = *j;
    while(f<n){
        r++;
        if(r==TailleBLC){
            r=0; s++;
        }
        f++;
    }
    *i = s;
    *j = r;
}
/**une fonction pour manipuler les i et j (blocs et positions), elle fait l'inverse de manfonc**/
void foncman(int n,int *i,int *j){
    int f=0;
    int s = *i;
    int r = *j;
    while(f<n){
        r--;
        if(r==0){
            r=TailleBLC; s--;
        }
        f++;
    }
    *i = s;
    *j = r;
}
/**la recherche**/
ijtrouv rechercheTOVC(char fileName[256], char cle[2], char name[TNP]){
    ijtrouv e;
    int i=1; int j=0; bool stop=false; bool trouv=false; int s,r;
    TOVC* f=ouvrir(fileName,'A');
    int dernierbloc=entete_fich(f,1);
    int dernierepos=entete_fich(f,2);
    fermer(f);
    char *clenreg=malloc(sizeof(char)*2); //taille de classeID
    char *longueur=malloc(sizeof(char)*2);
    char *Nomprenom=malloc(sizeof(char)*TNP);
    char *Teff=malloc(sizeof(char)*1);
    while((!trouv)&&(!stop)&&(i<=dernierbloc))
        {
            s=i; r=j;
            lire_chaine(fileName,longueur,2,&i,&j);
            manfonc(4,&i,&j);
            lire_chaine(fileName,clenreg,2,&i,&j);
            if(clenreg[0]=='P'){
                clenreg[0]='0';
            }
            if(strcmp(cle,clenreg)==0){
                lire_chaine(fileName,Teff,1,&i,&j);
                    lire_chaine(fileName,Nomprenom,strlen(name),&i,&j);
                    if(strcmp(Nomprenom,name)==0){
                        if(strcmp(Teff,"0")==0){
                            trouv=true;
                        }else{
                            stop=true;
                        }
                    }else{
                        if(strcomp(name,Nomprenom)==1){
                            stop=true;
                        }
                    }
            }else{
                int c1=atoi(clenreg);
                int c2=atoi(cle);
                if(c1>c2){stop=true;}
                if(c1<c2){
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            }
            }
        }
    i=s; j=r;
    manfonc(atoi(longueur)+2,&i,&j);
    e.trouv = trouv;
    e.i = i;
    e.j = j;
    return e;
}
ijtrouv rechercheTOVC20(char fileName[256], char cle[2], char name[TNP]){
    ijtrouv e;
    int i=1; int j=0; bool stop=false; bool trouv=false; int s,r;
    TOVC* f=ouvrir(fileName,'A');
    int dernierbloc=entete_fich(f,1);
    int dernierepos=entete_fich(f,2);
    fermer(f);
    char *clenreg=malloc(sizeof(char)*2); //taille de classeID
    char *longueur=malloc(sizeof(char)*2);
    char *Nomprenom=malloc(sizeof(char)*TNP);
    char *Teff=malloc(sizeof(char)*1);
    while((!trouv)&&(!stop)&&(i<=dernierbloc))
        {
            s=i; r=j;
            lire_chaine(fileName,longueur,2,&i,&j);
            manfonc(4,&i,&j);
            lire_chaine(fileName,clenreg,2,&i,&j);
            if(clenreg[0]=='P'){
                clenreg[0]='0';
            }
            if(strcmp(cle,clenreg)==0){
                lire_chaine(fileName,Teff,1,&i,&j);
                    lire_chaine(fileName,Nomprenom,strlen(name),&i,&j);
                    if(strcmp(Nomprenom,name)==0){
                        if(strcmp(Teff,"0")==0){
                            trouv=true;
                        }else{
                            stop=true;
                        }
                    }else{
                        if(strcomp(name,Nomprenom)==1){
                            stop=true;
                        }
                    }
            }else{
                int c1=atoi(clenreg);
                int c2=atoi(cle);
                if(c1>c2){stop=true;}
                if(c1<c2){
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            }
            }
        }
    i=s; j=r;
    e.trouv = trouv;
    e.i = i;
    e.j = j;
    return e;
}
/**l'insertion TOVC**/
void insertionTOVC(char fileName[256]){
ijtrouv en;
Enreg e; int l;
char chaine[256]; char longueur[2];
TOVC* fh=ouvrir(fileName,'A');
int i=entete_fich(fh,1);
int j=entete_fich(fh,2);
int n=entete_fich(fh,3);
fermer(fh);
char nom[TNP]; int clenreg; char cle[2]; char clef[2];
printf("\nBienvenue dans la procedure d'Insertion:\n");
printf("\nveuillez inserer successivement l'annee de scolarisation (0 pour P) et la salle (de 0 a 9 ) ie: 09 ou 53 : ");
scanf("%d",&clenreg);
printf("veuillez inserer le nom et le prenom de l'eleve : ");
scanf("%s", &nom);
int clee=clenreg;
sprintf(cle,"%d", clenreg);
if(clee<10){
    strcpy(clef,"0");
    strcat(clef,cle);
    strcpy(cle,clef);
}
en=rechercheTOVC(fileName,cle,nom);
    if(en.trouv){
        printf("\netudiant deja insere son i : %d et son j : %d",en.i,en.j);
        return;
    }else{
    printf("\nnon insere\n");
    strcpy(e.classID,cle);
    strcpy(e.Teff,"0");
    strcpy(e.NomPrenom,nom);
    printf("\nle genre: "); scanf("%s",&e.genre);
    printf("\nle tableau des notes: A20I20M20T20S20: "); scanf("%s",&e.tabNotes);
    printf("\nle id:"); scanf("%s",&e.numID);
    l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1+1;
    sprintf(longueur,"%d", l);
    strcpy(e.longEnreg,longueur);
    generer_chaine_enreg(e,chaine);
    if((en.j==j)&&(en.i==i)){
        ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
    }else{
        en=rechercheTOVC20(fileName,cle,nom);
        printf("\n\non a i : %d et j : %d ou devait s'inserer cet etudiant",en.i,en.j);
        int s=en.i; int r=en.j; char chaineTMP[256];
        int s1=s; int r1=r;
        j=j+strlen(chaine);
        if(j>TailleBLC){
             j=j-TailleBLC; i++;
                       }
            while((s!=i+1)){
                 r=r1; s=s1;
                 lire_chaine(fileName,chaineTMP,strlen(chaine),&s1,&r1);
                 ecrire_chaine2(fileName,chaine,strlen(chaine),'A',&s,&r);
                 strcpy(chaine,chaineTMP);
                           }
        }
        TOVC* fichier=ouvrir(fileName,'A');
        aff_Entete(fichier,1,i);
        aff_Entete(fichier,2,j);
        aff_Entete(fichier,3,entete_fich(fichier,3)+1);
        fermer(fichier);
            }
    }
/**la suppression**/
void suppressionTOVC(char filename[256])
{
char nom[TNP]; int clenreg; char cle[2]; char clef[2]; ijtrouv en; char eff[1]; int i,j,n;
printf("\nBienvenue dans la procedure de suppression logique:\n");
printf("\nveuillez inserer successivement l'annee de scolarisation (0 pour P) et la salle (de 0 a 9 ) ie: 09 ou 53 : ");
scanf("%d",&clenreg);
printf("veuillez inserer le nom et le prenom de l'eleve : ");
scanf("%s", &nom);
int clee=clenreg;
sprintf(cle,"%d", clenreg);
if(clee<10){
    strcpy(clef,"0");
    strcat(clef,cle);
    strcpy(cle,clef);
}
en=rechercheTOVC(filename,cle,nom);
    if (en.trouv){
            i=en.i; j=en.j;
            n=17+strlen(nom);
            foncman(n,&i,&j);
            strcpy(eff,"1"); ecrire_chaine(filename,eff,1,'A',&i,&j);
}
    else {printf("\netudiant n existe pas");}
    TOVC* fich = ouvrir(filename,'A');
    aff_Entete(fich,4,entete_fich(fich,4)+1);
    fermer(fich);
    return;
}
void miseajourTOVC(char fileName[256]){
char nom[TNP]; int clenreg; char cle[2]; char clef[2]; ijtrouv en; int i,j,n;
printf("\nBienvenue dans la procedure de la mise a jour:\n");
printf("\nveuillez inserer successivement l'annee de scolarisation (0 pour Preparatoire) et la salle (de 0 a 9 ) ie: 09 ou 53 : ");
scanf("%d",&clenreg);
printf("veuillez inserer le nom et le prenom de l'eleve : ");
scanf("%s", &nom);
int clee=clenreg;
sprintf(cle,"%d", clenreg);
if(clee<10){
    strcpy(clef,"0");
    strcat(clef,cle);
    strcpy(cle,clef);
}
en=rechercheTOVC(fileName,cle,nom);
printf("\non a cherche et i : %d et j: %d",en.i,en.j);
if(en.trouv){
    printf("\nmise a jour des notes");
}else{
    printf("\netudiant non existant et donc impossible de faire la mise a jour\n");
    return;
}
}
int main()
{
buffer buff;
ijtrouv enregi;
char chaine[256]; char chaine1[256];
int f=0; int c;
char nom[TNP];
int clenreg;
Enreg e,e1; char fileName[256]="zed";
chargement_initial(fileName,8);
affichage(fileName);
miseajourTOVC(fileName);
affichage(fileName);
return 0;
}
