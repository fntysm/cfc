/**la réponse aux questions théoriques est à la fin du fichier**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <windows.h>
#include <stdarg.h>
#define TailleBLC 97 // taille du bloc
#define MAXINDEX 150
#define b 20
#define MatMax 25
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
typedef struct RechMan /**un enregistrement pour manipuler les blocs et positions durant la recherche**/
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
/**une fonction pour afficher le fichier TOVC (bloc par bloc)**/
void affichage(char nom[256])
  {
   int i=1; int j=0; buffer buf; int stop=0;
   TOVC *f=ouvrir(nom,'A');
   printf("\ni: %d et j : %d",entete_fich(f,1),entete_fich(f,2));
     while((i<=entete_fich(f,1)))
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

/**une fonction pour génerer un enregistrement selon les critéres de l'énoncé du TP**/
Enreg generer_enreg(){
    Enreg e;
    int n1,n2,studentID,salle,annee,longu,classeID,k;
    int t=0;
    char classID[3]; char anneeString[3]; char salleString[2]; char genre; char noms[MAX];
    char prenoms[MAX]; char studentIDchar[4]; int tabnotes[MatMax]; int note;
    char tabnotesChar[MatMax];
    char noteChar[MatMax]; int a=0; const char s[2] = ",";char *token;
            studentID = 1000 + rand() % 8800;
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
            n1 = 1 + rand() % 347;
            n2 = 1 + rand() % 92;
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
                int noteligne = 1 + rand() % 84;
                readLine("notes.txt",noteligne,e.tabNotes);
            }else{
                strcpy(e.tabNotes,"A00I00M00T00F00N00H00S00");
                 }
            int l;
            char longueur[TL];
            l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1+1;
            sprintf(longueur,"%d", l);
            strcpy(e.longEnreg,longueur);
    return e;
}
void generer_chaine_enreg(Enreg e, char * chaine[256]){
    strcpy(chaine,e.longEnreg);
    strcat(chaine,e.numID);
    strcat(chaine,e.classID);
    strcat(chaine,"0"); // celui de Teff
    strcat(chaine,e.NomPrenom);
    strcat(chaine,e.genre);
    strcat(chaine,e.tabNotes);
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
    TOVC* f=ouvrir(fileName,'A'); int c1,c2;
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
                        }else{
                       i=s; j=r;
                       manfonc(atoi(longueur)+2,&i,&j);
                        }
                    }
            }else{
                c1=atoi(clenreg);
                c2=atoi(cle);
                if(c1>c2){stop=true;}
                if(c1<c2){
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            }

        }}
    i=s; j=r;
    manfonc(atoi(longueur)+2,&i,&j);
    e.trouv = trouv;
    e.i = i;
    e.j = j;
    return e;
}
/**la recherche 2 est un peu différente de la première, par exemple, elle retourne la première position de l'enregistrement**/
/**si il existe, sinon, elle retourne la position ou il devait s'insérer**/
ijtrouv rechercheTOVC20(char fileName[256], char cle[2], char name[TNP]){
    ijtrouv e;
    int i=1; int j=0; bool stop=false; bool trouv=false; int s,r;
    TOVC* f=ouvrir(fileName,'A'); int c1,c2;
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
                        }else{
                       i=s; j=r;
                       manfonc(atoi(longueur)+2,&i,&j);
                        }
                    }
            }else{
                c1=atoi(clenreg);
                c2=atoi(cle);
                if(c1>c2){stop=true;}
                if(c1<c2){
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            }

        }}
    i=s; j=r;
    e.trouv = trouv;
    e.i = i;
    e.j = j;
    return e;
}
/**une fonctions qui calcule le nombre d'occurences d'un élément d'un tableau**/
int nbrocc(int n,int list[n], int v){
    int cpt;
    cpt=0;
    int k=0;
    while (k<n){
    if(list[k] == v){cpt++;};
    k++;
    };
return cpt;
};
void chargement_initial(char fileName[256],int n){
     Enreg e; ijtrouv en; int f,cpt,cptsec;
     int i; int j; char chaine[256]; char cle[2]; char name[TNP];
     Enreg enregistrements[n]; Enreg enregTries[n]; int s; Enreg inter;
     int lesClefs[n]; int student; int etudiant,element,a;
     srand(time(NULL));
     student=0;
     while(student<n){
        e = generer_enreg();
        strcpy(cle,e.classID);
        if(cle[0]=='P'){cle[0]='0';};
        lesClefs[student]=atoi(cle);
        enregistrements[student]=e;
        student++;
     }
         for (etudiant = 0; etudiant < n; ++etudiant)
        {
            for (element = etudiant + 1; element < n; ++element)
            {
                if (lesClefs[etudiant] > lesClefs[element])
                {
                    a =  lesClefs[etudiant];
                    lesClefs[etudiant] = lesClefs[element];
                    lesClefs[element] = a;
                    inter = enregistrements[etudiant];
                    enregistrements[etudiant] = enregistrements[element];
                    enregistrements[element] = inter;
                }
            }

        }
        f=0;
        while(f<n){
            cpt = nbrocc(n,lesClefs,lesClefs[f]);
            if(cpt>1){
                for(cptsec=0;cptsec<cpt;cptsec++){
                    enregTries[cptsec]=enregistrements[f+cptsec];
                }
                for (i = 0; i < cpt; ++i)
                {
                for (j = i + 1; j < cpt; ++j)
                {

                if (strcomp(enregTries[i].NomPrenom,enregTries[j].NomPrenom)==-1)
                {

                    inter =  enregTries[i];
                    enregTries[i] = enregTries[j];
                    enregTries[j] = inter;

                }
               }
               }
               for(cptsec=0;cptsec<cpt;cptsec++){
                    enregistrements[f+cptsec]=enregTries[cptsec];
                }
            }
            f=f+cpt;
        }
     i=1; j=0;
     generer_chaine_enreg(enregistrements[0],&chaine);
     ecrire_chaine(fileName,chaine,strlen(chaine),'N',&i,&j);
     for(f=1;f<n;f++){
     generer_chaine_enreg(enregistrements[f],&chaine);
     ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
     }
     TOVC* fichier=ouvrir(fileName,'A');
     aff_Entete(fichier,1,i);
     aff_Entete(fichier,2,j);
     aff_Entete(fichier,3,n);
     fermer(fichier);
     return;
    }


/**l'insertion TOVC personnalisé pour cette opération quand elle dépend de les données entrées par l'utilisateur de la console**/
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
        printf("\netudiant deja insere");
        return;
    }else{
    strcpy(e.classID,cle);
    if(clee<10){
    e.classID[0]='P';
      }
    strcpy(e.Teff,"0");
    strcpy(e.NomPrenom,nom);
    int identifiant;
    identifiant = 1000 + rand()% 8000;
    sprintf(e.numID,"%d",identifiant);
    printf("\nle genre: "); scanf("%s",&e.genre);
    if(e.classID[0]!='P'){
                int noteligne = 1 + rand() % 84;
                readLine("notes.txt",noteligne,e.tabNotes);
            }else{
                strcpy(e.tabNotes,"A00I00M00T00F00N00H00S00");
                 }
    l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1+1;
    sprintf(longueur,"%d", l);
    strcpy(e.longEnreg,longueur);
    generer_chaine_enreg(e,chaine);
    if((en.j==j)&&(en.i==i)){
        ecrire_chaine(fileName,chaine,strlen(chaine),'A',&i,&j);
    }else{
        en=rechercheTOVC20(fileName,cle,nom);
        int s=en.i; int r=en.j; char chaineTMP[256];
        int s1=s; int r1=r;
        j=j+strlen(chaine);
        if(j>TailleBLC){
             j=j-TailleBLC; i++;
                       }
            while((s!=i+1)&&(r!=j+1)){
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
            n=26+strlen(nom);
            foncman(n,&i,&j);
            strcpy(eff,"1"); ecrire_chaine(filename,eff,1,'A',&i,&j);
}
    else {printf("\netudiant n existe pas");}
    TOVC* fich = ouvrir(filename,'A');
    aff_Entete(fich,4,entete_fich(fich,4)+1);
    fermer(fich);
    return;
}


/**mise à jour des notes**/
void miseajourTOVCnotes(char fileName[256]){
char nom[TNP]; int clenreg; char cle[2]; char clef[2]; ijtrouv en; int n; bool stop=false; char note[2];
printf("\nBienvenue dans la procedure de la mise a jour:\n"); char choice;
printf("\nveuillez inserer successivement l'annee de scolarisation (0 pour Preparatoire) et la salle (de 0 a 9 ) parexemple: 09 ou 53 : ");
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
int s=en.i; int r=en.j;
if(en.trouv){
    while(!stop){
        char c; printf("\nlaquelle de ces matieres voulez vous changer, entrer le caractere correspondant A20I20M20T20F20N20H20S20: ");
        scanf(" %c",&c); int j=r; int i=s;
        printf("\nla nouvelle note a inserer: "); scanf(" %s",&note);
        switch(c){
            case 'A':{printf("\nvous allez changer la note de l'examen de la langue arabe ");
                      foncman(23,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'I':{printf("\nvous allez changer la note de l'examen de l'education islamique ");
                      foncman(20,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'M':{printf("\nvous allez changer la note de l'examen de les mathematiques ");
                      foncman(17,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'T':{printf("\nvous allez changer la note de l'examen de la langue berbere ");
                      foncman(14,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);}break;
            case 'F':{printf("\nvous allez changer la note de l'examen de la langue francaise");
                      foncman(11,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'N':{printf("\nvous allez changer la note de l'examen de l'anglais");
                      foncman(8,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'H':{printf("\nvous allez changer la note de l'examen de l'histoire/geo");
                      foncman(5,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            case 'S':{printf("\nvous allez changer la note de l'examen du sport");
                      foncman(2,&i,&j);
                      ecrire_chaine2(fileName,note,2,'A',&i,&j);
            }break;
            default: {printf("\nmatiere inexistante\n");}break;
    }
    printf("\nest ce que vous voulez changer une autre note? Y/N : "); scanf(" %c",&choice); if(choice=='N')stop=true;
}}else{
    printf("\netudiant non existant et donc impossible de faire la mise a jour\n");
    return;
}
}
/**les fonctions de la partie 02**/


typedef struct EnteteTOF
{
    int nbBloc;
    int nbEnrg;
} EnteteTOF;

typedef struct TOF
{
    FILE *fichier;
    EnteteTOF entete;
} TOF;

struct e {
        char nom[20];
        int year;
        float moy;

    } ;


typedef struct TBlocTOF
{
  struct e tab[30] ;
 int nb; //nb d'enreg dans le bloc
 float min;
 float max;

}TBlocTOF;

typedef struct adr
{
    int numbloc;
    int depl;

}adr;

typedef struct Tcouple  // pour l' index  dense
{
    float moy;
   int numbloc;
   int depl;


};

struct Tcouple Index[MAXINDEX];



/************************** machine abstraite TOF****************************/


int enteteTOF(TOF *f, int i)
{
    switch (i)
    {
    case 1:
        return f->entete.nbBloc; // Nombre des blocs
    case 2:
        return f->entete.nbEnrg; // Nombre des enregistrements
    default:
        break;
    }
}

void affEnteteTOF(TOF *f, int i, int val)
{
    switch (i)
    {
    case 1:
        f->entete.nbBloc = val; // Nombre de blocs
    case 2:
        f->entete.nbEnrg = val; // Nombre des enregistrements
    default:
        break;
    }
}

void lireDirTOF(TOF *f, int i, TBlocTOF *buff)
{
    fseek(f->fichier, (sizeof(EnteteTOF) + sizeof(TBlocTOF) * (i - 1)), SEEK_SET);
    fread(buff, sizeof(TBlocTOF), 1, f->fichier);
    rewind(f->fichier);
}

void ecrireDirTOF(TOF *f, int i, TBlocTOF *buff)
{
    fseek(f->fichier, sizeof(EnteteTOF) + sizeof(TBlocTOF) * (i - 1), SEEK_SET);
    fwrite(buff, sizeof(TBlocTOF), 1, f->fichier);
}

TOF *ouvrirTOF(char nomFichier[] , char mode)
{
    TOF *f = malloc(sizeof(TOF));
    TBlocTOF buff = {.tab = 0, .nb = 0};
    if ((mode == 'A') || (mode == 'a'))
    {
        f->fichier = fopen(nomFichier, "rb+");
        fread(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
    }
    else if ((mode == 'N') || (mode == 'n'))
    {
        f->fichier = fopen(nomFichier, "wb+");
        affEnteteTOF(f, 1, 0);
        affEnteteTOF(f, 2, 0);
        fwrite(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
        ecrireDirTOF(f, 1, &buff);
    }
    else
    {
        printf("Mode d'ouverture non valide");
    }
    return (f);
}

void fermerTOF(TOF *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(EnteteTOF), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}

void allocBlocTOF(TOF *f)
{
    affEnteteTOF(f, 1, enteteTOF(f, 1) + 1);
}


/**fonctions principales de la partie 2**/


/**elle retourne la moyenne maximum dans un buffer**/
float returnmax (TBlocTOF buf){
    float max; int m=1; int t;
    max=buf.tab[0].moy;
for (t=0;t<20;t++){

    if (max < buf.tab[m].moy){max =buf.tab[m].moy; m++;}
    else {m++;}

}
     return max;
}

/**elle retourne la moyenne minimum dans un buffer**/
float returnmin (TBlocTOF buf){
       float min; float h; int t,m=0;
       min=buf.tab[0].moy;
for (t=0;t<20;t++){

    if (min >= buf.tab[m].moy){min =buf.tab[m].moy; m++; }
    else {m++;}

                  }
h=min;

     return h;
}

/**fonction qui ordonne un tableau ordre croissant**/
void ordonnerTableau(struct Tcouple tableau[MAXINDEX], int tailleTableau)
{
int i,t,k=0;
struct Tcouple tmp;
for(t =0 ; t < tailleTableau; t++)
{
for(i=t+1; i < tailleTableau ; i++)
{
if(tableau[t].moy > tableau[i].moy)
{
  tmp=tableau[t];

  tableau[t]=tableau[i];
  tableau[i]=tmp;

}
}
}
}


/**chargement de l'index**/
void chargementIndex (char fileName[256],int *nbe )
{
  TBlocTOF buf;
  struct Tcouple Index[MAXINDEX];
  struct e {
  char nom[20];
  int year;
  float moy;
    } ; //structure enreg fichier

    FILE *f = NULL; bool trouv=false;
    int k; int i=0; int j=0; float val=0;
    char name[20]; int y; float moy; int m;

    char nomf[30];
    printf( "\nConstruction d un fichier TOF \n\n");
    printf( "Donnez le nom du fichier a construire : ");

    scanf( " %s", nomf );

    f = ouvrirTOF(nomf,'N');
    if ( f == NULL ) {
        printf( "erreur lors de l ouverture du fichier %s en mode wb\n", nomf );
        return 0;
    }


    /** les notes des eleves **/

     float tab2[MAXINDEX] = {17,16.32,19,15.02,14.06,15,18,17,13.05,12.07,16.08,13.4,14.7,10.03,11.5,11.2,19.05,19.56,17.8,11.68,18.23,15.42,16.1,12.63,13.81,12.54,16.87,17.36,14,18,
     10.24,13.89,10.05,11.56,11.48,19.22,15.41,10.36,11.49,11.75,13.47,14.25,19.45,17.12,17.26,10.17,10.69,10.54,10,11,12,13,14,16,14,10.2,11.4,13.04,17.05,15.47,12.3,18.13,17,16.32,19,15.02,14.06,15,18,17,13.05,12.07,16.08,13.4,14.7,10.03,11.5,11.2,19.05,19.56,17.8,11.68,18.23,15.42,16.1,12.63,13.81,12.54,16.87,17.36,14,18,
     10.24,13.89,10.05,11.56,11.48,19.22,15.41,10.36,11.49,11.75,13.47,14.25,19.45,17.12,17.26,10.17,10.69,10.54,10,11,12,13,14,16,14,10.2,11.4,13.04,17.05,15.47,12.3,18.13,17,16.32,19,15.02,14.06,15,18,17,13.05,12.07,16.08,13.4,14.7,10.03,11.5,11.2,19.05,19.56,17.8,11.68,18.23,15.42,16.1,12.63,13.81,12.54,16.87,17.36,14,18,
     10.24,13.89,10.05,11.56,11.48,19.22,15.41,10.36,11.49,11.75,13.47,14.25,19.45,17.12,17.26,10.17,10.69,10.54,10,11,12,13,14,16,14,10.2,11.4,13.04,17.05,15.47,12.3,18.13};

     printf(" \n -------------------Affichage du fichier archive------------------\n  ");
     printf("\n numEtudiant |\tnomprenom  |\tannee de scolarisation  | moyenne \n ");
     srand(time(NULL));

     for (k=0;k<nbe;k++) //nombre d'enreg
     {

         if (j<b)
        {

        readLine("nomsArchive.txt",k,buf.tab[j].nom);
        buf.tab[j].year = ((rand () % 5) +1);
        buf.tab[j].moy= tab2[k];


         printf("| %d |\tnom %s  |\tannee %d | moy %.2f \n ",k,buf.tab[j].nom,buf.tab[j].year,buf.tab[j].moy);
         j++; buf.nb++;

         }
         else
         {
             /** a la fin on parcours le buffer pour la moyenne max et min**/
             buf.max=returnmax(buf);
             buf.min=returnmin(buf);
             printf("\nvaleur max et min du Buffer %.2f %.2f \n ",buf.max,buf.min);
             ecrireDirTOF(f,i,&buf); i++; j=1;
             srand(time(NULL));
             readLine("nomsArchive.txt",k,buf.tab[j].nom);
             buf.tab[0].year = (rand () % 5) +1;
             buf.tab[0].moy= tab2[k];
             printf("| %d |\tnom %s  |\tyear %d |\tmoy %.2f \n ",k,buf.tab[0].nom,buf.tab[0].year,buf.tab[0].moy);
         }

     }

    affEnteteTOF(f,1,i+1); affEnteteTOF(f,2,k);
    printf("\nil y a %d blocs et %d enregistrements \n",enteteTOF(f,1), enteteTOF(f,2));


   k=0; i=0; j=0;
   printf("\n Affichage de l'index");
  for (k=0;k<nbe;k++)
  {

     if (j<b)
     {
         Index[k].moy=tab2[k];  // on parcout le tab qui contient les moyennes
         Index[k].numbloc=i;
         Index[k].depl=j;
         j++;
     }
     else
     {
      i++;
      j=0;
      Index[k].moy=tab2[k];
      Index[k].numbloc=i;
      Index[k].depl=j;

      j++;
     }

  }
  // on va ordonner les valeurs de l'index
  ordonnerTableau(&Index,nbe);
  //affichage de l'index ordonné
  printf("\n");
  printf("\nAffichage de l'index en ordre" );
  for (k=0;k<nbe;k++)
  {
     printf("\n %d | moyenne %.2f <i,j> = <%d,%d >\n",k,Index[k].moy,Index[k].numbloc,Index[k].depl);

  }

       Recherche_Index(&Index,nbe);


}
 void Recherche_Index (struct Tcouple tab[MAXINDEX], int tailleTableau )
 {
    int m=0; int k=0; int cpt=0;float val=0; char etudiant[50];
    printf(" \n Procedure Recherche ");
    printf("\n donnez la moyennne  "); scanf("%f",&val);
    printf(" \n les etudiants qui ont une moyenne superieur/egale a %2.f ",val);

   for (k=0;k<tailleTableau;k++){
    if (val<tab[k].moy || val==tab[k].moy){ cpt++; readLine("nomsArchive.txt",k,etudiant);
    printf("\n %s",etudiant); printf(" <%d,%d> ",tab[k].numbloc,tab[k].depl);

    } }
    if (cpt==0){printf(" \n la moyenne %.2f n'existe pas",val);}

 }




 /*Insertion_eleve(char *filename, TOF *f,struct e student)
 {
     int k=0; int i=0; int j=0; TBlocTOF buf; struct adr a;
     f=ouvrirTOF(filename,'A');
     while (i<=enteteTOF(f,1))
     {
         lireDirTOF(f,i,&buf);

     if (j<=b)
     {
            if (strcomp(buf.tab[j].nom,student.nom)==1){ a.numbloc=i; a.depl=j;}
            else {j++;}

     }
     else {j=0; i++;}

     }

     //on a l'adresse ou doit se trouver l'eleve dans l'enreg a
     bool stop=false; struct e tmp;

     while ((i<=enteteTOF(f,1)) && (stop=false))
     {
         lireDirTOF(f,a.numbloc,&buf);
         tmp=buf.tab[buf.nb]; k=buf.nb;
         while(k>j){
            buf.tab[k]=buf.tab[k-1];
            k--;
         }
         buf.tab[j]=student;
         if (buf.nb<b){
            buf.nb++;
            buf.tab[buf.nb]=tmp;
            ecrireDirTOF(f,i,&buf);
            i++;
            student=tmp; j=0;
         }

     }
     if (a.numbloc>enteteTOF(f,1)){
        buf.tab[0]=student;
        buf.nb=1;
        ecrireDirTOF(f,a.numbloc,&buf);
        affEnteteTOF(f,1,enteteTOF(f,1)+1);}

        affEnteteTOF(f,2,enteteTOF(f,2)+1);
 }*/

void affichageStruct(char fileName[256]){
    ijtrouv e;
    int i=1; int j=0; bool stop=false; bool trouv=false; int s,r;
    TOVC* f=ouvrir(fileName,'A'); int c1,c2;
    int dernierbloc=entete_fich(f,1);
    int dernierepos=entete_fich(f,2);
    fermer(f);
    char *clenreg=malloc(sizeof(char)*2); //taille de classeID
    char *longueur=malloc(sizeof(char)*2);
    char *Nomprenom=malloc(sizeof(char)*TNP);
    char *identifiant=malloc(sizeof(char)*4);
    char *Teff=malloc(sizeof(char)*1);
    while(!stop)
        {
            s=i; r=j;
            lire_chaine(fileName,longueur,2,&i,&j);
            lire_chaine(fileName,identifiant,4,&i,&j);
            lire_chaine(fileName,clenreg,2,&i,&j);
            printf("\ncle : %s",clenreg);
            lire_chaine(fileName,Teff,1,&i,&j);
            lire_chaine(fileName,Nomprenom,atoi(longueur)-24-1-2-4-1,&i,&j);
            printf("\nID : %s nomprenom : %s",identifiant,Nomprenom);
            if(strcomp(Teff,"1")==0){printf("\nNe Poursuit pas ses etudes actuellement a notre ecole (supprimee logiquement)");}
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            printf("\n------------------------");
        }

}
void affichageNotes(char fileName[256]){
    ijtrouv e;
    int i=1; int j=0; bool stop=false; bool trouv=false; int s,r,a,aa;
    TOVC* f=ouvrir(fileName,'A'); int c1,c2;
    int dernierbloc=entete_fich(f,1);
    int dernierepos=entete_fich(f,2);
    fermer(f);
    char *clenreg=malloc(sizeof(char)*2); //taille de classeID
    char *longueur=malloc(sizeof(char)*2);
    char *Nomprenom=malloc(sizeof(char)*TNP);
    char *Teff=malloc(sizeof(char)*1);
    char *TABLEAUNOTES=malloc(sizeof(char)*25);
    char *gender=malloc(sizeof(char)*1);
    while(!stop)
        {
            s=i; r=j;
            lire_chaine(fileName,longueur,2,&i,&j);
            manfonc(4,&i,&j);
            lire_chaine(fileName,clenreg,2,&i,&j);
            printf("\ncle : %s",clenreg);
            lire_chaine(fileName,Teff,1,&i,&j);
            lire_chaine(fileName,Nomprenom,atoi(longueur)-24-1-2-4-1,&i,&j);
            lire_chaine(fileName,gender,1,&i,&j);
            printf("\nnomprenom : %s",Nomprenom);
            if(strcomp(Teff,"1")==0){printf("\nNe Poursuit pas ses etudes actuellement a notre ecole (supprimee logiquement)");
            }else{
                lire_chaine(fileName,TABLEAUNOTES,24,&i,&j);
                a=0;
                printf("\nLES NOTES DES EXAMENS DE CE TRIMESTRE\n");
                while(a<24){
                   switch(a){
                    case 0:{printf("\nla note de l'examen de la langue arabe : ");}break;
                    case 3:{printf("\nla note de l'examen de l'education islamique : ");}break;
                    case 6:{printf("\nla note de l'examen des mathematiques : ");}break;
                    case 9:{printf("\nla note de l'examen de la langue berbere : ");}break;
                    case 12:{printf("\nla note de l'examen du francais : ");}break;
                    case 15:{printf("\nla note de l'examen de l'anglais : ");}break;
                    case 18:{printf("\nla note de l'examen de l'histoire/geo: ");}break;
                    case 21:{printf("\nla note de l'examen du sport : ");}break;
                }
                a++;
                    for(int aa=0;aa<2;aa++){
                        printf("%c",TABLEAUNOTES[a]);
                        a++;
                    }
                }


            }
            i=s; j=r;
            manfonc(atoi(longueur)+2,&i,&j);
            if((i==dernierbloc)&&(!trouv)&&(j==dernierepos)){stop=true;}
            printf("\n------------------------");
        }

}

void home()
{

    printf("\n\n\n\n");
    printf("\t\t\t\t __| |________________________________________________________________________________________________________________| |__\n\t\t\t\t");
    printf("(__   ________________________________________________________________________________________________________________   __)\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("          ___  ___ _                                                                                            ");
    printf("| |\n\t\t\t\t");
    printf("   | |");
    printf("         / _ \\/ __| |                                                                                           ");
    printf("| |\n\t\t\t\t");
    printf("   | |");
    printf("        |  __/\\__ \\ |                 Ecole Nationale Superieure d'informatique   (ex INI)                      ");
    printf("| |\n\t\t\t\t");
    printf("   | |");
    printf("         \\___||___/_|                 Cycle preparatoire integre (2CPI)                                         ");
    printf("| |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("        Travail Pratique :");
    printf("  : Gestion de la scolarite                          ");
    printf("| |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("        Realise par :");
    printf("     Boukhetala Zaineb   Chehboub Cerine Mona                                                    ");
    printf("| |\n\t\t\t\t");
    printf("   | |");
    printf("                                 section C Groupe 08                                                            ");
    printf("| |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("        Encadre par :");
    printf(" Mme. ARTABAZ Saliha                                                                           ");
    printf("| |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("        Version :  ");
    printf("| |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    printf("   | |");
    printf("                              APPUYER SUR LA TOUCHE ENTREE POUR CONTINUER                                       ");
    printf("| |\n\t\t\t\t");
    printf(" __| |_______________________________________________________________________________________________________________ | |__\n\t\t\t\t");
    printf("(__   ________________________________________________________________________________________________________________   __)\n\t\t\t\t");
    printf("   | |                                                                                                                | |\n\t\t\t\t");
    getch();
}


int main()
{
system("COLOR F9");
home();
buffer buff;
ijtrouv enregi;
char chaine[256];
int n;
Enreg e; char fileName[256];
printf("\nDonnez le nom du fichier que vous voulez manipuler pour qu'on puisse l'initialiser : ");
scanf("%s",&fileName);
printf("\navec combien d'enregistrements voulez vous initialiser le fichier: ");
scanf("%d",&n);
chargement_initial(fileName,n);
system("cls");
int CHOICE;
int Fin=1;
int FinProg=1;
    printf("\n\t\t\tBIENVENUE DANS LA PLATEFORME DE MANIPULATION DES FICHIERS DE NOTRE ECOLE PRIMAIRE\n");
    printf("\n                                                     ****MENU****: \n");
    printf("\n1- Insertion d'un eleve\n");
    printf("\n2- Affichage de la liste des eleves\n");
    printf("\n3- Mise a jour des notes\n");
    printf("\n4- Suppression d'un etudiant\n");
    printf("\n5- Archivage\n");
    printf("\nchoisissez une option: ");
    scanf("%d",&CHOICE);
    while(FinProg){
    switch(CHOICE){
           case 1:{
                Fin=1;
                while(Fin){
                system("cls");
                system("COLOR F9");
                insertionTOVC(fileName);
                affichageStruct(fileName);
                Fin=0;}
                system("pause");
           }break;
           case 2:{
                Fin=1;
                while(Fin){
                system("COLOR F9");
                system("cls");
                int affich;
                printf("\nvoulez vous l'affichage avec les notes (1) ou pas (simple, juste la cle et le nom: 0): ");
                scanf("%d",&affich);
                switch(affich){
                    case 0:{affichageStruct(fileName);}break;
                    case 1:{affichageNotes(fileName);}break;
                }
                Fin=0;}
                system("pause");
           }break;
           case 3:{
                Fin=1;
                while(Fin){
                system("cls");
                system("COLOR F9");
                miseajourTOVCnotes(fileName);
                affichageNotes(fileName);
                printf("\nVoulez-vous continuer l operation? (Taper 1 pour continuer et 0 pour retourner au menu) : ");
                scanf("%d",&Fin);}
            }break;
           case 4:{
                Fin=1;
                while(Fin){
                system("cls");
                system("COLOR F9");
                suppressionTOVC(fileName);
                affichageStruct(fileName);
                printf("\nVoulez-vous continuer l operation? (Taper 1 pour continuer et 0 pour retourner au menu) : ");
                scanf("%d",&Fin);}
           }break;
           case 5:{
                Fin=1;
                while(Fin){
                system("cls");
                system("COLOR F9");
                chargementIndex(fileName,n);
                printf("\nVoulez-vous continuer l operation? (Taper 1 pour continuer et 0 pour retourner au menu) : ");
                scanf("%d",&Fin);}
           }break;
           default : {return 0;};
    };
    system("cls");
    system("COLOR F9");
    printf("\nBIENVENUE DANS LA PLATEFORME DE MANIPULATION DES FICHIERS DE NOTRE ECOLE PRIMAIRE\n");
    printf("\n****MENU****: \n");
    printf("\n1- Insertion d'un eleve\n");
    printf("\n2- Affichage des eleves et leur notes\n");
    printf("\n3- Mise a jour des notes\n");
    printf("\n4- Suppression d'un etudiant\n");
    printf("\n5- Archivage\n");
    printf("\n6- Quitter\n");
    printf("\nchoisissez une option: ");
    scanf("%d",&CHOICE);};
return 0;
}

/**
Répondre au questions théoriques:
QUESTION:

Comment devez vous procéder pour construire un fichier de dossiers scolaires par
élève comprenant les années de scolarisation par élève et la moyenne de passage.
Quelle sera la clé ? quel type du modèle allez vous choisir ?
REPONSE:

la clé de l'index: la moyenne
modèle: TOF

QUESTION:
Quel type d’index choisir ?

REPONSE:
index dense

QUESTION:
Quel est le coût de l’archivage?

REPONSE:
le meilleur cas et le pire des cas, dépend de n (le nombre d'enregistrements dans le fichier
TOF (TOVC ARCHIVEE)
donc n*liredir()+n*ecriredir()

**/


//****//
