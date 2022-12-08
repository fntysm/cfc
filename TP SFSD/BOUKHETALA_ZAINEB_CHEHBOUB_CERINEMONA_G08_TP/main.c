#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define TailleBLC 256
#define Max_enreg 20
#define b 3
#define MatMax 15
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
    char tabNotes[MatMax];
    char Teff;
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

void liredir(TOVC *pF,int i,Tbloc *buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(buf,sizeof(Tbloc),1,pF->F);
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
     char prenoms[MAX]; char studentIDchar[4]; int tabnotes[MatMax];
     char tabnotesChar[MatMax]; char tempID[TID] ; char tempcle[TC];
     Enreg e; Tbloc buff;
     fh = ouvrir(fileName,'N'); e.Teff = '0';
     //srand(time(NULL));
     i=0; j=0;
     for(f=0;f<n;f++){
            studentID = 999 + rand() % 10000;
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
                int a=0;
                if(a%3==0){
                    switch(a){
                    case 0:{e.tabNotes[0]='A';}break;
                    case 3:{e.tabNotes[3]='I';}break;
                    case 6:{e.tabNotes[6]='M';}break;
                    case 9:{e.tabNotes[9]='T';}break;
                }else{
                    e.tabNotes[a]= 1 + rand() % 21;
                }
            }
            ecrire_enreg(fh,e,i,j);
            printf("\ni = %d, ID: %s, la clef: %s, name: %s\n",f,e.numID,e.classID,e.NomPrenom);
            printf("genre: %c, Teff: %c, Tabnotes: %s\n",e.genre,e.Teff,e.tabNotes);
     }
     buff.posLibre = j;
     ecriredir(fh,i,buff);
     aff_entete(fh,2,n);
}
void ecrire_chaine(TOVC* fichier, char chaine[TailleBLC], int longu, int i, int j){
     Tbloc buff;
     int k, i1;
     for(k=0;k<longu;k++){
        if(j!=TailleBLC){
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
}
void ecrire_char(TOVC* fichier, char c, int i,int j){
     int i1;
     Tbloc buff;
     if(j!=TailleBLC){
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
}
void ecrire_enreg(TOVC* fichier, Enreg e, int i, int j){
    int l,i1,k;
    Tbloc buff;
    char longueur[TL];
    char numIDtemp[TID]; // to fix a bug
    l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1;
    sprintf(longueur,"%d", l);
    ecrire_chaine(fichier,longueur,strlen(longueur),i,j);
    ecrire_chaine(fichier,e.numID,strlen(e.numID),i,j);
    ecrire_chaine(fichier,e.classID,2,i,j);
    ecrire_chaine(fichier,e.NomPrenom,strlen(e.NomPrenom),i,j);
    ecrire_char(fichier,e.genre,i,j);
    ecrire_char(fichier,e.Teff,i,j);
    aff_entete(fichier,3,entete(fichier,3)+TL+l);
}
void lire_chaine(TOVC *f, char chaine[256], int lg, int *s, int *r)
{
    int i = *s; int j = *r; int m; Tbloc buf;
    liredir(f, i, &buf);
    for (int k=0;k < lg; k++)
    {
        if ((j < TailleBLC)||(i == entete(f, 1)))
        {
            if ((i == entete(f, 1))&&(j == entete(f, 2)))
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
}
/*void recherche(char * filename ,char clef[TC], char nomprenom[TNP], int longu , int trouv, int i, int j){
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
int main()
{
    TOVC* fh;
    Enreg e;
    bool *trouv, *stop;
    int i,j;
    chargement_initial(1,fh,"fichierdebut.bin");



    return 0;
}
