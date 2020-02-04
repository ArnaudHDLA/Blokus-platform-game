#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "headers.h"


typedef struct piece //structure utilisee pour definir les pieces du jeu
{
    char ** forme; //matrice dynamique (5x5) contenant la forme de la piece
    unsigned char used[4]; //tableau indiquant si la piece a ete utilisee par un joueur
} piece;


char ** Dynamic2020Matrix(char ** array, char value) //methode d'initialisation d'une matrice dynamique 20x20 remplie par la valeur value
{
    int i;
    int j;

    do
        array = (char**)malloc(20 * sizeof(char*)); //allocation de la 1e dimension
    while (array == NULL);

    for (i = 0; i < 20; i++)
        do
            array[i] = (char*)malloc(20 * sizeof(char)); //allocation de la 2e dimension
        while (array[i] == NULL);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
            array[i][j] = value; // initialisation des cases a 'value'

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

char ** RotateCW(char** mat) // pivotement sans le sens horaire d'une piece
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

char ** RotateCCW(char** mat)   // pivotement sans le sens anti-horaire d'une piece
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

unsigned char MoveUp(char ** placeOccupee) //deplacement vers le haut d'une piece
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
                placeOccupee[i - 1][j] = placeOccupee[i][j]; //décale toutes les lignes vers le haut
        for (i = 0; i < 20; i++)
            placeOccupee[19][i] = '0'; //la ligne la plus basse est donc forcement vide
    }

    return ok; // informe si la piece a pu etre deplacee
}

unsigned char MoveLeft(char ** placeOccupee) //deplacement vers la gauche d'une piece
{
    unsigned char ok = 1;
    unsigned short i;
    unsigned short j;
    for (i = 0; i < 20; i++)     // verifie que la piece n'est pas deja au bord gauche de la matrice de jeu
        if (placeOccupee[i][0] != '0')
            ok = 0;

    if (ok) // si on peut déplacer
        for (j = 0; j < 20; j++)
            for (i = 1; i < 20; i++)
                placeOccupee[j][i - 1] = placeOccupee[j][i]; //décale toutes les lignes vers la gauche
    for (i = 0; i < 20; i++)
        placeOccupee[i][19] = '0'; //la ligne la plus a droite est donc forcement vide

    return ok; // informe si la piece a pu etre deplacee
}

unsigned char MoveRight(char ** placeOccupee) //deplacement vers la droite d'une piece
{
    unsigned char ok = 1;
    short i;
    short j;

    // verifie que la piece n'est pas deja au bord droit de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[i][19] != '0')
            ok = 0;
    if (ok) // si on peut déplacer
    {
        for (j = 18; j >= 0; j--)
            for (i = 0; i < 20; i++)
                placeOccupee[i][j + 1] = placeOccupee[i][j]; //décale toutes les lignes vers la droite

        for (i = 0; i < 20; i++)
            placeOccupee[i][0] = '0'; //la ligne la plus a gauche est donc forcement vide
    }
    return ok; // informe si la piece a pu etre deplacee
}

unsigned char MoveDown(char ** placeOccupee) //deplacement vers le bas d'une piece
{
    unsigned char ok = 1;
    short i;
    short j;

    // verifie que la piece n'est pas deja au bord inferieur de la matrice de jeu
    for (i = 0; i < 20; i++)
        if (placeOccupee[19][i] != '0')
            ok = 0;
    if (ok) // si on peut déplacer
    {
        for (i = 18; i >= 0; i--)
            for (j = 0; j < 20; j++)
                placeOccupee[i + 1][j] = placeOccupee[i][j]; //décale toutes les lignes vers le bas

        for (i = 0; i < 20; i++)
            placeOccupee[0][i] = '0'; //la ligne la plus haute est donc forcement vide
    }
    return ok; // informe si la piece a pu etre deplacee
}


void DisplayTempMatrix(char ** matrix, unsigned short joueur, char** placeOccupee, BITMAP* image)  //ok
{
    /* affichage de la matrice temporaire qui permet de placer
    une piece sans affecter la matrice de jeu */
    int col_jou=makecol(0,0,25);
    int gt=9; //goto
    int bg1x=30;
    int bg1y=120; //begin
    int i;
    int j;
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (placeOccupee[i][j] != '0')
                textprintf_ex(image, font, bg1x+gt*2+j*gt, bg1y+gt*2+i*gt, makecol(255,255,255), makecol(0,0,0), " "); // on affiche la piece choisie en blanc pour montrer qu'elle n'est pas posee pour l'instant
            else
            {
                switch (matrix[i][j]) // sinon on affiche les pieces de la matrice de jeu dans les couleurs des joueurs
                {
                // si la case n'est pas libre
                case '1':
                    col_jou=makecol(0,0,200); // bleu
                    break;
                case '2':
                    col_jou=makecol(0,255,0); // vert
                    break;
                case '3':
                    col_jou=makecol(255,0,0); //rouge
                    break;
                case '4':
                    col_jou=makecol(255,255,0); //jaune
                    break;

                }

                if(matrix[i][j] == '0') //sinon on affiche un espace
                    textprintf_ex(image, font, bg1x+gt*2+j*gt, bg1y+gt*2+i*gt, makecol(255,255,255), makecol(200,200,250), " ");
                else
                    textprintf_ex(image, font, bg1x+gt*2+j*gt, bg1y+gt*2+i*gt, makecol(255,255,255), col_jou, " ");
            }
        }
    }
    blit(image,screen,0,0,0,00,800,600);
}

void DisplayTerrain(char ** terrain, BITMAP* image) //ok
{
    /* afiche le terrain de jeu avec les pieces
    les pieces sont colorees en fonction des joueurs */
    int col_jou=makecol(0,0,25);
    int gt=9; //goto
    int bg1x=30;
    int bg1y=120; //begin

    int i;
    int j;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (terrain[i][j] != '0')
            {
                switch (terrain[i][j]) // en fonction du joueur qui occupe la case
                {
                case '1':
                    col_jou=makecol(0,0,200); //joueur 1: bleu
                    break;
                case '2':
                    col_jou=makecol(0,255,0); // joueur 2: vert
                    break;
                case '3':
                    col_jou=makecol(255,0,0); // joueur 3: rouge
                    break;
                case '4':
                    col_jou=makecol(255,255,0); // joueur 4: jaune
                    break;

                }
                textprintf_ex(image, font, bg1x+gt*2+j*gt, bg1y+gt*2+i*gt, makecol(255,255,255), col_jou, " ");

            }
        }
    }
    blit(image,screen,0,0,0,00,800,600);
}

void DisplayAvailableBlocks(piece* pieces, int joueur, BITMAP* image)
{
    /* affiche la liste des pieces encore disponibles
    pour un joueur, elles sont a la couleur du joueur
    et ont leur identification imprimee dessus */
    int col_jou;
    int gt=9; //goto
    int bg2x=285;
    int bg2y=130;
    unsigned short i;
    unsigned short j;
    unsigned short k;
    unsigned short placedblocks = 0;
    unsigned short offset = 2; //pour le décalage de la matrice

    for (i = 0; i < 15; i++) //boucle permettant d'effacer les pieces du joueur précedent
        for (j = 0; j < 49; j++)
            textprintf_ex(image, font, bg2x+gt+j*gt, bg2y+gt+i*gt, makecol(255,0,0), makecol(200,200,250), " ");


    for (i = 0; i < 21; i++) // on verifie toutes les pieces
    {
        if (pieces[i].used[joueur - 1] == 0) // si la piece n'a pas ete utilisee
        {
            for (j = 0; j < 5; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    if (pieces[i].forme[j][k] != '0') // on affiche la piece a la couleur du joueur
                    {
                        switch (joueur)
                        {
                        case 1:
                            col_jou=makecol(0,0,200);
                            break;
                        case 2:
                            col_jou=makecol(0,255,0);
                            break;
                        case 3:
                            col_jou=makecol(255,0,0);
                            break;
                        case 4:
                            col_jou=makecol(255,255,0);
                            break;

                        }
                        textprintf_ex(image, font, bg2x+gt + (placedblocks % 10 * 5)*gt + k*gt, bg2y+gt-2*gt+offset*gt + j*gt, makecol(255,255,255), col_jou, "%c",pieces[i].forme[j][k]);
                    }
                }
            }
            placedblocks++; // on indique qu'un bloc a ete place pour pouvoir afficher le suivant a un endroit different
            if (placedblocks == 10 || placedblocks == 20) // si on a affiche 10 ou 20 pieces, on est au bord du tableau, il faut descendre d'une ligne
            {
                if (placedblocks == 10)
                    offset += 6;
                else
                    offset += 4;
            }
        }
    }
    blit(image,screen,0,0,0,00,800,600);
}

void DisplayManipulationMatrix(char ** mat, BITMAP* image)
{
    // affiche le contenu de la grille de manipulation d'une piece
    int gt=9; //goto
    int bg3x=360;
    int bg3y=330;

    unsigned short i;
    unsigned short j;

    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            if (mat[i][j] != '0')
                textprintf_ex(image, font, bg3x+gt+j*gt, bg3y+gt+i*gt, makecol(255,255,255), makecol(0,0,0), " ");
            else
                textprintf_ex(image, font, bg3x+gt+j*gt, bg3y+gt+i*gt, makecol(255,255,255), makecol(200,200,250), " ");
        }


    blit(image,screen,0,0,0,00,800,600);
}

char ** BlockSelection(piece* pieces, int joueur, unsigned char playertype, unsigned short* sbindex, BITMAP* image)
{
    // fonction de placement d'une piece par un utilisateur ou pas l'ordinateur
    int gt=9; //goto
    int bg3x=360;
    int bg3y=330;

    int i;
    int j;
    char selected = '&';
    unsigned char ok = 0;

    textprintf_ex(image, font, 440, 330, makecol(255,0,0), makecol(0,0,0), "Select a block:");
    blit(image,screen,0,0,0,00,800,600);

    if(playertype == 2) // si c'est un ordinateur qui a joue
    {
        rest(1000);
        selected = *sbindex; // la piece selectionnee a ete determinee par la methode CheckPlacableBlock et son indice stocke dans sbindex
        pieces[selected].used[joueur - 1] = 1;// on informe que la piece est utilisee
        DisplayManipulationMatrix(pieces[selected].forme, image);// on l'affiche dans la matrice de manipulation
        selected+=65;   //pour pouvoir garder le return a la fin et unique
        rest(1000); // delai d'attente pour que le(s) utilisateur(s) puissent voir ce que l'ordinateur fait
    }
    else // si un utiisateur joue
    {
        do
        {
            fflush(stdin);
            selected = readkey();
            textprintf_ex(image, font, 560, 330, makecol(255,0,0), makecol(0,0,0), "%c", selected);
            blit(image,screen,0,0,0,00,800,600);
            if (selected > 'U' || selected < 'A') // si la touche a ete tapee en majuscule on la reduit en miuscule
                selected -= 32;
            if (pieces[selected - 65].used[joueur - 1] == 0) //flag permettant de bloquer si la piece est deja prise
                ok = 1;
        }
        while (((selected < 'A' || selected > 'U') && (selected < 'a' || selected > 'u')) || ok == 0);
        if (selected > 'U' || selected < 'A')
            selected -= 32;

        *sbindex = selected-65; // on enregistre la piece selectionnee
        pieces[selected - 65].used[joueur - 1] = 1; //et on la declare comme utilisee par le joueur

        for (i = 0; i < 5; i++) // on affiche ensuite la piece dans la matrice de manipulation
            for (j = 0; j < 5; j++)
                if (pieces[selected - 65].forme[i][j] != '0')
                    textprintf_ex(image, font, bg3x+gt+j*gt, bg3y+gt+i*gt, makecol(255,255,255), makecol(0,0,0), " ");
    }


    // affichage du block selectionné dans la matrice de manipulation
    textprintf_ex(image, font, 440, 330+gt, makecol(255,0,0), makecol(0,0,0), "Rotation/Inversion (touches G-D-I): ");
    blit(image,screen,0,0,0,00,800,600);

    return pieces[selected - 65].forme; //retourne la forme de la piece choisie
}
//ok

char ** Manipulation(char ** mat, unsigned char playertype, unsigned short* moves, BITMAP* image)
{
    // fonction permettant de manipuler la piece choisie dans BlockSelection()
    int gt=9; //goto
    int bg3x=360;
    int bg3y=330;

    char manip;
    int i;
    int j;

    if(playertype == 1) // si c'est le tour d'un utilisateur on le laisse choisir les manipulations
    {
        manip = ' ';
        while (manip != '\r') //tant que l'utilisateur ne valide pas son choix par la touche ENTER
        {
            if (keypressed())  //si une touche est pressee
            {
                manip = readkey(); //on enregistre la touche pressee
                switch (manip)
                {
                case 'g':
                case 'G':
                    mat = RotateCCW(mat); //G pour Gauche donc rotation anti-horaire
                    break;

                case 'd':
                case 'D':
                    mat = RotateCW(mat); //D pour droite donc rotation horaire
                    break;

                case 'i':
                case 'I':
                    mat = HorizontalFlip(mat); //I pour inversion
                    break;
                }
                DisplayManipulationMatrix(mat, image); //affichage de la piece apres manipulation
            }
            blit(image,screen,0,0,0,00,800,600);
        }
    }
    else if(playertype == 2) // si c'est l'ordinateur qui joue
    {
        for(i = 0; i< *moves+1; i++)  //on reprend l'algo de la fonction EndGame() et on reproduit les manipulations qu'elle a ait pour arriver a un block compatible
        {
            if(i != 4)
            {
            //qu'elle arrive a la bonne forme
            // le nombre de manipulations a ete enregistre dans la variable moves
                mat = RotateCW(mat);
                DisplayManipulationMatrix(mat, image);
                rest(1000);
            }
            else
            {
                mat = HorizontalFlip(mat);
                DisplayManipulationMatrix(mat, image);
                rest(1000);

                mat = RotateCW(mat);
                DisplayManipulationMatrix(mat, image);
                rest(1000);
            }
            blit(image,screen,0,0,0,00,800,600);
        }
    }
    //vider matrice de manipulation
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            textprintf_ex(image, font, bg3x+gt+j*gt, bg3y+gt+i*gt, makecol(255,255,255), makecol(200,200,250), " ");

    blit(image,screen,0,0,0,00,800,600);
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

    for (i = 0; i < 20; i++) //compte le nombre de blocks qui composent la piece
        for (j = 0; j < 20; j++)
            if (placeOccupee[i][j] != '0')
                compteurMorceau += 1;

    for (i = 0; i < 20; i++) //on parcours toute la matrice
        for (j = 0; j < 20; j++)
            // controle que la piece n'en touche pas une autre du meme joueur sur ses bords
            // et qu'elle touche au moins une autre piece du meme joueur sur l'angle
            if(placeOccupee[i][j] == joueur + '0' && placeOccupeeFinal[i][j] == '0')
            {
                if(i == 0 || j == 0 || i == 19 || j == 19) // si une piece touche un des bords
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

char ** PlaceBlock(char ** selectedblock, unsigned char joueur, unsigned char playertype, int tour, char** placeOccupeeFinal, piece* pieces, unsigned short* sbindex, unsigned short* moves, BITMAP* image)
{
    // fonction permettant au joueur de placer sa piece sur le plateau
    int gt=9; //goto

    char manip = ' ';
    int i;
    int j;
    char ** placeOccupee = NULL;
    unsigned char validposition = 0;

    placeOccupee = Dynamic2020Matrix(placeOccupee, '0'); //initialisation de la matrice temporaire

    // placer la piece dans la matrice temporaire
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (selectedblock[i][j] != '0')
                placeOccupee[i+7][j+7] = joueur + '0'; //on place la piece choisie au milieu du plateau (dans un but de jouabilité)

    textprintf_ex(image, font, 440, 330+gt+gt, makecol(255,0,0), makecol(0,0,0), "Placez la pièce sur le plateau (Z-Q-S-D)");
    blit(image,screen,0,0,0,00,800,600);


    //afficher la matrice de jeu puis la matrice temp
    // offset de 2 a cause des bordures
    DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee, image); //on affiche la piece sur le plateau
    manip = ' ';
    while (!validposition) //tant que la position n'est pas valide
    {
        manip = ' ';
        do
        {
            if (keypressed())
            {
                manip = readkey();

                switch (manip) //en fonction de la demande de l'utilisateur on va deplacer la piece dans toutes les directions
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
                    textprintf_ex(image, font, 440, 330+gt*3, makecol(255,0,0), makecol(0,0,0), "Vous pouvez placer la piece ici !");
                    textprintf_ex(image, font, 440, 330+gt*4, makecol(255,0,0), makecol(0,0,0), "Appuyez sur 'h' pour une sugestion de place");
                    textprintf_ex(image, font, 440, 330+gt*5, makecol(255,0,0), makecol(0,0,0), "Appuyez sur 'x' pour changer de pièce");
                    blit(image,screen,0,0,0,00,800,600);
                }

                else
                {
                    if(tour !=0)
                    {
                        textprintf_ex(image, font, 440, 330+gt*3, makecol(255,0,0), makecol(0,0,0), "Impossible de placer la piece ici !");
                        textprintf_ex(image, font, 440, 330+gt*4, makecol(255,0,0), makecol(0,0,0), "Appuyez sur 'h' pour une sugestion de place");
                        textprintf_ex(image, font, 440, 330+gt*5, makecol(255,0,0), makecol(0,0,0), "Appuyez sur 'x' pour changer de pièce");

                        blit(image,screen,0,0,0,00,800,600);
                        validposition = 0;
                    }
                }
                DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee, image);
            }

        }
        while (manip != '\r' && manip != 'X' && manip != 'x' && manip != 'h' && manip !='H');
        //on poursuit tant que le joueur ne valide pas son choix
        // ou qu'il ne demande pas de conseil (H) ou qu'il ne change pas de piece
        if(manip == 'X' || manip == 'x' || ((manip == 'h' || manip == 'H')&& tour != 0))
            validposition = 1;         // s'il choisit de changer de piece ou qu'il veut un conseil on passe validposition a 1 pour permettre l'echappement de la boucle while
    }

    if(manip == '\r') // si le joueur valide son placement
    {
        for (i = 0; i < 20; i++)	//place block
            for (j = 0; j < 20; j++)
                if (placeOccupee[i][j] != '0')
                    placeOccupeeFinal[i][j] = placeOccupee[i][j]; // on inscrit la piece dans le plateau

        for (i = 0; i < 20; i++)
            for (j = 0; j < 20; j++)
                placeOccupee[i][j] = '0'; //et on vide la matrice temporaire

        for (i = 0; i < 6; i++) //on efface les champs qui guident l'utilisateur
            textprintf_ex(image, font, 440, 330+(i*gt), makecol(255,0,0), makecol(0,0,0), "                                           ");
        blit(image,screen,0,0,0,00,800,600);
    }
    else
    {
        if(manip == 'H' || manip == 'h') // si le joueur a besoin d'une suggestion de placement
        {
            for (i = 0; i < 20; i++)	//place block
                for (j = 0; j < 20; j++)
                    placeOccupee[i][j] = '0'; //on vide la matrice temporaire

            CheckPlacableBlock(pieces, joueur, placeOccupee, placeOccupeeFinal, sbindex, moves,0); //on verifie où une piece peut etre posée (enregistré dans placeOccupee)
            DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee, image); // et on affiche la suggestion
            for (i = 0; i < 6; i++)
                textprintf_ex(image, font, 440, 330+(i*gt), makecol(255,0,0), makecol(0,0,0), "                                           ");
            blit(image,screen,0,0,0,00,800,600);
            textprintf_ex(image, font, 440, 330, makecol(255,0,0), makecol(0,0,0), "PAUSED");
            while(!keypressed()) {}
        }
        for (i = 0; i < 6; i++)
            textprintf_ex(image, font, 440, 330+(i*gt), makecol(255,0,0), makecol(0,0,0), "                                           ");
        blit(image,screen,0,0,0,00,800,600);
        pieces[*sbindex].used[joueur-1] = 0; //on declare la piece comme disponible
        DisplayAvailableBlocks(pieces, joueur, image); //on réaffiche alors les pieces disponibles
        selectedblock = BlockSelection(pieces, joueur, playertype, sbindex, image);  //et on demande a l'utilisateur son nouveau choix
        selectedblock = Manipulation(selectedblock, playertype, moves, image);
        placeOccupeeFinal = PlaceBlock(selectedblock, joueur, playertype, tour, placeOccupeeFinal, pieces, sbindex, moves, image);
    }
    return placeOccupeeFinal; //on renvoie alors le terrain actualisé
}

char ** ComputerPlaceBlock(char ** placeOccupee, unsigned char joueur, int tour, char** placeOccupeeFinal, BITMAP* image)
{
    // fonction qui place la piece lorqu'un ordinateur joue
    int i;
    int j;

    if(tour == 0) // au 1er tour on place simplement la piece dans un angle
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

        //afficher la matrice de jeu puis la matrice temp
        // offset de 2 a cause des bordures
        DisplayTempMatrix(placeOccupeeFinal, joueur, placeOccupee, image);

        for (i = 0; i < 20; i++)	//place block
            for (j = 0; j < 20; j++)
                if (placeOccupee[i][j] != '0')
                    placeOccupeeFinal[i][j] = placeOccupee[i][j];

        for (i = 0; i < 20; i++)
            for (j = 0; j < 20; j++)
                placeOccupee[i][j] = '0';
    }
    return placeOccupeeFinal;
}


void DisplayGrid(BITMAP* image)
{
    // fonction qui permet l'affichage des bordures du jeu
    int gt=9; //goto
    int cf=makecol(100,255,100); //angle
    int cg=makecol(100,0,100);
    int bg1x=30;
    int bg1y=120; //begin
    int bg2x=285;
    int bg2y=130;
    int bg3x=360;
    int bg3y=330;
    int i;

    rectfill(image, bg1x+gt, bg1y+gt, bg1x+gt+196, bg1y+gt+196,makecol(0,0,0));
    rectfill(image, bg2x, bg2y, bg2x+457, bg2y+151,makecol(0,0,0));
    rectfill(image, bg3x, bg3y, bg3x+61, bg3y+61,makecol(0,0,0));


    for (i = 0; i < 20; i++)
        textprintf_ex(image, font, bg1x+gt*2+i*gt, bg1y, makecol(255,0,0), -1, "%c",'A' + i);

    for (i = 0; i < 20; i++)
        textprintf_ex(image, font, bg1x, bg1y+gt*2+i*gt, makecol(255,0,0), -1, "%c",'A' + i);

    for (i = 0; i < 21; i++)
        if (i == 0)
            textprintf_ex(image, font, bg1x+gt+i*gt, bg1y+gt, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg1x+gt+i*gt, bg1y+gt, makecol(255,0,0), cg, " ");
    textprintf_ex(image, font, bg1x+gt+21*gt, bg1y+gt, makecol(255,0,0), cf, " ");

    for (i = 0; i < 20; i++)
    {
        textprintf_ex(image, font, bg1x+gt, bg1y+gt+gt+i*gt, makecol(255,0,0), cg, " ");
        textprintf_ex(image, font, bg1x+22*gt, bg1y+gt+gt+i*gt, makecol(255,0,0), cg, " ");
    }

    /************************************/


    /***********AFFICHAGE MATRIX*****/
    for (i = 0; i < 21; i++)
        if (i == 0)
            textprintf_ex(image, font, bg1x+gt+i*gt, bg1y+gt*22, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg1x+gt+i*gt, bg1y+gt*22, makecol(255,0,0), cg, " ");
    textprintf_ex(image, font, bg1x+gt*22, bg1y+gt*22, makecol(255,0,0), cf, " ");


    for (i = 0; i < 50; i++)
        if (i == 0)
            textprintf_ex(image, font, bg2x+i*gt, bg2y, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg2x+i*gt, bg2y, makecol(255,0,0), cg, " ");

    textprintf_ex(image, font, bg2x+50*gt, bg2y, makecol(255,0,0), cf, " ");


    for (i = 0; i < 15; i++)
    {
        textprintf_ex(image, font, bg2x, bg2y+gt+i*gt, makecol(255,0,0), cg, " ");
        textprintf_ex(image, font, bg2x+50*gt, bg2y+gt+i*gt, makecol(255,0,0), cg, " ");
    }
    for (i = 0; i < 50; i++)
    {
        if (i == 0)
            textprintf_ex(image, font, bg2x+i*gt, bg2y+gt*16, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg2x+i*gt, bg2y+gt*16, makecol(255,0,0), cg, " ");
    }
    textprintf_ex(image, font, bg2x+gt*50, bg2y+gt*16, makecol(255,0,0), cf, " ");


    /************************************/


    /***********AFFICHAGE FLIP MATRIX*****/
    for (i = 0; i < 6; i++)
        if (i == 0)
            textprintf_ex(image, font, bg3x+i*gt, bg3y, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg3x+i*gt, bg3y, makecol(255,0,0), cg, " ");
    textprintf_ex(image, font, bg3x+6*gt, bg3y, makecol(255,0,0), cf, " ");

    for (i = 0; i < 5; i++)
    {
        textprintf_ex(image, font, bg3x, bg3y+gt+i*gt, makecol(255,0,0), cg, " ");
        textprintf_ex(image, font, bg3x+6*gt, bg3y+gt+i*gt, makecol(255,0,0), cg, " ");
    }

    for (i = 0; i < 6; i++)
        if (i == 0)
            textprintf_ex(image, font, bg3x+i*gt, bg3y+gt*6, makecol(255,0,0), cf, " ");
        else
            textprintf_ex(image, font, bg3x+i*gt, bg3y+gt*6, makecol(255,0,0), cg, " ");
    textprintf_ex(image, font, bg3x+gt*6, bg3y+gt*6, makecol(255,0,0), cf, " ");



    /***************************************/

    ///Rajout allegro init matrice transition
    int j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            textprintf_ex(image, font, bg3x+gt+j*gt, bg3y+gt+i*gt, makecol(255,255,255), makecol(200,200,250), " ");

    for (i = 0; i < 20; i++)
        for (j = 0; j < 20; j++)
            textprintf_ex(image, font, bg1x+gt*2+j*gt, bg1y+gt*2+i*gt, makecol(255,255,255), makecol(200,200,250), " ");

    for (i = 0; i < 15; i++)
        for (j = 0; j < 49; j++)
            textprintf_ex(image, font, bg2x+gt+j*gt, bg2y+gt+i*gt, makecol(255,0,0), makecol(200,200,250), " ");

    blit(image,screen,0,0,0,0,800,600);
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
        win = CheckPlacableBlock(pieces, joueur, placeOccupee, placeOccupeeFinal, sbindex, moves,1);

    return win;
// -1 if loose
// 0 if continue
// 1 if win
}

char ** PlayerMove(piece* allpieces,int i,unsigned char playerstype,int j,char ** terrain, unsigned short* moves, BITMAP* image)
{
    // cette fonction effectue la serie d'actions a faire pas l'utilisateur pour placer une piece
    // choisir la piece, la manipuler et la placer
    char ** result = NULL;
    char ** selectedblock = NULL;
    unsigned short selectedblockindex;

    selectedblock = BlockSelection(allpieces, i, playerstype, &selectedblockindex, image);
    selectedblock = Manipulation(selectedblock, playerstype, moves, image);
    result = PlaceBlock(selectedblock, i, playerstype, j, terrain, allpieces, &selectedblockindex, moves, image);
    return result;
}

char ** ComputerMove(piece* allpieces, char ** placeOccupee, int i, unsigned char playerstype, int j, char ** terrain, unsigned short * sbindex, unsigned short* moves, BITMAP* image)
{
    // placement d'une piece par l'ordinateur
    char ** result = NULL;
    char ** selectedblock = NULL;
    int gt=9; //goto
    int bg3x=360;
    int bg3y=330;

    int v;
    int w;

    if(j == 0)
        *sbindex = 0;
    selectedblock = BlockSelection(allpieces, i, playerstype, sbindex, image);
    if(j != 0)
        selectedblock = Manipulation(selectedblock, playerstype, moves, image);
    else
    {
        for (v = 0; v < 5; v++)
            for (w = 0; w < 5; w++)
                textprintf_ex(image, font, bg3x+gt+w*gt, bg3y+gt+v*gt, makecol(255,255,255), makecol(200,200,255), " ");

                blit(image,screen,0,0,0,00,800,600);

    }
    result = ComputerPlaceBlock(placeOccupee, i, j, terrain, image); // on place la piece et on sauvegarde le terrain de jeu actualisé
    return result;
}



/************************************************************************/


unsigned char Sauvegarder(char** mat2, piece* pieces, int tour, int joueur, int maxplayer, unsigned short loosers, short playeringame[4], float tempsjoueur[4])
{
// cette fonction permet d'enregistrer toutes les variables de jeu nécessaires pour reconstituer la partie
    int i;
    int j;
    unsigned char result = 0;
    FILE* fics = NULL;
    fics = fopen("lastgame.txt", "w"); //on ouvre le fichier de sauvegarde

    if(fics!=NULL) // si on a bien pu ouvrir le fichier
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

void SoloGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8)
{
    int gt=9;
    int i;
    int j;
    int maxplayer;
    char maxplayer2;
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


    while(maxplayer < 1 || maxplayer > 3)
    {
    maxplayer2=readkey();
    maxplayer=maxplayer2-'0';
    }

    for(i = 1; i<maxplayer+1; i++)      //on définir le nombre de joueurs IA en plus de l'utilisateur
        playerstype[i] = 2;

    i = 1;
    j = 0;
    DisplayGrid(image); //affichage des bordures du jeu
    while (playeringame[i-1] != 1) //tant qu'il n'y a pas de vainqueur
    {
        textprintf_ex(image, font, 10, 500-gt, makecol(255,0,0), makecol(0,0,0), "Tour: %d ", j+1);
        blit(image,screen,0,0,0,00,800,600);
        if(playeringame[i-1] != -1) //si le joueur n'est pas déclaré comme perdant
        {
            DisplayAvailableBlocks(pieces, i, image);
            if(j>0)
                playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
            if(playeringame[i-1] == 0)
            {
                // on controle si un utilisateur ou l'ordinateur joue (et on donne la main a l'un ou a l'autre en conséquence)
                if(playerstype[i-1] == 1)
                    terrain = PlayerMove(pieces, i, playerstype[i-1], j, terrain, &moves, image);
                else if(playerstype[i-1] == 2)
                    terrain = ComputerMove(pieces, placeOccupee, i, playerstype[i-1], j, terrain, &selectedblockindex, &moves, image);
            }

            if(playeringame[i-1] == -1) // si un joueur a perdu
            {
                textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "PLAYER %d OUT", i);
                blit(image,screen,0,0,0,00,800,600);
                rest(1000);

                for(i=0; i<=maxplayer; i++) // on controle le nombre de joueur toujours en jeu
                    loosers -= playeringame[i];
                if(loosers == maxplayer) //si il ne reste qu'un joueur il est déclaré vainqueur
                {
                    for(i=0; i<=maxplayer; i++)
                        if(playeringame[i] == 0)
                            playeringame[i] = 1;
                }
                else
                    loosers = 0; // sinon on reset le compteur

            }
            DisplayTerrain(terrain, image);
        }
        i++; //incémentation du joueur actf
        if (i > maxplayer + 1) //si on dépasse le nombre max de joueurs
        {
            j++; //on incrémente le tour
            i = 1; // on replace le joueur à 1
        }
    }
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"LE JOUEUR %d A GAGNE", i);
        blit(image,screen,0,0,0,00,800,600);
        while (!keypressed()) {}
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"                     ");
        textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "              ");
        char use;
        use=readkey();
        Menu(image, image4, image5, image6, image7, image8);
}

void MultiplayerGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8)
{
    // partie multijoueur
    int gt=9;
    int i;
    int j;
    int maxplayer;
    char maxplayer2;
    float temps_tour;
    unsigned short selectedblockindex;
    unsigned short loosers = 0;
    unsigned short moves;
    piece* pieces = NULL;
    char** terrain = NULL;
    char** placeOccupee = NULL;
    short playeringame[4] = {0,0,0,0}; //état du joueur: gagné/perdu/en jeu
    clock_t temps_init;
    clock_t temps_end;
    float tempsjoueur[4] = {0,0,0,0}; // temps de jeu

    srand(time(NULL));

    terrain = Dynamic2020Matrix(terrain, '0');
    placeOccupee = Dynamic2020Matrix(placeOccupee, '0');
    pieces = InitialiseBlocks();


    while(maxplayer < 1 || maxplayer > 3)
    {
    maxplayer2=readkey();
    maxplayer=maxplayer2-'0';
    }
    DisplayGrid(image);

        textprintf_ex(image, font, 10, 500+gt, makecol(255,0,0), makecol(0,0,0), "Temps:");
        blit(image,screen,0,0,0,00,800,600);
    for(i=0; i<maxplayer+1; i++)
    {
        textprintf_ex(image, font, 10, 500+gt*2+i*gt, makecol(255,0,0), makecol(0,0,0), "J%d = 0", i+1);
        blit(image,screen,0,0,0,00,800,600);
    }

    i = 1;
    j = 0;
    while (playeringame[i-1] != 1) //tant qu'aucun joueur n'a gagné
    {
        textprintf_ex(image, font, 10, 500-gt, makecol(255,0,0), makecol(0,0,0), "Tour: %d ", j+1);
        blit(image,screen,0,0,0,00,800,600);
        if(playeringame[i-1] != -1) // si le joueur n'a pas perdu
        {
            DisplayAvailableBlocks(pieces, i, image);  //affichage des pieces dispo
            if(j > 1) // on controle si un joueur a gagné ou perdu a partir du 2e tour
                playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
            if(!playeringame[i-1]) // tant si le joueur est toujours en jeu
            {
                temps_init = clock(); //temps général au début du tour
                terrain = PlayerMove(pieces, i, 1, j, terrain, &moves, image);
                temps_end = clock();
                temps_tour = temps_end - temps_init; //Calcul du temps du tour
                temps_tour/=1000.0;
                tempsjoueur[i-1] += temps_tour; //calcul et affichage du temps de jeu total

                textprintf_ex(image, font, 10, 500+gt+i*gt, makecol(255,0,0), makecol(0,0,0), "J%d = %.2f", i, tempsjoueur[i-1]);
                blit(image,screen,0,0,0,00,800,600);
            }
            if(playeringame[i-1] == -1) //si le joueur a perdu
            {
                textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "PLAYER %d OUT", i);
                blit(image,screen,0,0,0,00,800,600);
                rest(1000);

                for(i=0; i<=maxplayer; i++) //  on compte le nombre de joueur encore en jeu
                    loosers -= playeringame[i];
                if(loosers == maxplayer) // si il ne reste qu'un joueur
                {
                    for(i=0; i<=maxplayer; i++)
                        if(playeringame[i] == 0)
                            playeringame[i] = 1; //celui ci est déclaré vainqueur
                }
                else
                    loosers = 0; //sinon on remet le compteur a 0

            }
            DisplayTerrain(terrain, image); //affichage du terrain
        }
        i++; //incrémentation du n° du joueur actif
        if (i > maxplayer + 1) // si on dépace le nombre de joueur de la partie
        {
            j++; //on incrémente le numéro du tour
            i = 1; //on reset le numéro de joueur
        }
        Sauvegarder(terrain, pieces, j, i, maxplayer, loosers, playeringame, tempsjoueur);

    }
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"LE JOUEUR %d A GAGNE", i);
        blit(image,screen,0,0,0,00,800,600);
        while (!keypressed()) {}
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"                     ");
        textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "              ");
        for (i=1; i<5; i++)
            textprintf_ex(image, font, 10, 500+gt+i*gt, makecol(255,0,0), makecol(0,0,0), "                ");
        char use;
        use=readkey();
        Menu(image, image4, image5, image6, image7, image8);

}

unsigned char RestartGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8)
{
    // jeu lancé depuis une sauvegarde
    int gt=9;
    int i;
    int j;
    int w;
    char use;
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

    fics = fopen("lastgame.txt", "r"); //ouverture du fichier


    if(fics != NULL)
    {
        result = 1;
        terrain = Dynamic2020Matrix(terrain, '0');
        placeOccupee = Dynamic2020Matrix(placeOccupee, '0');
        pieces = InitialiseBlocks();

        for(a = 0; a < 20 ; a++) {   //recuperation matrice de jeu
            for(b = 0; b< 20; b++)
            {
                fscanf(fics, "%c", &scaned);
                if(scaned == 10)
                    fscanf(fics, "%c", &scaned);
                terrain[a][b] = scaned;
        //        printf("%c", terrain[a][b]);
            }    //        printf("\n");
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

        DisplayGrid(image);
        textprintf_ex(image, font, 10, 500+gt, makecol(255,0,0), makecol(0,0,0), "Temps:");
        blit(image,screen,0,0,0,00,800,600);
        for(a=0; a<maxplayer+1; a++)
        {
            textprintf_ex(image, font, 10, 500+gt+a*gt+gt, makecol(255,0,0), makecol(0,0,0), "J%d = %.2f", i+1, tempsjoueur[a]);
            blit(image,screen,0,0,0,00,800,600);
        }

        //MULTIPLAYER GAME
        DisplayTerrain(terrain, image);
        while (playeringame[i-1] != 1)
        {
        textprintf_ex(image, font, 10, 500-gt, makecol(255,0,0), makecol(0,0,0), "Tour: %d ", j+1);
        blit(image,screen,0,0,0,00,800,600);
            if(playeringame[i-1] != -1)
            {
                DisplayAvailableBlocks(pieces, i, image);
                if(j > 1)
                    playeringame[i-1] = EndGame(pieces, placeOccupee, i, terrain, &selectedblockindex, &moves);
                if(!playeringame[i-1])
                {
                    temps_init = clock(); //temps général au début du tour
                    terrain = PlayerMove(pieces, i, 1, j, terrain, &moves, image);
                    temps_end = clock();
                    temps_tour = temps_end - temps_init; //Calcul du temps du tour
                    temps_tour/=1000.0;
                    tempsjoueur[i-1] += temps_tour;
                textprintf_ex(image, font, 10, 500+gt+i*gt, makecol(255,0,0), makecol(0,0,0), "J%d = %.2f", i, tempsjoueur[i-1]);
                blit(image,screen,0,0,0,00,800,600);

                }
                if(playeringame[i-1] == -1)
                {
                    textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "PLAYER %d OUT", i);
                    blit(image,screen,0,0,0,00,800,600);
                    rest(1000);

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
                DisplayTerrain(terrain, image);
            }
            i++;
            if (i > maxplayer + 1)
            {
                j++;
                i = 1;
            }
            Sauvegarder(terrain, pieces, j, i, maxplayer, loosers, playeringame, tempsjoueur);

        }
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"LE JOUEUR %d A GAGNE", i);
        blit(image,screen,0,0,0,00,800,600);
        while (!keypressed()) {}
        textprintf_ex(image, font, 10, 500, makecol(255,0,0), makecol(0,0,0),"                     ");
        textprintf_ex(image, font, 200, 500+gt, makecol(255,0,0), makecol(0,0,0), "              ");
        use=readkey();
        for (w=1; w<5; w++)
            textprintf_ex(image, font, 10, 500+gt+w*gt, makecol(255,0,0), makecol(0,0,0), "                ");
        Menu(image, image4, image5, image6, image7, image8);
    }
    else
    {
        blit(image8,screen,0,0,0,00,800,600);
        while (!keypressed()) {}
        use=readkey();
        blit(image4,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }


    return result;
}


void Menu(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8)
{
    char use;
    char choix = '0';

    blit(image4,screen,0,0,0,0,SCREEN_W,SCREEN_H);


    while (choix != '5')
    {
        if (keypressed())
        {
            choix = readkey();

            switch (choix)
            {
            case '1':
                blit(image6,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                SoloGame(image, image4, image5, image6, image7, image8);
                break;


            case'2':
                blit(image5,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                MultiplayerGame(image, image4, image5, image6, image7, image8);
                break;

            case '3':
                RestartGame(image, image4, image5, image6, image7, image8);
                break;

            case '4':
                blit(image7,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                while (!keypressed()) {}
                use=readkey();
                blit(image4,screen,0,0,0,0,SCREEN_W,SCREEN_H);
                break;
            case '5':
                break;
            }
        }
    }

}

/************************************************************/


int main()
{
    srand(time(NULL));

    ///INIT ALLEGRO
    BITMAP *image;
    BITMAP *image4;
    BITMAP *image5;
    BITMAP *image6;
    BITMAP *image7;
    BITMAP *image8;

    srand(time(NULL));
    set_uformat(U_ASCII);

    allegro_init();
    install_keyboard();

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0)!=0)
    {
        allegro_message("prb gfx mode");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    image=load_bitmap("image/plateau.bmp",NULL);
    image4=load_bitmap("menu.bmp",NULL);
    image5=load_bitmap("joueurs.bmp",NULL);
    image6=load_bitmap("ordi.bmp",NULL);
    image7=load_bitmap("regles.bmp",NULL);
    image8=load_bitmap("nosave.bmp",NULL);


    if (!image)
    {
        allegro_message("Erreur image.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    if (!image4)
    {
        allegro_message("Erreur image4.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
        if (!image5)
    {
        allegro_message("Erreur image5.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
        if (!image6)
    {
        allegro_message("Erreur image6.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
        if (!image7)
    {
        allegro_message("Erreur image7.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

        if (!image8)
    {
        allegro_message("Erreur image8.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    ///FIN INIT ALLEGRO

    Menu(image, image4, image5, image6, image7, image8);
    return 0;
}
END_OF_MAIN();

