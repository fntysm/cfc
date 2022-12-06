#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TailleBLC 70
#define Max_enreg 20
#define b 3
#define MatMax 22
#define MatNum 11
#define MAX 50
#define TL 3
#define TC 3
#define TID 4
#define TNP 25
/**déclaration des structures**/
typedef struct Tbloc
{
	char chaine[TailleBLC];
	int suivant;
	int posLibre;
}	Tbloc ;

typedef struct Tbloc Buffer;

typedef struct TEnreg
{
    char longEnreg[TL];
    char numID[TID]; // 4 caractères
    // int anneeScol; , L’année de scolarisation 5/P sur un caractère
    // int classeID[1]; et numéro de salle
    char classID[TC];
    char NomPrenom[TNP];
    char genre;
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
     int i,j,n1,n2, studentID, salle, annee, longu, classeID;
     char classID[3];
     char anneeString[3];
     char salleString[2];
     char genre;
     char noms[MAX];
     char prenoms[MAX];
     char studentIDchar[4];
     int tabnotes[MatMax];
     char tabnotesChar[MatMax];
     Enreg e;
     Tbloc buff;
     srand(time(0));
     for(i=0;i<n;i++){
            printf("\n\ni = %d\n",i);
            studentID = rand() % 10000;
            salle = rand() % 10;
            annee = 1 + rand() % 6;
            sprintf(salleString,"%d", salle);
            sprintf(anneeString,"%d", annee);
            strcat(anneeString,salleString);
            // on a effectuer la clé à sa place
            strcpy(classID,anneeString);
            printf("\nla clef de cet enregistrement: %s",e.classID);
            classeID = atoi(classID);
            if(classeID / 60 == 1){
                classID[0] = 'P';
            }
            n1 = rand() % 7;
            n2 = rand() % 7;
            readLine("noms.txt",n1,noms);
            readLine("prenoms.txt",n2,prenoms);
            e.genre = prenoms[0];
            printf("\non a le genre: %c",e.genre);
            for(j=0;j<strlen(prenoms);j++){
                prenoms[j]=prenoms[j+1];
            }
            printf("\nprenom : %s",prenoms);
            strcpy(e.NomPrenom,noms);
            strcat(e.NomPrenom,prenoms);
           // printf("",e.longEnreg,e.numID,e.classID,e.NomPrenom,e.genre,e.tabNotes);

     }
}
/*void enregInMC(TEnreg e, Tbloc buff){

}*/
void ecrire_chaine(TOVC* fichier, char chaine[TailleBLC], int longu, int i, int j){
     int k;
     int i1;
     Tbloc buff;
     for(k=0;k<longu;k++){
        if(j!=TailleBLC){
                buff.chaine[j]=chaine[k];
                j++;
        }
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
void ecrire_enreg(TOVC* fichier, Enreg e, int i, int j){
    int l;
    char longueur[TL];
    l = strlen(e.NomPrenom)+strlen(e.tabNotes)+4+2+1;
    sprintf(longueur,"%d", l);
    ecrire_chaine(fichier,longueur,TL,i,j);
    ecrire_chaine(fichier,e.numID,TID,i,j);
    ecrire_chaine(fichier,e.classID,TC,i,j);
    ecrire_chaine(fichier,e.NomPrenom,TNP,i,j);
    ecrire_chaine(fichier,e.genre,1,i,j);
    aff_entete(fichier,3,entete(fichier,3)+TL+l);
}
int main()
{
    TOVC* fh;
    Enreg e;
    Buffer buff;
    strcpy(e.classID,"trr");
    e.genre = 'C';
    strcpy(e.numID,"tres");
    strcpy(e.longEnreg,"tre");
    strcpy(e.tabNotes,"tresbien");
    strcpy(e.NomPrenom,"rem85");
    ecrire_enreg(fichier,e,1,1);

    return 0;
}
