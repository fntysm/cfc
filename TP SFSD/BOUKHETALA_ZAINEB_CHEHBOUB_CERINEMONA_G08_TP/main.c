#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TailleBLC 70
#define Max_enreg 20
#define b 3
#define MatMax 15
#define MAX 50

/**déclaration des structures**/
typedef struct Tbloc
{
	char chaine[TailleBLC];
}	Tbloc ;

typedef struct Tbloc Buffer;

typedef struct TEnreg
{
    int numID; // 4 caractères
    // int anneeScol; , L’année de scolarisation 5/P sur un caractère
    // int classeID[1]; et numéro de salle
    char * classID;
    char NomPrenom[50];
    char genre;
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
void chargement_initial(int n, TOVC fh){
     int i; // nbr d'enreg
     int n1,n2;
     char classID[3];
     char anneeString[3];
     char salleString[2];
     char genre, c;
     char noms[50];
     char prenoms[50];
     int studentID, salle, annee;
     Enreg e;
     srand(time(0));
     // pour construire le tableau des notes
     int note = rand() % 10;
     int classeID;
     for(i=0;i<n;i++){
            printf("\n\ni = %d\n",i);
            // Générer aléatoirement le numéro d’identification de l’élève.
            studentID = rand() % 10000;
            // on va concaténér annee et salle
            salle = rand() % 10;
            annee = 1 + rand() % 6; // if annee == 6 donc on parle de l'année préparatoire
            sprintf(salleString,"%d", salle);
            printf("\n%s",salleString);
            sprintf(anneeString,"%d", annee);
            printf("\n%s",anneeString);
            strcat(anneeString,salleString);
            strcpy(classID,anneeString);
            classeID = atoi(classID);
            if(classeID / 60 == 1){
                classID[0] = 'P';
            }
            // pour choisir les lignes dans les 2 fichiers;
            n1 = rand() % 8;
            n2 = rand() % 8;
            readLine("noms.txt",n1,noms);
            readLine("prenoms.txt",n2,prenoms);
            e.numID = studentID;
            e.classID = classID;
            e.genre = prenoms[0];
            strcpy(e.NomPrenom,noms);
            for(i=0;i<strlen(prenoms);i++){
                prenoms[i]=prenoms[i+1];
            }
            strcat(e.NomPrenom,prenoms);
            printf("\nle name: %s et le genre : %c",e.NomPrenom,e.genre);

     }
}
int main()
{
    TOVC fh;
    chargement_initial(2,fh);
    return 0;
}
