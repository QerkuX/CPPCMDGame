#include <windows.h>
#include <iostream>
#include <conio.h>

//define keys
#define VK_W 0x57
#define VK_A 0x41
#define VK_D 0x44

using namespace std;

//Clear function that don't cause flickering (not written by me)
void ClearScreen()
{
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}


char square = 219;
char player = 190;
char door = 177;
int doorPosY;
int doorPosX;
int playerPosY;
int playerPosX;
char playerSprite = 'p';
bool jumping = false;
int jumpHeight = 2;
int jumpStage = 0;
int option;



int main(){

    int y;
    int x;

    char matrix[100][100];

    cout << "Do you want to create your own map: \n1. Yes\n2. No (load default one)\n";
    cin >> option;

    if (option == 1){
        cout << "Write height of your map (max 100): ";
        cin >> y;
        cout << "Write width of your map (max 100): ";
        cin >> x;

        if (x > 100 || y > 100){
                system("cls");
                cout << "Im not securing that -_-" << endl;
                system("pause");
                return 0;
        }

        string level;

        cout << "o - air, x - ground, p - player, d - door \n" << endl;
        for (int i = 0; i < y; i++){
            cin >> level;
            if (level.length() > x){
                system("cls");
                cout << "Im not securing that -_-" << endl;
                system("pause");
                return 0;
            }

            for (int j = 0; j < x; j++){
                matrix[i][j] = level[j];
            }
        }

        bool havePlayer = false, playerHaveGround = false, haveGround = false, haveDoor = false, haveAir = false;
        int tempPlayerX, tempPlayerY;
        for (int i = 0; i < y; i++){
            for (int j = 0; j < x; j++){
                if (matrix[i][j] == 'x'){
                    haveGround = true;
                }
                if (matrix[i][j] == 'p'){
                    havePlayer = true;
                    tempPlayerY = i;
                    tempPlayerX = j;

                }
                if (matrix[i][j] == 'd'){
                    haveDoor = true;
                }
                if (matrix[i][j] == 'o'){
                    haveAir = true;
                }
            }
        }

        if (matrix[tempPlayerY+1][tempPlayerX] == 'x'){
            playerHaveGround = true;
        }
        if (!havePlayer || !haveGround || !haveDoor || !haveAir || !playerHaveGround){
            system("cls");
            cout << "You need to use every block and player need to have ground below him!\n" << endl;
            system("pause");
            return 0;
        }
    }
    else{
        y = 6;
        x = 20;
        char matrixDefault[100][100]=
        {
            {'o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o'},
            {'o','o','o','o','o','o','o','o','o','o','d','o','o','o','o','o','o','o','o','o'},
            {'o','o','o','o','o','o','o','o','o','x','x','o','o','o','o','x','x','o','o','o'},
            {'o','o','x','x','x','x','o','o','o','o','o','o','o','o','o','o','o','o','x','x'},
            {'p','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','x','x'},
            {'x','x','x','x','x','x','x','x','x','x','x','x','o','o','x','x','x','x','x','x'}
        };

        for (int i = 0; i < y; i++){
            for (int j = 0; j < x; j++){
                matrix[i][j] = matrixDefault[i][j];
            }
        }
    }

    //Makes map look good
    for (int i = 0; i < y; i++){
        for (int j = 0; j < x; j++){
            if (matrix[i][j] == 'o'){
                matrix[i][j] = ' ';
            }
            else if (matrix[i][j] == 'x'){
                matrix[i][j] = square;
            }
            else if (matrix[i][j] == 'd'){
                matrix[i][j] = door;
                doorPosY = i;
                doorPosX = j;
            }
            else if (matrix[i][j] == 'p'){
                playerPosY = i;
                playerPosX = j;

                //This is fix to the bug that accour because when you make a map, you need to use 'd' (door), but the game for some reason think that you pressed 'd' in game,
                //so it increses playerPos
                if (option == 1){
                    playerPosX--;
                }
            }
        }
    }

    system("cls");

	while (true)
    {
        //Get input and change player position
		if (GetAsyncKeyState(VK_W) && matrix[playerPosY+1][playerPosX] == square){
            jumping = true;
		}

		if (GetAsyncKeyState(VK_A) && playerPosX != 0 && matrix[playerPosY][playerPosX-1] != square){
            playerSprite = 'q';
			matrix[playerPosY][playerPosX] = ' ';
			playerPosX--;
			matrix[playerPosY][playerPosX] = playerSprite;
		}
		else if (GetAsyncKeyState(VK_D) && playerPosX != x-1 && matrix[playerPosY][playerPosX+1] != square){
            playerSprite = 'p';
			matrix[playerPosY][playerPosX] = ' ';
			playerPosX++;
			matrix[playerPosY][playerPosX] = playerSprite;
		}

		//Jumping
		if (jumping){
            if (jumpStage == jumpHeight || matrix[playerPosY-1][playerPosX] == square){
                jumping = false;
                jumpStage = 0;
			}
            else{
                jumpStage++;
                matrix[playerPosY][playerPosX] = ' ';
                playerPosY--;
                matrix[playerPosY][playerPosX] = playerSprite;
            }
		}

		//Falling
		else if (matrix[playerPosY+1][playerPosX] != square){

            matrix[playerPosY][playerPosX] = ' ';
            playerPosY++;
			matrix[playerPosY][playerPosX] = playerSprite;
		}

        //Clear screen
        ClearScreen();

        //Check if winned
        if (playerPosX == doorPosX && playerPosY == doorPosY){
            system("cls");
            cout << "You winned!\n" << endl;
            system("pause");
            return 0;
        }

        //Check if dead
        if (playerPosY > y){
            system("cls");
            cout << "You are dead!\n" << endl;
            system("pause");
            return 0;
        }

        //Draw map
		for (int i = 0; i < y; i++){
            for (int j = 0; j < x; j++){
                cout << matrix[i][j];
            }
            cout << endl;
		}
		Sleep(100);
	}
}
