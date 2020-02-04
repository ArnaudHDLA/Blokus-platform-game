void gotoligcol(int lig, int col);

typedef struct piece piece;

char ** Dynamic2020Matrix(char ** array, char value);

char ** HorizontalFlip(char** mat);

char ** RotateCW(char** mat);

char ** RotateCCW(char** mat);

unsigned char MoveUp(char ** placeOccupee);

unsigned char MoveLeft(char ** placeOccupee);

unsigned char MoveRight(char ** placeOccupee);

unsigned char MoveDown(char ** placeOccupee);

void DisplayTempMatrix(char ** matrix, unsigned short joueur, char** placeOccupee);

void DisplayTerrain(char ** terrain);

void DisplayAvailableBlocks(piece* pieces, int joueur);

void DisplayManipulationMatrix(char ** mat);

char ** BlockSelection(piece* pieces, int joueur, unsigned char playertype, unsigned short* sbindex);

char ** Manipulation(char ** mat, unsigned char playertype, unsigned short* moves);

unsigned char ControlConnection(char ** placeOccupee, char** placeOccupeeFinal, int joueur);

short CheckPlacableBlock(piece* pieces, int joueur, char** placeOccupee, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves, unsigned char ajustsbindex);

char ** PlaceBlock(char ** selectedblock, unsigned char joueur, unsigned char playertype, int tour, char** placeOccupeeFinal, piece* pieces, unsigned short* sbindex, unsigned short* moves);

char ** ComputerPlaceBlock(char ** placeOccupee, unsigned char joueur, int tour, char** placeOccupeeFinal);

void regles();

void DisplayGrid();

short EndGame(piece* pieces, char ** placeOccupee, int joueur, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves);

char ** PlayerMove(piece* allpieces,int i,unsigned char playerstype,int j,char ** terrain, unsigned short* moves);

char ** ComputerMove(piece* allpieces, char ** placeOccupee, int i, unsigned char playerstype, int j, char ** terrain, unsigned short * sbindex, unsigned short* moves);

unsigned char Sauvegarder(char** mat2, piece* pieces, int tour, int joueur, int maxplayer, unsigned short loosers, short playeringame[4], float tempsjoueur[4]);

piece* InitialiseBlocks();

void SoloGame();

void MultiplayerGame();

unsigned char RestartGame();

char Menu();
