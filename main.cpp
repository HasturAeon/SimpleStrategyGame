#include <iostream>
#include <windows.h>
#include "Tile.h"
#include "Point.h"

using namespace std;

void __init__(int startPointAX, int startPointAY, int startPointBX, int startPointBY);
void drawGrid();
bool operateInput();
int readCommand();
bool invadeTile(int goalX, int goalY, int owner);
bool checkNeighbour(int owner, int x, int y);
void inspectTile(int x, int y);
bool recruitCivilians(int goalX, int goalY, int howMany, int owner);
void updateMap();
void showStatus();
void helpTab();
void collectTaxes(int owner);
void transferMilitary(int startX, int startY, int goalX, int goalY, int howMuch, int owner);
void recruitByForce(int owner);
bool checkGame(int startPointAX, int startPointAY, int startPointBX, int startPointBY);
void giveUp();

const int WIDTH = 11;
const int HEIGHT = 11;
const int howManyCities = 5;
const int startingMoney = 500;

const int OVCiviliansPT = 2;
const int OCCiviliansPT = 4;
const int OVMoneyPT = 2;
const int OCMoneyPT = 15;
const int OVMilitaryPowerPT = 1;
const int OCMilitaryPowerPT = 2;

const int NVCiviliansPT = 1;
const int NCCiviliansPT = 3;
const int NVMoneyPT = 1;
const int NCMoneyPT = 15;
const int NVMilitaryPowerPT = 0;
const int NCMilitaryPowerPT = 1;

const int NEUTRAL_ID = 0;
const int PLAYER_ID = 1;
const int ENEMY_ID = 2;

Tile grid[WIDTH][HEIGHT];
Point cities[howManyCities];

int playerMoney = startingMoney;
int enemyMoney = startingMoney;

int main() {
    string decision;

    bool done = false;

    int startPointAX = 0, startPointAY = 5;
    int startPointBX = 10, startPointBY = 5;

    __init__(startPointAX, startPointAY, startPointBX, startPointBY);

    while(!done) {
        system("cls");
        drawGrid();
        if (operateInput()) {
            done = true;
        }
        if (checkGame(startPointAX, startPointAY, startPointBX, startPointBY)) {
            if (grid[startPointBX][startPointBY].ownerID == PLAYER_ID) {
                system("cls");
                cout << "Congratulations! You won the game!" << endl << "Do you want to try again?[Y/N]" << endl;
                cin >> decision;
                if (decision[0] == 'Y') {
                    __init__(startPointAX, startPointAY, startPointBX, startPointBY);
                } else {
                    done = true;
                }
            } else {
                system("cls");
                cout << "You lost... Do you want to try again?[Y/N]" << endl;
                cin >> decision;
                if (decision[0] == 'Y') {
                    __init__(startPointAX, startPointAY, startPointBX, startPointBY);
                } else {
                    done = true;
                }
            }
        }
        updateMap();
    }

    return 0;
}

bool checkGame(int startPointAX, int startPointAY, int startPointBX, int startPointBY) {
    if (grid[startPointAX][startPointAY].ownerID != PLAYER_ID || grid[startPointBX][startPointBY].ownerID != ENEMY_ID) {
        return true;
    } return false;
}

bool operateInput() {
    bool finishTurn = false;

    int input = 0;

    int startX = 0;
    int startY = 0;

    int goalX = 0;
    int goalY = 0;

    int howMany = 0;
    while (!finishTurn) {
        input = readCommand();
        if (input == -1) { finishTurn = true; return true; }
        else if (input == 1) {
        cin >> goalX >> goalY;
        if (!invadeTile(goalX, goalY, PLAYER_ID)) {
                finishTurn = false;
            } else {
                finishTurn = true;
            }
        } else if (input == 2) {
            cin >> goalX >> goalY;
            inspectTile(goalX, goalY);
        } else if (input == 3) {
            cin >> goalX >> goalY >> howMany;
            if (!recruitCivilians(goalX, goalY, howMany, PLAYER_ID)) {
                finishTurn = false;
            } else {
                finishTurn = true;
            }
        } else if (input == 4) {
            finishTurn = true;
        } else if (input == 5) {
            showStatus();
        } else if(input == 6) {
            helpTab();
        } else if (input == 7) {
            collectTaxes(PLAYER_ID);
            finishTurn = true;
        } else if (input == 8) {
            cin >> startX >> startY >> goalX >> goalY >> howMany;
            transferMilitary(startX, startY, goalX, goalY, howMany,PLAYER_ID);
            finishTurn = true;
        } else if (input == 9) {
            recruitByForce(PLAYER_ID);
        } else if (input == 10) {
            giveUp();
            finishTurn = true;
        }
    }
    return false;
}

void giveUp() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID) {
                grid[x][y].ownerID == NEUTRAL_ID;
            }
        }
    }
}

void __init__(int startPointAX, int startPointAY, int startPointBX, int startPointBY) {
    cities[0].x = 1;
    cities[0].y = 2;
    cities[1].x = 8;
    cities[1].y = 9;
    cities[2].x = 8;
    cities[2].y = 3;
    cities[3].x = 4;
    cities[3].y = 7;
    cities[4].x = 5;
    cities[4].y = 1;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[x][y].militaryPower = 0;
            grid[x][y].ownerID = NEUTRAL_ID;
            grid[x][y].civilians = 10;
            grid[x][y].money = 5;
        }
    }

    grid[startPointAX][startPointAY].militaryPower = 20;
    grid[startPointAX][startPointAY].ownerID = PLAYER_ID;
    grid[startPointAX][startPointAY].civilians = 30;
    grid[startPointAX][startPointAY].city = true;
    grid[startPointAX][startPointAY].money = 100;
    grid[startPointBX][startPointBY].militaryPower = 20;
    grid[startPointBX][startPointBY].ownerID = ENEMY_ID;
    grid[startPointBX][startPointBY].civilians = 30;
    grid[startPointBX][startPointBY].city = true;
    grid[startPointBX][startPointBY].money = 100;

    for (int y = 0; y < howManyCities; y++) {
        grid[cities[y].x][cities[y].y].city = true;
        grid[cities[y].x][cities[y].y].militaryPower = 10;
        grid[cities[y].x][cities[y].y].civilians = 30;
        grid[cities[y].x][cities[y].y].money = 100;
    }
}

void collectTaxes(int owner) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == owner && grid[x][y].money > 0) {
                playerMoney += grid[x][y].money;
                grid[x][y].money = 0;
            }
        }
    }
}

void recruitByForce(int owner) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == owner) {
                grid[x][y].militaryPower += grid[x][y].civilians;
                grid[x][y].civilians = 0;
            }
        }
    }
}

void transferMilitary(int startX, int startY, int goalX, int goalY, int howMuch, int owner) {
    if (startX + 1 == goalX && startY == goalY || startX - 1 == goalX && startY == goalY || startX == goalX && startY + 1 == goalY || startX == goalX && startY - 1 == goalY) {
        if (grid[startX][startY].ownerID == owner && grid[goalX][goalY].ownerID == owner) {
            if (grid[startX][startY].militaryPower > howMuch) {
                grid[goalX][goalY].militaryPower += howMuch;
                grid[startX][startY].militaryPower -= howMuch;
            } else if (grid[startX][startY].militaryPower <= howMuch) {
                grid[goalX][goalY].militaryPower += grid[startX][startY].militaryPower;
                grid[startX][startY].militaryPower = 0;
            }
        }
    }
}

void showStatus() {
    cout << "Status:" << endl << "  Joint military power: ";
    int militaryPower = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID) {
                militaryPower += grid[x][y].militaryPower;
            }
        }
    }
    cout << militaryPower << endl << "  Joint military power income per turn: ";
    int militaryPowerIncome = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID && !grid[x][y].city) {
                militaryPowerIncome += OVMilitaryPowerPT;
            } else if (grid[x][y].ownerID == PLAYER_ID && grid[x][y].city) {
                militaryPowerIncome += OCMilitaryPowerPT;
            }
        }
    }
    cout << militaryPowerIncome << endl << "  Cities: ";
    int cities = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID && grid[x][y].city) {
                cities++;
            }
        }
    }
    cout << cities << endl << "  Joint civilians: ";
    int civilians = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID) {
                civilians += grid[x][y].civilians;
            }
        }
    }
    cout << civilians << endl << "  Joint civilians income per turn: ";
    int civiliansIncome = 0;
    civiliansIncome += cities * OCCiviliansPT;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID && !grid[x][y].city) {
                civiliansIncome += OVCiviliansPT;
            }
        }
    }
    cout << civiliansIncome << endl << "  Collected money: " << playerMoney << endl << "  Money to collect: ";
    int moneyToCollect = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID) {
                moneyToCollect += grid[x][y].money;
            }
        }
    }
    cout << moneyToCollect << endl << "  Joint money income per turn: ";
    int moneyIncome = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].ownerID == PLAYER_ID && !grid[x][y].city) {
                moneyIncome += OVMoneyPT;
            } else if (grid[x][y].ownerID == PLAYER_ID && grid[x][y].city) {
                moneyIncome += OCMoneyPT;
            }
        }
    }
    cout << moneyIncome << endl;
}

void updateMap() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[x][y].city) {
                if (grid[x][y].ownerID == PLAYER_ID || grid[x][y].ownerID == ENEMY_ID) {
                    grid[x][y].civilians += OCCiviliansPT;
                    grid[x][y].militaryPower += OCMilitaryPowerPT;
                    grid[x][y].money += OCMoneyPT;
                } else {
                    grid[x][y].civilians += NCCiviliansPT;
                    grid[x][y].militaryPower += NCMilitaryPowerPT;
                    grid[x][y].money += NCMoneyPT;
                }
            } else {
                if (grid[x][y].ownerID == PLAYER_ID || grid[x][y].ownerID == ENEMY_ID) {
                    grid[x][y].civilians += OVCiviliansPT;
                    grid[x][y].militaryPower += OVMilitaryPowerPT;
                    grid[x][y].money += OVMoneyPT;
                } else {
                    grid[x][y].civilians += NVCiviliansPT;
                    grid[x][y].militaryPower += NVMilitaryPowerPT;
                    grid[x][y].money += NVMoneyPT;
                }
            }
        }
    }
}

void helpTab() {
    cout << "Commands:" << endl;
    cout << "  Exit: Quits the game(THE GAME DOES NOT SAVE!)." << endl;
    cout << "  Help: Shows this tab :)." << endl;
    cout << "  Status: Shows you the state of your kingdom." << endl;
    cout << "  Inspect [Position X] [Position Y]: Shows you the state of a specific tile with the coordinates X and Y." << endl;
    cout << "  Wait: You loose your turn, but the enemy does not." << endl;
    cout << "  Invade [Defending tile position X] [Defending tile position Y]: Invade a tile - if the attacking tile's military power is bigger than your connected tiles, you lose your army and the tile is unscathed, but if your military strength is bigger than the tile you attack, you claim that tile, but loose how many military power the defending tile had." << endl;
    cout << "  Recruit [Position X] [Position Y] [HowMany]: Recruit civilians from the tile with position X and position Y, and recruit this much as you write. If you want to recruit more than there are civilians, you'll recruit all civilians." << endl;
}

void inspectTile(int x, int y) {
    cout << "Owner: ";
    if (grid[x][y].ownerID == NEUTRAL_ID) {
        cout << "None" << endl;
    } else if (grid[x][y].ownerID == PLAYER_ID) {
        cout << "Player" << endl;
    } else {
        cout << "Enemy" << endl;
    }
    cout << "Military Power: " << grid[x][y].militaryPower << endl;
    cout << "Civilians: " << grid[x][y].civilians << endl;
    cout << "Status: ";
    if (grid[x][y].city) {
        cout << "City" << endl;
    } else {
        cout << "Village" << endl;
    }
    cout << "Money: " << grid[x][y].money << endl;
}

bool invadeTile(int goalX, int goalY, int owner) {
    int jointMilitaryPower = 0;
    if (grid[goalX + 1][goalY].ownerID == owner) {
        jointMilitaryPower += grid[goalX + 1][goalY].militaryPower;
    } else if (grid[goalX - 1][goalY].ownerID == owner) {
        jointMilitaryPower += grid[goalX - 1][goalY].militaryPower;
    } else if (grid[goalX][goalY + 1].ownerID == owner) {
        jointMilitaryPower += grid[goalX][goalY + 1].militaryPower;
    } else if (grid[goalX][goalY - 1].ownerID == owner) {
        jointMilitaryPower += grid[goalX][goalY - 1].militaryPower;
    }
    if (checkNeighbour(owner, goalX, goalY)) {
        if (jointMilitaryPower > grid[goalX][goalY].militaryPower) {
            grid[goalX][goalY].ownerID = owner;
            grid[goalX][goalY].militaryPower = jointMilitaryPower - grid[goalX][goalY].militaryPower;
            jointMilitaryPower = 1;
        } else {
            grid[goalX][goalY].militaryPower = grid[goalX][goalY].militaryPower - jointMilitaryPower;
            jointMilitaryPower = 0;
        }
    } else {
        return false;
    } return true;
}

bool recruitCivilians(int goalX, int goalY, int howMany, int owner) {
    if (grid[goalX][goalY].ownerID == owner) {
        if (grid[goalX][goalY].civilians >= howMany) {
            grid[goalX][goalY].civilians = grid[goalX][goalY].civilians - howMany;
            grid[goalX][goalY].militaryPower = grid[goalX][goalY].militaryPower + howMany;
        } else if (grid[goalX][goalY].civilians < howMany) {
            grid[goalX][goalY].militaryPower = grid[goalX][goalY].militaryPower + grid[goalX][goalY].civilians;
            grid[goalX][goalY].civilians = 0;
        }
    } else {
        return false;
    } return true;
}

bool checkNeighbour(int owner, int x, int y) {
    if (grid[x + 1][y].ownerID == owner) {
        return true;
    } else if (grid[x - 1][y].ownerID == owner) {
        return true;
    } else if (grid[x][y + 1].ownerID == owner) {
        return true;
    } else if (grid[x][y - 1].ownerID == owner) {
        return true;
    }

    return false;
}

void drawGrid() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
            if (grid[x][y].ownerID == PLAYER_ID) {
                if (grid[x][y].city == true) {
                    SetConsoleTextAttribute(hConsole, 2);
                    cout << "[]";
                } else {
                    SetConsoleTextAttribute(hConsole, 10);
                    cout << "::";
                }
            }
            else if (grid[x][y].ownerID == ENEMY_ID) {
                if (grid[x][y].city == true) {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "[]";
                } else {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "::";
                }
            }
            else {
                if (grid[x][y].city == true) {
                    SetConsoleTextAttribute(hConsole, 8);
                    cout << "[]";
                } else {
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "::";
                }
            }
        }
        cout << endl;
    }
}

int readCommand() {
    int commandID = 0;

    string command;

    cin >> command;

    if (command == "exit") { //Done
        commandID = -1;
    } else if (command == "invade") { //Done
        commandID = 1;
    } else if (command == "inspect") { //Done
        commandID = 2;
    } else if (command == "recruit") { //Done
        commandID = 3;
    } else if (command == "wait") { //Done
        commandID = 4;
    } else if (command == "status") { //Done
        commandID = 5;
    } else if (command == "help") { //Done
        commandID = 6;
    } else if (command == "taxes") { //Not done
        commandID = 7;
    } else if (command == "transfer") { //Not done
        commandID = 8;
    } else if (command == "forceRecruit") { //Not done
        commandID = 9;
    } else if (command == "giveUp") { //Not done
        commandID = 10;
    }

    return commandID;
}
