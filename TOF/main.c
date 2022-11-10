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
    int adr_dernier_bloc; //numero d'ordre
    int nbr_enreg;
    int indice_libre;
    int nb_sup;
}Entete;

int entete(TOVC *pF,int i)
{
    if (i==1) return ((pF->entete).adr_dernier_bloc);
    if (i==2) return ((pF->entete).nbr_enreg);
    if (i==4) return ((pF->entete).nb_sup);
    return ((pF->entete).indice_libre);
}
void EcrireDir(FILE* pF,int i,Tbloc buf)
{
    if (i<=entete(pF,1))
    {
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}
void chargement_initial(char nomf[MAF], int n, float u){
    // n le nombre d'enreg qu'on veut insérer
    // u désigne le taux de chargement voulu au départ (il est entre 0 et 1)
    FILE *fh;
    Tbloc buffer;
    Tenreg e;
    fh = fopen(nomf,"wb");
    int i,j,k;
    i = 1; //num de bloc à remplir
    j = 1; //num d'enreg dans le bloc
    printf("donnez les enregistremens dans l'ordre croissant suivant la clé: ");
    for (k=0;k<n;k++){
        scanf("%s %s",&e.key,&e.info);
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
    fclose(fh);
    return;
}
int main()
{
    return 0;
}
