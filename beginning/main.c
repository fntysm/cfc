#include <stdio.h>
#include <stdlib.h>

int main()
{

}





C:/Users/lenovo/cfc/.git/



/**other important information**/
/**"a" ouverture en mode � ajout �
"r+" ouverture en lecture/�criture
"w+" cr�ation d'un nouveau fichier en lecture/�criture
"a+" ouverture en mode � ajout � en lecture/�criture
Si le mode contient le caract�re �b� le fichier est ouvert en mode binaire
Ex : "rb+" ou "r+b" d�signe une ouverture en lecture/�criture d�un fichier binaire*/


/**- d�but du fichier, si origine vaut SEEK_SET
- position courante, si origine vaut SEEK_CUR
- fin du fichier, si origine vaut SEEK_END*/


/**
c = fgetc( f );
Lit et retourne (type int) le prochain caract�re de f
Retourne EOF en cas de fin de fichier d�pass�e ou erreur.
fputc( c, f );
Ecrit le caract�re c � la position courante du fichier f. Retourne EOF si erreur.
fgets( buf, n, f );
Lit dans buf, tous les caract�res � partir de la position courante de f, jusqu��
trouver une fin de ligne '\n' (qui est aussi lue dans buf) ou alors jusqu�� ce que n-1
caract�res soient lus. Un caract�re de fin de cha�ne '\0' est rajout� � la fin de buf.
En cas d'erreur ou fin de fichier, la fonction fgets retourne NULL.
fputs( buf, f );
Ecrit tous les caract�res contenus de buf (sauf le '\0') dans le fichier f, � partir de
la position courante. Pour �crire une ligne, il faut pr�voir un caract�re '\n' � la fin
de buf (et avant le caract�re de fin de cha�ne '\0').
En cas d'erreur, la fonction retourne (type int) EOF.
Pour les E/S format�es :
fscanf( f, format, &var1, &var2, ... ) et fprintf( f, format, exp1, exp2, ... )



 manipulation des fichiers**/
    /**d�clarer une variable fichier f (de type flux)**
    FILE *fHandler1;
    FILE *fHandler2;
    /**pour ouvrir un fichier**
    mode1 = "r" /**ouverture en lecture*
    mode2 = "w" /**cr�ation d'un nouveau fichier (�crasement de l'ancien s'il existe d�j�)*
    fHandler1 = fopen("file", mode1);
    fHandler2 = fopen("fileN", mode2);
    /**sch�ma typique pour lire s�quentiellement le contenu d'un fichier**/
    /**Pour savoir si on a d�pass� la fin de fichier (en mode lecture) feof
    while(!(feof(fHandler1))){
        /**le nombre d'�l�ments lus=fread(le r�sultat de la lect sera plac� dans BUFF,la taille de chaque �l�ment,le nombre d'�l�ments lus cons�cutivement,fichier)
        elementLus = fread()
        /**le nombre d'�l�ments �crits=fwrite(r�cup�r�s depuis la zone point�e par buf,la taille de chaque �l�ment,le nb d'�l�ments �crits,fichier)
        elementEcr = fwrite()
        /**Pour modifier explicitement la position courante, fseek(fichier,d�place la position courante d'un nombre d'octets relativement li� �, origine)
        fseek()
    }
    /**pour fermer un fichier
    fclose(fHandler1);
    fclose(fHandler2);
*/
