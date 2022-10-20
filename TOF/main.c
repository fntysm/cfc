#include <stdio.h>
#include <stdlib.h>
#define MAX 23
#define b 4 // maximum d'enreg
typedef struct enreg{
    int key;
    char info[MAX]; // type qlq
}Tenreg;
typedef struct bloc{
    Tenreg Tab[b];
    int nb;
}Tbloc;
int main()
{
    Tenreg e;
    Tbloc buffer;
    int i,k,j;
    FILE* fh;
    char nomf[MAX];
    printf("enter le nom du fichier: ");
    scanf("%s",&nomf);
    fh = fopen(nomf,"wb");
    i = 0;
    j = 0;
    for(k=0,k<n,k++){
        scanf("%d %s",&e.key,&e.info);
        if(j<b){
            buffer.Tab[j]=e;
            j++;
        }else{
            buffer.nb = j
        }
        ecriredir(f,i,buf);
        j=0;
        i++;
        buffer.tab[0]=e;

    }
    return 0;
}
