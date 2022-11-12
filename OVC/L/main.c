#include <stdio.h>
#include <stdlib.h>
#define b 100
/**déclarations des structures**/
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
    int posLibre; // la première position libre dans le dernier bloc
    int nbrPerdues; // nombre de caractères perdues suite aux suppressions logiques
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
