#include <stdio.h>
#include <stdlib.h>
#define b 100
/**d�clarations des structures**/
typedef struct un_bloc{
    char Tab[b];
    int next;
}Tbloc;
typedef struct Tbloc buffer;
typedef struct maillon
{
    int val;
    int suivant;
}maillon;
typedef struct Entete{
    int premierBloc;
    int dernierBloc;
    int posLibre; // la premi�re position libre dans le dernier bloc
    int nbrPerdues; // nombre de caract�res perdues suite aux suppressions logiques
}entete;
typedef LnOVC{
    FILE *fh;
    entete enteTe;
}LnOVC;

int main()
{
    printf("Hello world!\n");
    return 0;
}
