#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2048

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
        fgets(buffer, MAX, fH);
    };
    printf("the line number %d from the file %s's content:\n\n%s", lineNum, fileName, buffer);
    fclose(fH);
}
FILE* deleteLine(char fileName[MAX/2], int lineNum){
    FILE *fH;
    FILE *fNew;
    char buffer[MAX];
    char newFileName[MAX/2];
    fH = fopen(fileName, "r");
    if (fH == NULL){
            printf("\nINEXISTANT FILE\n");
            return;
    };
    strcpy(newFileName, "temp_");
    strcat(newFileName,fileName);
    fNew = fopen(newFileName,"w");
    int lines = 0;
    while(!(feof(fH))){
        fgets(buffer, MAX, fH);
        if (lineNum != lines) {
                fputs(buffer, fNew);
        };
        lines++;
    };
    fclose(fH);
    fclose(fNew);
    remove(fileName);
    rename(newFileName,fileName);
    return fNew;
}
void replaceLine(char fileName[MAX/2], int lineNum, char buffer[MAX]){
    FILE *fH;
    char content[MAX];
    int lines = 0;
    fH = fopen(fileName, "r");
    if (fH == NULL){
            printf("\nINEXISTANT FILE\n");
            return;
    };
    while(!(feof(fH)) && (lines<=lineNum)){
        lines++;
        fgets(content, MAX, fH);
        if(lines==lineNum){
            fputs(buffer, fH);
        }
    };
    fclose(fH);
}
int main()
{
    printf("\twelcome to the file manipulation c program\n\n");
    printf("what do you want to do?\n\n1. read a specific line from a file\n2. delete a specific line from a file\n3. replace a specific line in a file\n4. find the largest number in a file\n5. reverse a file\n6. merge contents of 2 files in a third file\n7. quit\n");
    int choice;
    FILE *fHandler;
    char fileName[MAX/2];
    int lineNumber;
    char buffer[MAX];
    printf("please enter your choice: ");
    scanf("%d",&choice);
    switch(choice){
    case 1:
        {
            system("cls");
            printf("you chose to read a specific line from a file\n");
            printf("enter the file's proper path: ");
            scanf("%s", &fileName);
            printf("\nenter the line number: ");
            scanf("%d", &lineNumber);
            readLine(fileName, lineNumber, buffer);
        }
        break;
    case 2:
        {
            system("cls");
            printf("you chose to delete a specific line from a file");
            printf("enter the file's proper path: ");
            scanf("%s", &fileName);
            printf("\nenter the line number: ");
            scanf("%d", &lineNumber);
            fHandler = deleteLine(fileName, lineNumber);
        }
        break;
    case 3:
        {
            system("cls");
            printf("you chose to replace a specific line in a file");
            printf("enter the file's proper path: ");
            scanf("%s", &fileName);
            printf("\nenter the line number: ");
            scanf("%d", &lineNumber);
            printf("\ninput the replacement text: ");
            scanf("%s", &buffer);
            replaceLine(fileName, lineNumber, buffer);
        }
        break;
    case 4:
        {
            system("cls");
            printf("you chose to find the largest number in a file");
        }
        break;
    case 5:
        {
            system("cls");
            printf("you chose to reverse a file");
        }
        break;
    case 6:
        {
            system("cls");
            printf("you chose to merge contents of 2 files in a third file");
        }
        break;
    case 7:
        {
            system("cls");
            printf("you chose to quit");
        }
        break;
    default:
        {
            printf("\n\nthe value chosen to input is not included in the menu, try one more time");
        }
    }
    return 0;
}




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
    /*FILE *fHandler;
    fHandler = fopen("io.txt","w");
    int time = 1;
    fputs("a test\n",fHandler);
    fprintf(fHandler,"this is a %d hit wonder\n", time);
    fclose(fHandler);
    FILE *fh;
    fh = fopen("file.txt","r");
    char read[15];
    char buff[256];
    // reading the file content as a straight string
     fgets(buff,256,fh);
    while (fscanf(fh,"%s",&read)!=-1){
        printf("Buffer 01: %s\n",read);
        printf("Buffer 02: %s\n",buff);
    }
    fclose(fh);*/
