#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TailleBLC 70
#define Max_enreg 20
#define b 3
#define MatMax 15

/**déclaration des structures**/
typedef struct Tbloc
{
	char chaine[TailleBLC];
}	Tbloc ;

typedef struct Tbloc Buffer;

typedef struct TEnreg
{
    int numID; // 4 caractères
    int anneeScol; // L’année de scolarisation 5/P sur un caractère
    int classeID[1];// et numéro de salle
    char * NomPrenom;
    char genre[1];
    int tabNotes[MatMax];
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

void liredir(TOVC *pF,int i,Buffer *buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(buf,sizeof(Tbloc),1,pF->F);
    }
}

void ecriredir(TOVC *pF,int i,Buffer buf)
{
    printf("la");
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        printf("on est la");
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}
int alloc_bloc(TOVC *pF)
{
    aff_entete(pF,1,entete(pF,1)+1);
    return entete(pF,1);
}
TOVC chargement_initial(){
     int i; // nbr d'enreg
     char *anneeString;
     char *salleString;
     Enreg e;
     srand(time(0));
     // on veut une aléa entre 0 et 5
     int n1 = rand() % 6;
     int n2 = rand() % 4;
     // pour construire le tableau des notes
     int note = rand() % 10;
     for(i=0;i<5;i++){
            // Générer aléatoirement le numéro d’identification de l’élève.
            int studentID = rand() % 10000;
            // on va concaténér annee et salle
            int salle = rand() % 21;
            int annee = rand() % 6;
            printf("\n%d\n",salle);
            manipulerLaCle(salle,annee,salleString,anneeString);
         //  strcat(anneeString,salleString);
           // printf("%s", anneeString);

     }

}
void manipulerLaCle(int salle, int annee, char *stringSalle, char *stringAnnee){
    printf("on a entrer ici");
    sprintf(stringSalle,"%d", salle);
    printf("on a ca %s", stringSalle);
}
int main()
{
    TOVC fh;
    fh = chargement_initial();
    return 0;
}
