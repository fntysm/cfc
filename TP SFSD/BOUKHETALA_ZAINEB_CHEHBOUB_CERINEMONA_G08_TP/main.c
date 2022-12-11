#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
typedef struct Tbloc
{
	char chaine[TailleBLC];
	int suivant;
	int posLibre;
}	Tbloc ;

typedef struct TEnreg
{
    char longEnreg[TL];
    char numID[TID];
    char classID[TC];
    char NomPrenom[TNP];
    char genre;
    char Teff;
    char tabNotes[MatMax];
}   Enreg;

typedef struct Entete
{
    int adr_dernier_bloc;
    int nbr_enreg;
    int indice_libre;
    int nb_sup;
}Entete;

typedef struct TOVC
{
    FILE *F;
    Entete entete;
}	TOVC;

/**machine abstraite**/
TOVC *ouvrir(char *filename,char mod) // mod = 'A' ancien (rb+) || mod = 'N' nouveau (wb+)
{
    TOVC *I = malloc(sizeof(TOVC));
    char s[3];
    if ((mod == 'A') || (mod =='a')) sprintf(s,"rb+");
    else sprintf(s,"wb+");
    I->F=fopen(filename,s);
    if ((mod == 'A') || (mod =='a'))
    {
        fread(&(I->entete),sizeof(Entete),1,I->F);
    }
    else
    {
        (I->entete).adr_dernier_bloc=0;
        (I->entete).nbr_enreg=0;
        (I->entete).indice_libre=0;
        (I->entete).nb_sup=0;
        fwrite(&(I->entete),sizeof(Entete),1,I->F);
    }
    return I;
}
void fermer(TOVC * pF)
{

    fseek(pF->F,0,0);
    fwrite(&(pF->entete),sizeof(Entete),1,pF->F);
    fclose(pF->F);

}
int entete(TOVC *pF,int i)
{
    if (i==1) return ((pF->entete).adr_dernier_bloc);
    if (i==2) return ((pF->entete).nbr_enreg);
    if (i==4) return ((pF->entete).nb_sup);
    return ((pF->entete).indice_libre);
}
void aff_entete(TOVC *pF,int i,int val)
{
    if (i==1) (pF->entete).adr_dernier_bloc=val;
    else if (i==2) (pF->entete).nbr_enreg=val;
    else if (i==4) (pF->entete).nb_sup=val;
    else (pF->entete).indice_libre=val;
}

void liredir(TOVC *pF,int i,Tbloc buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(&buf,sizeof(Tbloc),1,pF->F);
    }
}

void ecriredir(TOVC *pF,int i,Tbloc buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}
int alloc_bloc(TOVC *pF)
{
    aff_entete(pF,1,entete(pF,1)+1);
    return entete(pF,1);
}
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
void chargement_initial(int n, TOVC* fh, char* fileName){
     int i,j,n1,n2,studentID,salle,annee,longu,classeID,f,k;
     char classID[3]; char anneeString[3]; char salleString[2]; char genre; char noms[MAX];
     char prenoms[MAX]; char studentIDchar[4]; int tabnotes[MatMax]; int note;
     char tabnotesChar[MatMax]; char tempID[TID] ; char tempcle[TC];
     Enreg e; Tbloc buff; char noteChar[MatMax]; int a=0; int nbrEnreg;
     fh = ouvrir(fileName,'N');
     srand(time(NULL));
     i=0; j=0;
     for(f=0;f<n;f++){
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
            strncpy(e.classID,anneeString,2);
            n1 = rand() % 7;
            n2 = rand() % 7;
            readLine("noms.txt",n1,noms);
            readLine("prenoms.txt",n2,prenoms);
            e.genre=prenoms[0];
            for(k=0;k<strlen(prenoms);k++){
                prenoms[k]=prenoms[k+1];
            }
            strcpy(e.NomPrenom,noms);
            strcat(e.NomPrenom,prenoms);
            if(e.classID[0]!='P'){
                while(a<MatMax){
                    if(a%3==0){
                    switch(a){
                    case 0:{e.tabNotes[0]='A';}break;
                    case 3:{e.tabNotes[3]='I';}break;
                    case 6:{e.tabNotes[6]='M';}break;
                    case 9:{e.tabNotes[9]='T';}break;
                    case 12:{e.tabNotes[12]='S';}break;}
                }else{
                    note = rand() % 21;
                    sprintf(noteChar,"%d", note);
                    if(note/10==0){
                     e.tabNotes[a]='0';
                     e.tabNotes[a+1]=noteChar[0];
                     a++;
                    }else{
                     e.tabNotes[a]=noteChar[0];
                     e.tabNotes[a+1]=noteChar[1];
                     a++;
                     }
                }
                a++;
                }
            }else{
                strcpy(e.tabNotes,"NULL");
                 }
            strcpy(&e.tabNotes[MatMax], &e.tabNotes[MatMax+1]);
            e.Teff = '0';
            ecrire_enreg(fh,e,i,j);
            aff_entete(fh,2,nbrEnreg++);
     }
     buff.posLibre = j;
     aff_entete(fh,2,n);
}
int ecrire_chaine(TOVC* fichier, char * chaine, int longu, int i, int j, Tbloc buff){
     int k, i1;
     printf("\nchaine to include: %s",chaine);
     for(k=0;k<longu;k++){
        if(j<TailleBLC){
                buff.chaine[j]=chaine[k];
                j++;
        }else{
        i1 = i;
        i = alloc_bloc(fichier);
        buff.suivant = i;
        buff.posLibre = j;
        j = 0;
        ecriredir(fichier,i1,buff);
        buff.chaine[j]=chaine[k];
        j++;
        }
     }
     printf("\nbuff.chaine: %s",buff.chaine);
     aff_entete(fichier,1,i);
     aff_entete(fichier,3,j);
     return j;
}
/*void ecrire_chainedjihane(char nom[256],char chaine[256],int lg)
{
    TOVC *f = ouvrir(nom,'N');

int i=0;
int j=0;
int x; int y;
Tbloc buf;
    for( int k=0; k<lg;k++)
    {
        if((j)<TailleBLC)
        {
           buf.chaine[j]=chaine[k];
           j++;
        }
        else{
        ecriredir(f,i,buf);
        alloc_bloc(f);
        i++;
         buf.chaine[0]=chaine[k];
        j=1;
        }
    }
    aff_entete(f,1,i);
    aff_entete(f,2,j);
    ecriredir(f,i,buf);
    x=entete(f,1);
    y=entete(f,2);
    fermer(f);
}
*/
int ecrire_char(TOVC* fichier, char c, int i,int j, Tbloc buff){
     int i1;
     if(j<TailleBLC){
                buff.chaine[j]=c;
                j++;
        }else{
        i1 = i;
        i = alloc_bloc(fichier);
        buff.suivant = i;
        buff.posLibre = j;
        j = 0;
        ecriredir(fichier,i1,buff);
        buff.chaine[j]=c;
        j++;
        }
        aff_entete(fichier,1,i);
        aff_entete(fichier,3,j);
        return j;
}
void ecrire_enreg(TOVC* fichier, Enreg e, int i, int j){
    int l,i1,k;
    Tbloc buff;
    char longueur[TL];
    liredir(fichier,i,buff);
    l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1;
    sprintf(longueur,"%d", l);
    j=ecrire_chaine(fichier,longueur,strlen(longueur),i,j,buff);
    j=ecrire_chaine(fichier,e.numID,strlen(e.numID),i,j,buff);
    j=ecrire_chaine(fichier,e.classID,strlen(e.classID),i,j,buff);
    j=ecrire_chaine(fichier,e.NomPrenom,strlen(e.NomPrenom),i,j,buff);
    j=ecrire_char(fichier,e.genre,i,j,buff);
    j=ecrire_char(fichier,e.Teff,i,j,buff);
    j=ecrire_chaine(fichier,e.tabNotes,strlen(e.tabNotes),i,j,buff);
    ecriredir(fichier,i,buff);
    printf("\non a apres insertion de tout l'enreg: %s",buff.chaine);
    aff_entete(fichier,3,entete(fichier,3)+TL+l);
}
void lire_chaine(TOVC *f, char chaine[256], int lg, int *s, int *r)
{
	int k; int i = *s; int j = *r; int m; Tbloc buf;
    liredir(f, i, buf);
    printf("\non est dans lirechaine: ");
    for ( k=0;k < lg; k++)
    {
        if ((j < TailleBLC)||(i == entete(f, 1)))
        {
            if ((i == entete(f, 1))&&(j == entete(f, 2))){break;}
            chaine[k] = buf.chaine[j];
            j++;
        }
        else
        {
            i++;
            liredir(f, i, buf);
            chaine[k] = buf.chaine[0];
            j = 1;
        }
        m = k;
    }
    chaine[m + 1] = '\0';
    if (j == TailleBLC)
    {
        i++;
        liredir(f, i, buf);
        j = 0;
    }
    *s = i;
    *r = j;
    printf("\nchaine: %s",chaine);
}
/*void affich_TOVC(TOVC * pF)
{
    int i=1,i1=1,j=0,j1=0;
    Enreg e;
    Tbloc* buff;
    printf("ENTETE : %d\t%d\t%d\t%d\n",entete(pF,1),entete(pF,2),entete(pF,3),entete(pF,4));
    while (i<=entete(pF,1))
    {
        liredir(pF,i,buff);
        lire_chaine(pF,buff->chaine,strlen(buff->chaine),i,j);
        printf("\n%s",buff->chaine);
        if (i==i1) printf(" \nDans le Bloc %d\n",i);
        else printf(" \ncommence du bloc %d et chevauche le bloc %d\n",i,i1);
        if (j1==TailleBLC) {i1++;j1=0;}
        i=i1;j=j1;
        if ((i==entete(pF,1)) && j==entete(pF,3)) break;
    }
}*/

/*void recherche(TOVC *fh,char * filename ,char clef[TC], char nomprenom[TNP], int longu , int trouv, int i, int j){
    Tbloc buff;
    fh = ouvrir(filename,'A');
    trouv=0; j=0; stop=0;
    if(entete(fh,1)!=0){
        i=0;
        liredir(fh,i,buff);
        while((i<=entete(fh,1))&&(!(trouv)&&!(stop))){

        }
    }
}*/
TOVC* insertion(char* filename){
    TOVC* fichier;
    int i,j;
    fichier = ouvrir(filename,'A');
    Enreg e;
    printf("\nveuillez inserer: \nl'identifiant: "); scanf("%s",&e.numID);
    printf("\nle nomprenom: "); scanf("%s",&e.NomPrenom);
    printf("\nla cle: "); scanf("%s",&e.classID);
    printf("\nle genre: "); scanf("%c",&e.genre);
    printf("\nle tableau des notes: A20I20M20T20S20: "); scanf("%s",&e.tabNotes);
    // recherche() sinon
    i = entete(fichier,1);
    j = entete(fichier,3);
    ecrire_enreg(fichier,e,i,j);
    return fichier;
}
int main()
{
    TOVC* fh;
    Enreg e;
    bool *trouv, *stop;
    int i,j;
    char chaine[256];
    chargement_initial(1,fh,"fichierdebut.bin");
  //  printf("\non va essayer de lire un seul enreg: \n");
    //affich_TOVC(fh);
    fh = insertion("fichierdebut.bin");
    return 0;
}
