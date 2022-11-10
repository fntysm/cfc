#include <stdio.h>
#include <stdlib.h>
#define MAX 17
#define MAF 25
#define b 4 // maximum d'enreg
//les déclarations des structures
typedef struct enreg{
    char key[MAX]; // la clé
    char info[MAX]; // type qlq
}Tenreg;

typedef struct bloc{
    Tenreg Tab[b];
    int nb;
}Tbloc;

typedef struct Entete
{
    int adr_dernier_bloc; //l'adresse du ernier bloc utilisé
}Entete;

typedef struct TOF
{
    FILE *F;
    Entete entete;
}TOF;

int entete(TOF *pF,int i)
{
    if (i==1) return ((pF->entete).adr_dernier_bloc);
}
void EcrireDir(TOF *pF,int i,Tbloc buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}
void aff_entete(TOF *pF,int i,int val)
{
    if (i==1) (pF->entete).adr_dernier_bloc=val;
}
void chargement_initial(char nomf[MAF], int n, float u){
    // n le nombre d'enreg qu'on veut insérer
    // u désigne le taux de chargement voulu au départ (il est entre 0 et 1)
    TOF *fh;
    Tbloc buffer;
    Tenreg e;
    fh = fopen(nomf,"wb");
    int i,j,k;
    i = 1; //num de bloc à remplir
    j = 1; //num d'enreg dans le bloc
    printf("donnez les enregistremens dans l'ordre croissant suivant la clé: ");
    for (k=0;k<n;k++){
        scanf("%s %s",&e.key,&e.info);
        printf("\nsuccessfully read\n");
        if(j<=(u*b)){
            buffer.Tab[j]=e;
            j++;
        }else{
            buffer.nb = j-1;
            EcrireDir(fh, i, buffer);
            buffer.Tab[1]=e;
            i++;
            j=2;
        }
    }
    buffer.nb = j-1;
    EcrireDir(fh,i,buffer);
    aff_entete(fh,1,i);
    fclose(fh);
    return;
}
int main()
{
    chargement_initial("fichier.bin",3,0.5);
    return 0;
}
