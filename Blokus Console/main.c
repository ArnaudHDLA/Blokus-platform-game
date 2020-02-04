#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

void gotoligcol(int lig, int col)
{
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mycoord);
}

typedef struct piece    //structure utilisee pour definir les pieces du jeu
{
    char ** forme;  //matrice dynamique (5x5) contenant la forme de la piece
    unsigned char used[4];  //tableau indiquant si la piece a ete utilisee par un joueur
} piece;

char ** Dynamic2020Matrix(char ** array, char value)    //methode d'initialisation d'une matrice dynamique 20x20 remplie par la valeur value
{
    int i;
    int j;

    do
        array = (char**)malloc(20 * sizeof(char*)); //allocation de la 1e dimension
    while (array == NULL);

    for (i = 0; i < 20; i++)
        do
            array[i] = (char*)malloc(20 * sizeof(char));    //allocation de la 2e dimension
        while (array[i] == NULL);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
            array[i][j] = value;    // initialisation des cases a 'value'

    return array;
}

char ** HorizontalFlip(char** mat)  // retournement horizontal d'une piece
{
    int i;
    int j;
    char ** matflipped = NULL;

    // on enregistre les manipulation sur une nouelle matrice pour ne pas corrompre l'originale
    do
        matflipped = (char**)malloc(5 * sizeof(char*));
    while (matflipped == NULL);

    for (i = 0; i < 5; i++)
        do
            matflipped[i] = (char*)malloc(5 * sizeof(char));
        while (matflipped[i] == NULL);

    // on parcours la matrice de haut en bas et on copie de bas en haut
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            matflipped[i][j] = mat[4 - i][j];
    return matflipped;
}

char ** RotateCW(char** mat)  // pivotement sans le sens horaire d'une piece
{
    int i;
    int j;
    char ** matflipped = NULL;

    // on enregistre les manipulation sur une nouelle matrice pour ne pas corrompre l'originale
    do
        matflipped = (char**)malloc(5 * sizeof(char*));
    while (matflipped == NULL);

    for (i = 0; i < 5; i++)
        do
            matflipped[i] = (char*)malloc(5 * sizeof(char));
        while (matflipped[i] == NULL);

    // on parcours la matrice de haut en bas et on copie de droite a gauche
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            matflipped[j][4 - i] = mat[i][j];
    return matflipped;
}

char ** RotateCCW(char** mat)  // pivotement sans le sens anti-horaire d'une piece
{
    int i;
    int j;
    char ** matflipped = NULL;

    // on enregistre les manipulation sur une nouelle matrice pour ne pas corrompre l'originale
    do
        matflipped = (char**)malloc(5 * sizeof(char*));
    while (matflipped == NULL);

    for (i = 0; i < 5; i++)
        do
            matflipped[i] = (char*)malloc(5 * sizeof(char));
        while (matflipped[i] == NULL);

    // on parcours la matrice de haut en bas et on copie de gauche a droite
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            matflipped[4 - j][i] = mat[i][j];
    return matflipped;
}

unsigned char MoveUp(char ** placeOccupee)  //deplacement vers le haut d'une piece
{
    unsigned char ok = 1;
    unsigned short i;
    unsigned short j;

    // verifie que la piece n'est pas deja au bord haut de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[0][i] != '0')
            ok = 0;

    if (ok) // si on peut déplacer
    {
        for (i = 1; i < 20; i++)
            for (j = 0; j < 20; j++)
                placeOccupee[i - 1][j] = placeOccupee[i][j];    //décale toutes les lignes vers le haut
        for (i = 0; i < 20; i++)
            placeOccupee[19][i] = '0';  //la ligne la plus basse est donc forcement vide
    }

    return ok;// informe si la piece a pu etre deplacee
}

unsigned char MoveLeft(char ** placeOccupee)  //deplacement vers la gauche d'une piece
{
    unsigned char ok = 1;
    unsigned short i;
    unsigned short j;

    // verifie que la piece n'est pas deja au bord gauche de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[i][0] != '0')
            ok = 0;

    if (ok)// si on peut déplacer
        for (j = 0; j < 20; j++)
            for (i = 1; i < 20; i++)
                placeOccupee[j][i - 1] = placeOccupee[j][i];//décale toutes les lignes vers la gauche
    for (i = 0; i < 20; i++)
        placeOccupee[i][19] = '0';  //la ligne la plus a droite est donc forcement vide

    return ok;// informe si la piece a pu etre deplacee
}

unsigned char MoveRight(char ** placeOccupee)  //deplacement vers la droite d'une piece
{
    unsigned char ok = 1;
    short i;
    short j;

    // verifie que la piece n'est pas deja au bord droit de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[i][19] != '0')
            ok = 0;
    if (ok)// si on peut déplacer
    {
        for (j = 18; j >= 0; j--)
            for (i = 0; i < 20; i++)
                placeOccupee[i][j + 1] = placeOccupee[i][j];//décale toutes les lignes vers la droite

        for (i = 0; i < 20; i++)
            placeOccupee[i][0] = '0';   //la ligne la plus a gauche est donc forcement vide
    }
    return ok;// informe si la piece a pu etre deplacee
}

unsigned char MoveDown(char ** placeOccupee)  //deplacement vers le bas d'une piece
{
    unsigned char ok = 1;
    short i;
    short j;

    // verifie que la piece n'est pas deja au bord inferieur de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[19][i] != '0')
            ok = 0;
    if (ok)// si on peut déplacer
    {
        for (i = 18; i >= 0; i--)
            for (j = 0; j < 20; j++)
                placeOccupee[i + 1][j] = placeOccupee[i][j];//décale toutes les lignes vers le bas

        for (i = 0; i < 20; i++)
            placeOccupee[0][i] = '0';//la ligne la plus haute est donc forcement vide
    }
    return ok;  // informe si la piece a pu etre deplacee
}

void DisplayTempMatrix(char ** matrix, unsigned short joueur, char** placeOccupee)
{
    /* affichage de la matrice temporaire qui permet de placer
    une piece sans affecter la matrice de jeu */

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int i;
    int j;
    for (i = 0; i < 20; i++)
    {
        gotoligcol(i + 2, 2);   // decalage de 2 pour prendre en compte les bordures

        for (j = 0; j < 20; j++)
        {
            if (placeOccupee[i][j] != '0')  // on affiche la piece choisie en blanc
                printf("%c", 219);          // pour montrer qu'elle n'est pas posee pour l'instant
            else
            {
                switch (matrix[i][j])   // sinon on affiche les pieces de la matrice de jeu dans les couleurs des joueurs
                {
                // si la case n'est pas libre
                case '1':
                    SetConsoleTextAttribute(hConsole, 17);  // bleu
                    break;
                case '2':
                    SetConsoleTextAttribute(hConsole, 34);  // vert
                    break;
                case '3':
                    SetConsoleTextAttribute(hConsole, 68);  //rouge
                    break;
                case '4':
                    SetConsoleTextAttribute(hConsole, 238); //jaune
                    break;

                }
                if(matrix[i][j] == '0') //sinon on affiche un espace
                    printf(" ");
                else
                    printf("%c", matrix[i][j]);

                SetConsoleTextAttribute(hConsole, 7);	//default color
            }
        }
    }
}

void DisplayTerrain(char ** terrain)
{
    /* afiche le terrain de jeu avec les pieces
    les pieces sont colorees en fonction des joueurs */
    int i;
    int j;

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (i = 0; i < 20; i++)
    {
        gotoligcol(i + 2, 2);   // décalage de 2 (bordures)

        for (j = 0; j < 20; j++)
        {
            if (terrain[i][j] != '0')
            {
                switch (terrain[i][j])  // en fonction du joueur qui occupe la case
                {
                case '1':
                    SetConsoleTextAttribute(hConsole, 17);  //joueur 1: bleu
                    break;
                case '2':
                    SetConsoleTextAttribute(hConsole, 34); // joueur 2: vert
                    break;
                case '3':
                    SetConsoleTextAttribute(hConsole, 68);  // joueur 3: rouge
                    break;
                case '4':
                    SetConsoleTextAttribute(hConsole, 238); // joueur 4: jaune
                    break;

                }
                printf("%c", terrain[i][j]);
                SetConsoleTextAttribute(hConsole, 7);			//default color
            }
            else
                printf(" ");    //si la case est libre on affiche un vide
        }
    }
}

void DisplayAvailableBlocks(piece* pieces, int joueur)
{
    /* affiche la liste des pieces encore disponibles
    pour un joueur, elles sont a la couleur du joueur
    et ont leur identification imprimee dessus */
    unsigned short i;
    unsigned short j;
    unsigned short k;
    unsigned short placedblocks = 0;
    unsigned short offset = 2;  //pour le décalage de la matrice

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (i = 0; i < 13; i++)    //boucle permettant d'effacer les pieces du joueur précedent
    {
        gotoligcol(offset + i, 28);

        for (j = 0; j < 49; j++)
        {
            printf(" ");
        }
    }

    for (i = 0; i < 21; i++)    // on verifie toutes les pieces
    {
        if (pieces[i].used[joueur - 1] == 0)    // si la piece n'a pas ete utilisee
        {
            for (j = 0; j < 5; j++)
            {
                gotoligcol(offset + j, 28 + placedblocks % 10 * 5); //on l'affiche en decalant pour le pas ecraser les autres pieces
                for (k = 0; k < 4; k++)
                {
                    if (pieces[i].forme[j][k] != '0')   // on affiche la piece a la couleur du joueur
                    {
                        switch (joueur)
                        {
                        case 1:
                            SetConsoleTextAttribute(hConsole, 23);
                            break;
                        case 2:
                            SetConsoleTextAttribute(hConsole, 39);
                            break;
                        case 3:
                            SetConsoleTextAttribute(hConsole, 71);
                            break;
                        case 4:
                            SetConsoleTextAttribute(hConsole, 225);
                            break;

                        }
                        printf("%c", pieces[i].forme[j][k]);
                        SetConsoleTextAttribute(hConsole, 7);
                    }
                    else
                        printf(" ");
                }
            }
            placedblocks++; // on indique qu'un bloc a ete place pour pouvoir afficher le suivant a un endroit different
            if (placedblocks == 10 || placedblocks == 20)   // si on a affiche 10 ou 20 pieces, on est au bord du tableau, il faut descendre d'une ligne
            {
                if (placedblocks == 10)
                    offset += 6;
                else
                    offset += 4;

            }
        }
    }
}

void DisplayManipulationMatrix(char ** mat)
{
    // affiche le contenu de la grille de manipulation d'une piece
    unsigned short i;
    unsigned short j;

    for (i = 0; i < 5; i++)
    {
        gotoligcol(i + 19, 31);
        for (j = 0; j < 5; j++)
        {
            if (mat[i][j] != '0')
                printf("%c", 219);
            else
                printf(" ");
        }
    }

}

char ** BlockSelection(piece* pieces, int joueur, unsigned char playertype, unsigned short* sbindex)
{
    // fonction de placement d'une piece par un utilisateur ou pas l'ordinateur
    int i;
    int j;
    char selected = '&';
    unsigned char ok = 0;

    gotoligcol(18, 38);
    printf("Select a block:");


    if(playertype == 2) // si c'est un ordinateur qui a joue
    {
        Sleep(1000);
        selected = *sbindex;    // la piece selectionnee a ete determinee par la methode CheckPlacableBlock et son indice stocke dans sbindex
        pieces[selected].used[joueur - 1] = 1;  // on informe que la piece est utilisee
        DisplayManipulationMatrix(pieces[selected].forme);  // on l'affiche dans la matrice de manipulation
        selected+=65;   //pour pouvoir garder le return a la fin et qu'il soit unique
        Sleep(1000);    // delai d'attente pour que le(s) utilisateur(s) puissent voir ce que l'ordinateur fait
    }
    else    // si un utiisateur joue
    {
        do
        {
            fflush(stdin);
            scanf("%c", &selected); // on ecoute son choix de piece
            gotoligcol(18, 53);
            printf("    "); // on efface le champs de saisie pour qu'il soit degage en cas d'erreur utilisateur
            gotoligcol(18, 53);
            if (selected > 'U' || selected < 'A')   // si la touche a ete tapee en majuscule on la reduit en miuscule
                selected -= 32;
            if (pieces[selected - 65].used[joueur - 1] == 0)    //flag permettant de bloquer si la piece est deja prise
                ok = 1;
            fflush(stdin);
        }
        while (((selected < 'A' || selected > 'U') && (selected < 'a' || selected > 'u')) || ok == 0);  // on bloque tant que la piece est utilisee ou que la saisie est mauvaise
        if (selected > 'U' || selected < 'A')   // si la touche a ete tapee en majuscule on la reduit en miuscule
            selected -= 32;

        *sbindex = selected-65; // on enregistre la piece selectionnee
        pieces[*sbindex].used[joueur - 1] = 1; //et on la declare comme utilisee par le joueur

        for (i = 0; i < 5; i++) // on affiche ensuite la piece dans la matrice de manipulation
        {
            gotoligcol(i + 19, 31);
            for (j = 0; j < 5; j++)
                if (pieces[selected - 65].forme[i][j] != '0')
                    printf("%c", 219);
                else
                    printf(" ");
        }

    }


    // affichage du block selectionné dans la matrice de manipulation
    gotoligcol(19, 38);
    printf("Rotation/Inversion (touches G-D-I): ");

    return pieces[selected - 65].forme; //retourne la forme de la piece choisie
}

char ** Manipulation(char ** mat, unsigned char playertype, unsigned short* moves)
{
    // fonction permettant de manipuler la piece choisie dans BlockSelection()
    char manip;
    int i;
    int j;

    if(playertype == 1) // si c'est le tour d'un utilisateur on le laisse choisir les manipulations
    {
        manip = ' ';
        while (manip != '\r')   //tant que l'utilisateur ne valide pas son choix par la touche ENTER
        {
            if (kbhit())    //si une touche est pressee
            {
                manip = getch();    //on enregistre la touche pressee
                switch (manip)
                {
                case 'g':
                case 'G':
                    mat = RotateCCW(mat);   //G pour Gauche donc rotation anti-horaire
                    break;

                case 'd':
                case 'D':
                    mat = RotateCW(mat);    //D pour droite donc rotation horaire
                    break;

                case 'i':
                case 'I':
                    mat = HorizontalFlip(mat);  //I pour inversion
                    break;
                }
                DisplayManipulationMatrix(mat); //affichage de la piece apres manipulation
            }
        }
    }
    else if(playertype == 2)    // si c'est l'ordinateur qui joue
    {
        for(i = 0; i< *moves+1; i++)    //on reprend l'algo de la fonction EndGame() et on reproduit les manipulations
        {
            //qu'elle arrive a la bonne forme
            // le nombre de manipulations a ete enregistre dans la variable moves
            if(i != 4)
            {
                mat = RotateCW(mat);
                DisplayManipulationMatrix(mat);
                Sleep(1000);
            }
            else
            {
                mat = HorizontalFlip(mat);
                DisplayManipulationMatrix(mat);
                Sleep(1000);

                mat = RotateCW(mat);
                DisplayManipulationMatrix(mat);
                Sleep(1000);
            }
        }
    }
    //vider matrice de manipulation
    for (i = 0; i < 5; i++)
    {
        gotoligcol(i + 19, 31);
        for (j = 0; j < 5; j++)
            printf(" ");
    }
    return mat; //on retourne la matrice manipulee
}

unsigned char ControlConnection(char ** placeOccupee, char** placeOccupeeFinal, int joueur)
{
    /* fonction essentielle permettant de controler la connexite de pieces
    elle renvoie un flag informant si la piece (temporairement dans placeOccupee)
    peut etre placee ou non dans le terrain de jeu (placeOccupeeFinal) */
    unsigned char test = 0;
    unsigned char valid = 0;
    short i;
    short j;
    unsigned short compteurMorceauValide = 0;
    unsigned short compteurMorceau = 0;

    for (i = 0; i < 20; i++)    //compte le nombre de blocks qui composent la piece
        for (j = 0; j < 20; j++)
            if (placeOccupee[i][j] != '0')
                compteurMorceau += 1;

    for (i = 0; i < 20; i++)    //on parcours toute la matrice
        for (j = 0; j < 20; j++)
            // controle que la piece n'en touche pas une autre du meme joueur sur ses bords
            // et qu'elle touche au moins une autre piece du meme joueur sur l'angle
            if(placeOccupee[i][j] == joueur + '0' && placeOccupeeFinal[i][j] == '0')   // controle la connexite uniquement aux abords de la piece et si l'emplacement est libre
            {
                if(i == 0 || j == 0 || i == 19 || j == 19)  // si une piece touche un des bords
                {
                    if(i == 0)  //contact du bloc sur le bord haut
                    {
                        if(j!= 0 && j != 19)
                            if((placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0')
                                    && ((placeOccupeeFinal[i + 1][j - 1] == joueur + '0') || (placeOccupeeFinal[i + 1][j + 1] == joueur + '0')))
                                test = 1;
                    }
                    if(j == 0)  //contact du bloc sur le bord gauche
                    {
                        if(i!= 0 && i != 19)
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0')
                                    && ((placeOccupeeFinal[i + 1][j + 1] == joueur + '0') || (placeOccupeeFinal[i - 1][j + 1] == joueur + '0')))
                                test = 1;
                    }
                    if(i == 19) //contact du bloc sur le bord bas
                    {
                        if(j != 19 && j != 0)
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0')
                                    && ((placeOccupeeFinal[i - 1][j - 1] == joueur + '0') || (placeOccupeeFinal[i - 1][j + 1] == joueur + '0')))
                                test = 1;
                    }
                    if(j == 19)     //contact du bloc sur le bord droit
                    {
                        if(i != 19 && i != 0)
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0')
                                    && ((placeOccupeeFinal[i - 1][j - 1] == joueur + '0') || (placeOccupeeFinal[i + 1][j - 1] == joueur + '0')))
                                test = 1;
                    }
                }
                else
                {
                    if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0')
                            && ((placeOccupeeFinal[i - 1][j - 1] == joueur + '0') || (placeOccupeeFinal[i + 1][j - 1] == joueur + '0') || (placeOccupeeFinal[i + 1][j + 1] == joueur + '0') || (placeOccupeeFinal[i - 1][j + 1] == joueur + '0')))
                        test = 1;
                }
            }

    //Test si la piece n'a pas d'autres pièces qui la touche directement

    if(test)
        for (i = 0; i < 20; i++)
            for (j = 0; j < 20; j++)
                if (placeOccupee[i][j] == joueur + '0' && placeOccupeeFinal[i][j] == '0')
                {
                    if(i == 0 || j == 0 || i == 19 || j == 19)
                    {
                        if(i == 0 && j != 0 && j != 19)
                        {
                            if((placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0'))
                                compteurMorceauValide += 1; // Compte le nombre de morceau valide de la pièce
                        }
                        if(j == 0 && i != 0 && i != 19)
                        {
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0'))
                                compteurMorceauValide += 1; // Compte le nombre de morceau valide de la pièce
                        }
                        if(i == 19 && j != 0 && j != 19)
                        {
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0'))
                                compteurMorceauValide += 1; // Compte le nombre de morceau valide de la pièce
                        }
                        if(j == 19 && i != 0 && i != 19)
                        {
                            if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') && (placeOccupeeFinal[i][j - 1] != joueur + '0'))
                                compteurMorceauValide += 1; // Compte le nombre de morceau valide de la pièce
                        }
                    }
                    else
                    {
                        if((placeOccupeeFinal[i - 1][j] != joueur + '0') && (placeOccupeeFinal[i + 1][j] != joueur + '0') &&
                                (placeOccupeeFinal[i][j - 1] != joueur + '0') && (placeOccupeeFinal[i][j + 1] != joueur + '0'))
                            compteurMorceauValide += 1; // Compte le nombre de morceau valide de la pièce
                    }
                }
    if(compteurMorceau == compteurMorceauValide)
        valid = 1;
    return valid;
}

short CheckPlacableBlock(piece* pieces, int joueur, char** placeOccupee, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves, unsigned char ajustsbindex)
{
    // fonciton controlant la possibilité de placer une piece dispo dans la matrice quelle que soit la position ou l'orientation
    unsigned short i=0;
    unsigned short j=0;
    unsigned short k=0;
    unsigned short m=0;
    unsigned short l=0;
    unsigned short a=0;
    short win = -1; // determine si le joueur a perdu/gagne/continue de jouer
    char** block = NULL;    //MODIF

    //initialisation de la matrice temporaire permettant la manipulation des pieces sans atteindre les pieces
    do
        block = (char**)malloc(20 * sizeof(char*));
    while (block == NULL);

    for (i = 0; i < 20; i++)
        do
            block[i] = (char*)malloc(20 * sizeof(char));
        while (block[i] == NULL);
    /**************/

    i = 0;
    while(i<21 && win != 0) //on parcours toutes les pieces tant que l'on a pas pu en trouver une qui peut etre placee
    {
        if(pieces[i].used[joueur-1] == 0)   //si la piece n'est pas prise on verifie si elle peut etre placee
        {
            for(j=0; j<5; j++)
                for(k=0; k<5; k++)
                    block[j][k] = pieces[i].forme[j][k];    //on stock la piece pour pouvoir faire les rotations sans affecter la piece originale dans le tableau de base


            m = 0;
            while(m<8 && win != 0)  // on essaye toutes les orientations possibles (8) tant qu'on a pas réussi a placer la piece
            {
                if(m != 4)
                    block = RotateCW(block);    //on effectue les tests sur les 4 rotations
                else
                {
                    block = HorizontalFlip(block);  //et ensuite avec les 4rotations de la piece inversee
                    block = RotateCW(block);
                }
                //pour etre sur que la piece commence en haut apres avoir effectue la rotation
                j = 0;
                l = 0;
                // les deux while() permettent de ne pas tenir compte des lignes/colonnes vides dans la matrice de manipulation
                // pour s'assurer que l'on place la piece au coin supérieur gauche au debut du balayage
                while(block[j][0] == '0' && block[j][1] == '0' && block[j][2] == '0' && block[j][3] == '0' && block[j][4] == '0')
                    j++;
                while(block[0][l] == '0' && block[1][l] == '0' && block[2][l] == '0' && block[3][l] == '0' && block[4][l] == '0')
                    l++;

                for(a=j; a<5; a++)
                {
                    for(k=l; k<5; k++)
                    {
                        if(block[a][k] != '0')
                            placeOccupee[a-j][k-l] = joueur + '0';  //on place donc la piece au bord supérieur gauche
                    }
                }
                do
                {
                    l = 0;
                    while(l<20 && win != 0) //on balaye de gauche a droite tant que la piece ne peut pas etre placee
                    {
                        if(!ControlConnection(placeOccupee, placeOccupeeFinal, joueur)) //si la piece ne peut pas etre placee cet endroit on decale la piece vers la droite
                            MoveRight(placeOccupee);
                        else
                        {
                            //sinon on retient l'index de la piece et le nombre de manipulations faites pour arriver a ce placement
                            // cela servira lorsque l'ordinateur devra placer une piece (voir ComputerPlaceBlock())
                            if(ajustsbindex)
                                *sbindex = i;
                            *moves = m;
                            win = 0;    //si mon peut placer la piece, le joueur est toujours en jeu
                        }

                        l++;
                    }
                    if(win != 0)    //si on a pas encore trouvé de piece qui peut etre posée dans le jeu
                        for(l = 0; l<20; l++)   // arrivé a la fin de la colonne on ramene la piece a gauche
                            MoveLeft(placeOccupee);
                }
                while(win != 0 && MoveDown(placeOccupee));  //et on la descend d'une ligne (si on a pas déjà trouvé une piece compatible

                if(win != 0)
                    for(l = 0; l<20; l++)
                        for(j = 0; j<20; j++)
                            placeOccupee[l][j] = '0'; //vider la matrice pour la prochaine piece a controler
                m++;
            }
        }
        i++;
    }
    return win;
    // -1 = le joueur a perdu
    // 0 = le joueur peut encore jouer
}

char ** PlaceBlock(char ** selectedblock, unsigned char joueur, unsigned char playertype, int tour, char** placeOccupeeFinal, piece* pieces, unsigned short* sbindex, unsigned short* moves)
{
    // fonction permettant au joueur de placer sa piece sur le plateau
    char manip = ' ';
    int i;
    int j;
    char ** placeOccupee = NULL;
    unsigned char validposition = 0;

    placeOccupee = Dynamic2020Matrix(placeOccupee, '0');    //initialisation de la matrice temporaire

    // placer la piece dans la matrice temporaire
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (selectedblock[i][j] != '0')
                placeOccupee[i+7][j+7] = joueur + '0';  //on place la piece choisie au milieu du plateau (dans un but de jouabilité)

    gotoligcol(20, 38);
    printf("Placez la piece sur le plateau (Z-Q-S-D)");

    //afficher la matrice de jeu puis la matrice temp
    // offset de 2 a cause des bordures
    DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee); //on affiche la piece sur le plateau
    manip = ' ';
    while (!validposition)  //tant que la position n'est pas valide
    {
        manip = ' ';
        do
        {
            if (kbhit())
            {
                manip = getch();

                switch (manip)  //en fonction de la demande de l'utilisateur on va deplacer la piece dans toutes les directions
                {
                // on fait suivre les deux cas en majuscule et en minuscule pour que les deplacements marchent meme si le clavier est en verouillage majuscule
                case 'z':
                case 'Z':
                    MoveUp(placeOccupee);
                    break;

                case 'q':
                case 'Q':
                    MoveLeft(placeOccupee);
                    break;

                case 'd':
                case 'D':
                    MoveRight(placeOccupee);
                    break;

                case 's':
                case 'S':
                    MoveDown(placeOccupee);
                    break;
                }
                if (tour == 0)  // au 1er tour on check juste si les pieces sont bien dans les coins
                {
                    if ((placeOccupee[0][0] != '0' && placeOccupeeFinal[0][0] == '0') || (placeOccupee[0][19] != '0' && placeOccupeeFinal[0][19] == '0') || (placeOccupee[19][0] != '0' && placeOccupeeFinal[19][0] == '0') || (placeOccupee[19][19] != '0' && placeOccupeeFinal[19][19] == '0'))
                        validposition = 1;
                    else
                        validposition = 0;
                }

                if (ControlConnection(placeOccupee, placeOccupeeFinal, joueur) && tour != 0)   //verification que la piece peut etre posee a cet endroit
                {
                    validposition = 1;
                    gotoligcol(21, 38);
                    printf("Vous pouvez placer la piece ici !");
                }
                else
                {
                    gotoligcol(21, 38);
                    if(tour !=0)
                    {
                        printf("Impossible de placer la piece ici !");
                        validposition = 0;
                    }
                }
                DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee);
            }

            gotoligcol(22, 38);
        }
        while (manip != '\r' && manip != 'X' && manip != 'x' && manip != 'h' && manip !='H');
        //on poursuit tant que le joueur ne valide pas son choix
        // ou qu'il ne demande pas de conseil (H) ou qu'il ne change pas de piece
        if(manip == 'X' || manip == 'x' || ((manip == 'h' || manip == 'H')&& tour != 0))
            validposition = 1;
        // s'il choisit de changer de piece ou qu'il veut un conseil on passe validposition a 1 pour permettre l'echappement de la boucle while
    }

    if(manip == '\r')   // si le joueur valide son placement
    {
        for (i = 0; i < 20; i++)	//place block
            for (j = 0; j < 20; j++)
                if (placeOccupee[i][j] != '0')
                    placeOccupeeFinal[i][j] = placeOccupee[i][j];   // on inscrit la piece dans le plateau

        for (i = 0; i < 20; i++)
            for (j = 0; j < 20; j++)
                placeOccupee[i][j] = '0';   //et on vide la matrice temporaire

        for (i = 0; i < 4; i++)
        {
            gotoligcol(18+i, 38);   //on efface les champs qui guident l'utilisateur
            printf("                                        ");
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            gotoligcol(18+i, 38);   //on efface les champs qui guident l'utilisateur
            printf("                                        ");
        }
        if(manip == 'H' || manip == 'h')    // si le joueur a besoin d'une suggestion de placement
        {
            for (i = 0; i < 20; i++)	//place block
                for (j = 0; j < 20; j++)
                    placeOccupee[i][j] = '0';   //on vide la matrice temporaire

            CheckPlacableBlock(pieces, joueur, placeOccupee, placeOccupeeFinal, sbindex, moves, 0); //on verifie où une piece peut etre posée (enregistré dans placeOccupee)
            DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee); // et on affiche la suggestion
        }
        pieces[*sbindex].used[joueur-1] = 0;    //on declare la piece comme disponible
        DisplayAvailableBlocks(pieces, joueur); //on réaffiche alors les pieces disponibles
        selectedblock = BlockSelection(pieces, joueur, playertype, sbindex);    //et on demande a l'utilisateur son nouveau choix
        selectedblock = Manipulation(selectedblock, playertype, moves);
        placeOccupeeFinal = PlaceBlock(selectedblock, joueur, playertype, tour, placeOccupeeFinal, pieces, sbindex, moves);
    }
    return placeOccupeeFinal;   //on renvoie alors le terrain actualisé
}

char ** ComputerPlaceBlock(char ** placeOccupee, unsigned char joueur, int tour, char** placeOccupeeFinal)
{
    // fonction qui place la piece lorqu'un ordinateur joue
    int i;
    int j;

    if(tour == 0)   // au 1er tour on place simplement la piece dans un angle
    {
        if(placeOccupeeFinal[0][0] == '0')
            placeOccupeeFinal[0][0] = joueur + '0';
        else
        {
            if(placeOccupeeFinal[0][19] == '0')
                placeOccupeeFinal[0][19] = joueur + '0';
            else
            {
                if(placeOccupeeFinal[19][19] == '0')
                    placeOccupeeFinal[19][19] = joueur + '0';
                else
                    placeOccupeeFinal[19][0] = joueur + '0';
            }
        }
    }
    else
    {

        // placer la piece dans la matrice temporaire
        for (i = 0; i < 5; i++)
            for (j = 0; j < 5; j++)
                if (placeOccupee[i][j] != '0')
                    placeOccupeeFinal[i][j] = placeOccupee[i][j];

        gotoligcol(20, 38);
        printf("L'ordinateur place la piece");

        //afficher la matrice de jeu puis la matrice temp
        // offset de 2 a cause des bordures
        DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee);

        for (i = 0; i < 20; i++)	//place block
            for (j = 0; j < 20; j++)
                if (placeOccupee[i][j] != '0')
                    placeOccupeeFinal[i][j] = placeOccupee[i][j];

        for (i = 0; i < 20; i++)
            for (j = 0; j < 20; j++)
                placeOccupee[i][j] = '0';

        for (i = 0; i < 4; i++)
        {
            gotoligcol(18+i, 38);   //on efface les champs qui guident l'utilisateur
            printf("                                        ");
        }
    }
    return placeOccupeeFinal;
}

void regles()
{

    printf("Regles\n");
    printf("Le Blokus est un jeu de 2 à 4 joueurs.\n");
    printf("Chaque joueur dispose de 21 pieces differentes, se voit attribue une couleur ainsi qu'un coin du plateau ou il devra jouer la premiere piece de son choix. ");
    printf(" Les pieces d’un joueur doivent etre connectees uniquement via un angle.\n");
    printf("Un joueur sera considere comme gagnant s'il arrive a poser toutes ses pieces en premier ou s'il possede le moins de pieces a la fin de la partie.\n\n");
    printf("Appuyez sur une touche pour retourner au menu\nAppuyez sur 4 pour terminer");

    fflush(stdin);
    getchar();
}

void DisplayGrid()
{
    // fonction qui permet l'affichage des bordures du jeu
    HANDLE hConsole;
    int i;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    /***********AFFICHAGE MAIN MATRIX************/
    SetConsoleTextAttribute(hConsole, 31);
    printf("  ABCDEFGHIJKLMNOPQRST\n\n");
    for (i = 0; i < 20; i++)
        printf("%c\n", 'A' + i);

    gotoligcol(1, 1);
    for (i = 0; i < 21; i++)
        if (i == 0)
            printf("%c", 201);
        else
            printf("%c", 205);
    printf("%c", 187);

    for (i = 0; i < 20; i++)
    {
        gotoligcol(i + 2, 1);
        printf("%c", 186);
        gotoligcol(i + 2, 22);
        printf("%c", 186);
    }
    printf("\n ");
    for (i = 0; i < 21; i++)
        if (i == 0)
            printf("%c", 200);
        else
            printf("%c", 205);
    printf("%c", 188);
    SetConsoleTextAttribute(hConsole, 7);
    /*******************************************/

    /********AFFICHAGE SUB MATRIX*************/
    //49 x 13
    gotoligcol(1, 27);
    for (i = 0; i < 50; i++)
        if (i == 0)
            printf("%c", 201);
        else
            printf("%c", 205);
    printf("%c", 187);

    for (i = 0; i < 15; i++)
    {
        gotoligcol(i + 2, 27);
        printf("%c", 186);
        gotoligcol(i + 2, 77);
        printf("%c", 186);
    }
    gotoligcol(17, 27);
    for (i = 0; i < 50; i++)
        if (i == 0)
            printf("%c", 200);
        else
            printf("%c", 205);
    printf("%c", 188);
    /************************************/


    /***********AFFICHAGE FLIP MATRIX*****/
    gotoligcol(18, 30);
    for (i = 0; i < 6; i++)
        if (i == 0)
            printf("%c", 201);
        else
            printf("%c", 205);
    printf("%c", 187);

    for (i = 0; i < 5; i++)
    {
        gotoligcol(i + 19, 30);
        printf("%c", 186);
        gotoligcol(i + 19, 36);
        printf("%c", 186);
    }
    gotoligcol(24, 30);

    for (i = 0; i < 6; i++)
        if (i == 0)
            printf("%c", 200);
        else
            printf("%c", 205);
    printf("%c", 188);
    /***************************************/
}

short EndGame(piece* pieces, char ** placeOccupee, int joueur, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves)
{
    // fonction de controle permettant de verifier si un joueur a gagné, perdu ou est toujours en jeu
    char ** block = NULL;
    unsigned short i=0;
    unsigned short a = 0;
    unsigned short b = 0;
    unsigned char noblockleft = 0;
    short win=1;      //si on entre pas dans le if() principal, c'est gagné

    for (a = 0; a < 20; a++)
        for (b = 0; b < 20; b++)
            placeOccupee[a][b] = '0';

    //initialisation block
    do
        block = (char**)malloc(5 * sizeof(char*));
    while (block == NULL);

    for (a = 0; a < 5; a++)
        do
            block[a] = (char*)malloc(5 * sizeof(char));
        while (block[a] == NULL);

    for (a = 0; a < 5; a++)
        for (b = 0; b < 5; b++)
            block[a][b] = '0';

    while(i<21 && noblockleft == 0)     //check if block left
    {
        if(pieces[i].used[joueur-1] == 0)
            noblockleft = 1;
        else
            i++;
    }

    if(noblockleft == 1)    //if block left then go
        win = CheckPlacableBlock(pieces, joueur, placeOccupee, placeOccupeeFinal/*, block*/, sbindex, moves, 1);

    return win;
// -1 if loose
// 0 if continue
// 1 if win
}

char ** PlayerMove(piece* allpieces,int i,unsigned char playerstype,int j,char ** terrain, unsigned short* moves)
{
    // cette fonction effectue la serie d'actions a faire pas l'utilisateur pour placer une piece
    // choisir la piece, la manipuler et la placer
    char ** result = NULL;
    char ** selectedblock = NULL;
    unsigned short selectedblockindex;

    selectedblock = BlockSelection(allpieces, i, playerstype, &selectedblockindex);
    selectedblock = Manipulation(selectedblock, playerstype, moves);
    result = PlaceBlock(selectedblock, i, playerstype, j, terrain, allpieces, &selectedblockindex, moves);
    return result;
}

char ** ComputerMove(piece* allpieces, char ** placeOccupee, int i, unsigned char playerstype, int j, char ** terrain, unsigned short * sbindex, unsigned short* moves)
{
    // placement d'une piece par l'ordinateur
    char ** result = NULL;
    char ** selectedblock = NULL;
    unsigned short a;
    unsigned short b;

    if(j == 0)
        *sbindex = 0;
    selectedblock = BlockSelection(allpieces, i, playerstype, sbindex);
    if(j != 0)
        selectedblock = Manipulation(selectedblock, playerstype, moves);
    else
    {
        for (a = 0; a < 5; a++)
        {
            gotoligcol(a + 19, 31);
            for (b = 0; b < 5; b++)
                printf(" ");
        }
    }
    result = ComputerPlaceBlock(placeOccupee, i, j, terrain);   // on place la piece et on sauvegarde le terrain de jeu actualisé
    return result;
}

unsigned char Sauvegarder(char** mat2, piece* pieces, int tour, int joueur, int maxplayer, unsigned short loosers, short playeringame[4], float tempsjoueur[4])
{
    // cette fonction permet d'enregistrer toutes les variables de jeu nécessaires pour reconstituer la partie
    int i;
    int j;
    unsigned char result = 0;
    FILE* fics = NULL;
    fics = fopen("lastgame.txt", "w");  //on ouvre le fichier de sauvegarde

    if(fics!=NULL)  // si on a bien pu ouvrir le fichier
    {
        result = 1;
        // copy matrix
        for (i=0; i<20; i++)
        {
            for (j=0; j<20; j++)
            {
                fprintf(fics, "%c", mat2[i][j]); //Ecriture de la matrice dans le fichier
            }
            fprintf(fics, "\n");
        }
        //copy blocks used
        for (i=0; i<21; i++)
            fprintf(fics, "\n%d %d %d %d", pieces[i].used[0], pieces[i].used[1], pieces[i].used[2], pieces[i].used[3]);
        //copy metadata
        fprintf (fics, "\n%d\n%d\n%d\n%d", tour, joueur, maxplayer, loosers); //Ecriture de tour avec un espace avant pour separer les chaines

        //copy playeringame
        fprintf(fics, "\n%d %d %d %d", playeringame[0], playeringame[1], playeringame[2], playeringame[3]);

        //copy playeringame
        fprintf(fics, "\n%.2f %.2f %.2f %.2f", tempsjoueur[0], tempsjoueur[1], tempsjoueur[2], tempsjoueur[3]);

        fclose(fics);
    }

    return result;
}

piece* InitialiseBlocks()
{
    /* cette fonction initialise le tableau contenant les pieces et leurs attributs
    c'est ici que la mémoire est allouée dynamiquement et qu'on enregistre la forme des pieces */

    unsigned short i;
    unsigned short j;
    unsigned short k;
    piece* allpieces = NULL;

    do
        allpieces = (piece*)malloc(21*sizeof(piece));
    while(allpieces == NULL);

    /********INITIALISATION TABLEAU DE PIECES***********/
    for (i = 0; i < 21; i++)
    {
        do
            allpieces[i].forme = (char**)malloc(5 * sizeof(char*));
        while (allpieces[i].forme == NULL);
        for (k = 0; k < 5; k++)
            do
                allpieces[i].forme[k] = (char*)malloc(5 * sizeof(char));
            while (allpieces[i].forme[k] == NULL);
        allpieces[i].used[0] = 0;
        allpieces[i].used[1] = 0;
        allpieces[i].used[2] = 0;
        allpieces[i].used[3] = 0;
        for (j = 0; j < 5; j++)
            for (k = 0; k < 5; k++)
                allpieces[i].forme[j][k] = '0';
    }
    /**************************************************/

    //creation pieces
    allpieces[0].forme[0][0] = 'a';

    allpieces[1].forme[0][0] = 'b';
    allpieces[1].forme[0][1] = 'b';

    allpieces[2].forme[0][0] = 'c';
    allpieces[2].forme[0][1] = 'c';
    allpieces[2].forme[0][2] = 'c';

    allpieces[3].forme[0][0] = 'd';
    allpieces[3].forme[0][1] = 'd';
    allpieces[3].forme[1][0] = 'd';

    allpieces[4].forme[0][0] = 'e';
    allpieces[4].forme[0][1] = 'e';
    allpieces[4].forme[0][2] = 'e';
    allpieces[4].forme[0][3] = 'e';

    allpieces[5].forme[0][0] = 'f';
    allpieces[5].forme[1][0] = 'f';
    allpieces[5].forme[2][0] = 'f';
    allpieces[5].forme[3][0] = 'f';
    allpieces[5].forme[4][0] = 'f';

    allpieces[6].forme[0][0] = 'g';
    allpieces[6].forme[1][0] = 'g';
    allpieces[6].forme[2][0] = 'g';
    allpieces[6].forme[2][1] = 'g';

    allpieces[7].forme[0][0] = 'h';
    allpieces[7].forme[1][0] = 'h';
    allpieces[7].forme[2][0] = 'h';
    allpieces[7].forme[1][1] = 'h';

    allpieces[8].forme[0][0] = 'i';
    allpieces[8].forme[1][0] = 'i';
    allpieces[8].forme[1][1] = 'i';
    allpieces[8].forme[0][1] = 'i';

    allpieces[9].forme[0][0] = 'j';
    allpieces[9].forme[0][1] = 'j';
    allpieces[9].forme[1][1] = 'j';
    allpieces[9].forme[1][2] = 'j';

    allpieces[10].forme[0][0] = 'k';
    allpieces[10].forme[1][0] = 'k';
    allpieces[10].forme[1][1] = 'k';
    allpieces[10].forme[1][2] = 'k';
    allpieces[10].forme[2][1] = 'k';

    allpieces[11].forme[0][0] = 'l';
    allpieces[11].forme[1][0] = 'l';
    allpieces[11].forme[2][0] = 'l';
    allpieces[11].forme[3][0] = 'l';
    allpieces[11].forme[3][1] = 'l';

    allpieces[12].forme[0][1] = 'm';
    allpieces[12].forme[1][1] = 'm';
    allpieces[12].forme[2][1] = 'm';
    allpieces[12].forme[3][0] = 'm';
    allpieces[12].forme[2][0] = 'm';

    allpieces[13].forme[0][1] = 'n';
    allpieces[13].forme[1][1] = 'n';
    allpieces[13].forme[2][1] = 'n';
    allpieces[13].forme[2][0] = 'n';
    allpieces[13].forme[1][0] = 'n';

    allpieces[14].forme[0][0] = 'o';
    allpieces[14].forme[0][1] = 'o';
    allpieces[14].forme[1][1] = 'o';
    allpieces[14].forme[2][1] = 'o';
    allpieces[14].forme[2][0] = 'o';


    allpieces[15].forme[0][0] = 'p';
    allpieces[15].forme[1][0] = 'p';
    allpieces[15].forme[1][1] = 'p';
    allpieces[15].forme[1][2] = 'p';
    allpieces[15].forme[2][2] = 'p';

    allpieces[16].forme[0][0] = 'q';
    allpieces[16].forme[1][0] = 'q';
    allpieces[16].forme[2][0] = 'q';
    allpieces[16].forme[3][0] = 'q';
    allpieces[16].forme[1][1] = 'q';

    allpieces[17].forme[0][1] = 'r';
    allpieces[17].forme[1][1] = 'r';
    allpieces[17].forme[2][1] = 'r';
    allpieces[17].forme[2][0] = 'r';
    allpieces[17].forme[2][2] = 'r';

    allpieces[18].forme[0][0] = 's';
    allpieces[18].forme[1][0] = 's';
    allpieces[18].forme[2][0] = 's';
    allpieces[18].forme[2][1] = 's';
    allpieces[18].forme[2][2] = 's';

    allpieces[19].forme[0][0] = 't';
    allpieces[19].forme[0][1] = 't';
    allpieces[19].forme[1][1] = 't';
    allpieces[19].forme[1][2] = 't';
    allpieces[19].forme[2][2] = 't';

    allpieces[20].forme[0][1] = 'u';
    allpieces[20].forme[1][0] = 'u';
    allpieces[20].forme[1][1] = 'u';
    allpieces[20].forme[1][2] = 'u';
    allpieces[20].forme[2][1] = 'u';

    return allpieces;
}

void SoloGame()
{
    int i;
    int j;
    int maxplayer;
    unsigned short selectedblockindex;
    unsigned short loosers = 0;
    unsigned short moves;
    piece* pieces = NULL;
    char** terrain = NULL;
    char** placeOccupee = NULL;
    short playeringame[4] = {0,0,0,0};
    unsigned char playerstype[4] = {1,0,0,0};

    terrain = Dynamic2020Matrix(terrain, '0');
    placeOccupee = Dynamic2020Matrix(placeOccupee, '0');
    pieces = InitialiseBlocks();

    system("cls");
    printf("1 - 1 Bot\n");
    printf("2 - 2 Bots\n");
    printf("3 - 3 Bots\n");
    while (!scanf("%d", &maxplayer) || maxplayer < 0 || maxplayer > 3)
    {
        fflush(stdin);
        scanf("%d", &maxplayer);
    }
    for(i = 1; i<maxplayer+1; i++)      //on définir le nombre de joueurs IA en plus de l'utilisateur
        playerstype[i] = 2;

    i = 1;
    j = 0;
    DisplayGrid();  //affichage des bordures du jeu
    while (playeringame[i-1] != 1)  //tant qu'il n'y a pas de vainqueur
    {
        gotoligcol(23, 0);
        printf("Tour: %d", j + 1);
        if(playeringame[i-1] != -1) //si le joueur n'est pas déclaré comme perdant
        {
            DisplayAvailableBlocks(pieces, i);
            if(j>0) //on controle si le joueur a gagné ou perdu a partir du 2e tour
                playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
            if(playeringame[i-1] == 0)  //si le joueur est toujours en jeu
            {
                // on controle si un utilisateur ou l'ordinateur joue (et on donne la main a l'un ou a l'autre en conséquence)
                if(playerstype[i-1] == 1)
                    terrain = PlayerMove(pieces, i, playerstype[i-1], j, terrain, &moves);
                else if(playerstype[i-1] == 2)
                    terrain = ComputerMove(pieces, placeOccupee, i, playerstype[i-1], j, terrain, &selectedblockindex, &moves);
            }

            if(playeringame[i-1] == -1) // si un joueur a perdu
            {
                gotoligcol(25,0);
                printf("PLAYER %d OUT",i);
                Sleep(1000);

                for(i=0; i<=maxplayer; i++) // on controle le nombre de joueur toujours en jeu
                    loosers -= playeringame[i];
                if(loosers == maxplayer)    //si il ne reste qu'un joueur il est déclaré vainqueur
                {
                    for(i=0; i<=maxplayer; i++)
                        if(playeringame[i] == 0)
                            playeringame[i] = 1;
                }
                else
                    loosers = 0; // sinon on reset le compteur

            }
            DisplayTerrain(terrain);
        }
        i++;    //incémentation du joueur actf
        if (i > maxplayer + 1)  //si on dépasse le nombre max de joueurs
        {
            j++;    //on incrémente le tour
            i = 1;  // on replace le joueur à 1
        }
    }
    gotoligcol(24, 0);
    printf("LE JOUEUR %d A GAGNE\nPress any key to continue", i);
}

void MultiplayerGame()
{
    // partie multijoueur
    int i;
    int j;
    int maxplayer;
    float temps_tour;
    unsigned short selectedblockindex;
    unsigned short loosers = 0;
    unsigned short moves;
    piece* pieces = NULL;
    char** terrain = NULL;
    char** placeOccupee = NULL;
    short playeringame[4] = {0,0,0,0};  //état du joueur: gagné/perdu/en jeu
    clock_t temps_init;
    clock_t temps_end;
    float tempsjoueur[4] = {0,0,0,0};   // temps de jeu

    srand(time(NULL));

    terrain = Dynamic2020Matrix(terrain, '0');
    placeOccupee = Dynamic2020Matrix(placeOccupee, '0');
    pieces = InitialiseBlocks();

    system("cls");
    printf("1 - 2 Joueurs\n");
    printf("2 - 3 Joueurs\n");
    printf("3 - 4 Joueurs\n");
    while (!scanf("%d", &maxplayer) || maxplayer < 0 || maxplayer > 3)
    {
        fflush(stdin);
        scanf("%d", &maxplayer);
    }
    DisplayGrid();
    gotoligcol(24,0);
    printf("Temps:");
    for(i=0; i<maxplayer+1; i++)
    {
        gotoligcol(25+i,0);
        printf("J%d = 0",i+1);
    }

    i = 1;
    j = 0;
    while (playeringame[i-1] != 1)  //tant qu'aucun joueur n'a gagné
    {
        gotoligcol(23, 0);
        printf("Tour: %d", j + 1);
        if(playeringame[i-1] != -1) // si le joueur n'a pas perdu
        {
            DisplayAvailableBlocks(pieces, i);  //affichage des pieces dispo
            if(j > 1)   // on controle si un joueur a gagné ou perdu a partir du 2e tour
                playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
            if(!playeringame[i-1])  // tant si le joueur est toujours en jeu
            {
                temps_init = clock(); //temps général au début du tour
                terrain = PlayerMove(pieces, i, 1, j, terrain, &moves); // action du joueur
                temps_end = clock();
                temps_tour = temps_end - temps_init; //Calcul du temps du tour
                temps_tour/=1000.0;
                tempsjoueur[i-1] += temps_tour;     //calcul et affichage du temps de jeu total
                gotoligcol(24+i,0);
                printf("J%d = %.2f",i, tempsjoueur[i-1]);

            }
            if(playeringame[i-1] == -1) //si le joueur a perdu
            {
                gotoligcol(24+i,11);
                printf("PLAYER %d OUT",i);
                Sleep(1000);

                for(i=0; i<=maxplayer; i++) //  on compte le nombre de joueur encore en jeu
                    loosers -= playeringame[i];
                if(loosers == maxplayer)    // si il ne reste qu'un joueur
                {
                    for(i=0; i<=maxplayer; i++)
                        if(playeringame[i] == 0)
                            playeringame[i] = 1;    //celui ci est déclaré vainqueur
                }
                else
                    loosers = 0;    //sinon on remet le compteur a 0

            }
            DisplayTerrain(terrain);    //affichage du terrain
        }
        i++;    //incrémentation du n° du joueur actif
        if (i > maxplayer + 1)  // si on dépace le nombre de joueur de la partie
        {
            j++;    //on incrémente le numéro du tour
            i = 1;  //on reset le numéro de joueur
        }
        Sauvegarder(terrain, pieces, j, i, maxplayer, loosers, playeringame, tempsjoueur);  //on sauvegarde la partie

    }
    gotoligcol(24, 0);
    printf("LE JOUEUR %d A GAGNE\nPress any key to continue", i);
}

unsigned char RestartGame()
{
    // jeu lancé depuis une sauvegarde
    int i;
    int j;
    unsigned short a;
    unsigned short b;
    int maxplayer;
    float temps_tour;
    unsigned short selectedblockindex;
    unsigned short loosers = 0;
    unsigned short moves;
    piece* pieces = NULL;
    char** terrain = NULL;
    char** placeOccupee = NULL;
    short playeringame[4] = {0,0,0,0};
    clock_t temps_init;
    clock_t temps_end;
    float tempsjoueur[4] = {0,0,0,0};
    FILE* fics = NULL;
    unsigned char result = 0;
    char scaned;
    float scan;

    fics = fopen("lastgame.txt", "r");  //ouverture du fichier


    if(fics != NULL)
    {
        result = 1;
        terrain = Dynamic2020Matrix(terrain, '0');
        placeOccupee = Dynamic2020Matrix(placeOccupee, '0');
        pieces = InitialiseBlocks();

        for(a = 0; a < 20 ; a++)     //recuperation matrice de jeu
        {
            for(b = 0; b< 20; b++)
            {
                fscanf(fics, "%c", &scaned);
                if(scaned == 10)
                    fscanf(fics, "%c", &scaned);
                terrain[a][b] = scaned;
                printf("%c", terrain[a][b]);
            }
            printf("\n");
        }

        fscanf(fics, "%c", &scaned);


        for(a = 0; a < 21 ; a++)    //recuperation des pieces (utilisation)
        {
            for(b = 0; b < 4; b++)
            {
                fscanf(fics, "%d", &scaned);
                pieces[a].used[b] = scaned;
            }
            fscanf(fics, "%c", &scaned);    //pour ne pas prendre le retour a la ligne
        }

        fscanf(fics, "%d", &scaned);    //recuperation tour
        j = scaned;
        fscanf(fics, "%d", &scaned);    //recuperation joueurs
        i = scaned;
        fscanf(fics, "%d", &scaned);    //recuperation maxplayer
        maxplayer = scaned;
        fscanf(fics, "%d", &scaned);    //recuperation loosers
        loosers = scaned;

        for(a = 0; a < 4 ; a++)    //recuperation de l'etat des joueurs (gagne/perdu/en jeu)
        {
            fscanf(fics, "%d", &scaned);
            playeringame[a] = scaned;

        }

        for(a = 0; a < 4 ; a++)    //recuperation des temps
        {
            fscanf(fics, "%f", &scan);
            tempsjoueur[a] = scan;
        }

        DisplayGrid();
        gotoligcol(24,0);
        printf("Temps:");
        for(a=0; a<maxplayer+1; a++)
        {
            gotoligcol(25+a,0);
            printf("J%d = %.2f",i+1, tempsjoueur[a]);
        }

        //MULTIPLAYER GAME
        DisplayTerrain(terrain);
        while (playeringame[i-1] != 1)
        {
            gotoligcol(23, 0);
            printf("Tour: %d", j + 1);
            if(playeringame[i-1] != -1)
            {
                DisplayAvailableBlocks(pieces, i);
                if(j > 1)
                    playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
                if(!playeringame[i-1])
                {
                    temps_init = clock(); //temps général au début du tour
                    terrain = PlayerMove(pieces, i, 1, j, terrain, &moves);
                    temps_end = clock();
                    temps_tour = temps_end - temps_init; //Calcul du temps du tour
                    temps_tour/=1000.0;
                    tempsjoueur[i-1] += temps_tour;
                    gotoligcol(24+i,0);
                    printf("J%d = %.2f",i, tempsjoueur[i-1]);

                }
                if(playeringame[i-1] == -1)
                {
                    gotoligcol(25,0);
                    printf("PLAYER %d OUT",i);
                    Sleep(1000);

                    for(i=0; i<=maxplayer; i++)
                        loosers -= playeringame[i];
                    if(loosers == maxplayer)
                    {
                        for(i=0; i<=maxplayer; i++)
                            if(playeringame[i] == 0)
                                playeringame[i] = 1;
                    }
                    else
                        loosers = 0;

                }
                DisplayTerrain(terrain);
            }
            i++;
            if (i > maxplayer + 1)
            {
                j++;
                i = 1;
            }
            Sauvegarder(terrain, pieces, j, i, maxplayer, loosers, playeringame, tempsjoueur);

        }
        gotoligcol(24, 0);
        printf("LE JOUEUR %d A GAGNE\nPress any key to continue", i);
    }
    else
    {
        printf("Il n'existe aucune sauvegarde de jeu");
    }


    return result;
}


char Menu()
{
    char choix = '0';

    printf("\n***** BLOKUS *****\n\n");
    printf("1 - 1 Joueur\n");
    printf("2 - Multiplayer\n");
    printf("3 - Reprendre la derniere partie\n");
    printf("4 - Regles\n");
    printf("5 - Quitter\n");

    while (choix != '5')
    {
        if (kbhit())
        {
            choix = getch();

            switch (choix)
            {
            case '1':
                SoloGame();
                Menu();
                break;


            case'2':
                MultiplayerGame();
                Menu();
                break;

            case '3':
                RestartGame();
                Menu();
                break;

            case '4':
                system("cls");
                regles();
                system("cls");
                Menu();
                break;
            case '5':
                break;

                system("cls");
            }
        }
    }
    return choix;

}



int main()
{
    srand(time(NULL));

    Menu();
    return 0;
}
