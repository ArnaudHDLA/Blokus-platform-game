
typedef struct piece piece;

char ** Dynamic2020Matrix(char ** array, char value);

char ** HorizontalFlip(char** mat);

char ** RotateCW(char** mat);

char ** RotateCCW(char** mat);

unsigned char MoveUp(char ** placeOccupee);

unsigned char MoveLeft(char ** placeOccupee);

unsigned char MoveRight(char ** placeOccupee);

unsigned char MoveDown(char ** placeOccupee);

void DisplayTempMatrix(char ** matrix, unsigned short joueur, char** placeOccupee, BITMAP* image);

void DisplayTerrain(char ** terrain, BITMAP* image);

void DisplayAvailableBlocks(piece* pieces, int joueur, BITMAP* image);

void DisplayManipulationMatrix(char ** mat, BITMAP* image);

char ** BlockSelection(piece* pieces, int joueur, unsigned char playertype, unsigned short* sbindex, BITMAP* image);

char ** Manipulation(char ** mat, unsigned char playertype, unsigned short* moves, BITMAP* image);

unsigned char ControlConnection(char ** placeOccupee, char** placeOccupeeFinal, int joueur);

short CheckPlacableBlock(piece* pieces, int joueur, char** placeOccupee, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves, unsigned char ajustsbindex);

char ** PlaceBlock(char ** selectedblock, unsigned char joueur, unsigned char playertype, int tour, char** placeOccupeeFinal, piece* pieces, unsigned short* sbindex, unsigned short* moves, BITMAP* image);

char ** ComputerPlaceBlock(char ** placeOccupee, unsigned char joueur, int tour, char** placeOccupeeFinal, BITMAP* image);

void DisplayGrid(BITMAP* image);

short EndGame(piece* pieces, char ** placeOccupee, int joueur, char** placeOccupeeFinal, unsigned short* sbindex, unsigned short* moves);

char ** PlayerMove(piece* allpieces,int i,unsigned char playerstype,int j,char ** terrain, unsigned short* moves, BITMAP* image);

char ** ComputerMove(piece* allpieces, char ** placeOccupee, int i, unsigned char playerstype, int j, char ** terrain, unsigned short * sbindex, unsigned short* moves, BITMAP* image);

unsigned char Sauvegarder(char** mat2, piece* pieces, int tour, int joueur, int maxplayer, unsigned short loosers, short playeringame[4], float tempsjoueur[4]);

piece* InitialiseBlocks();

void SoloGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8);

void MultiplayerGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8);

unsigned char RestartGame(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8);

void Menu(BITMAP* image, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8);

