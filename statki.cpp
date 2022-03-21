#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;
#define X 10
#define Y 21
enum shipType {
    CAR,
    BAT,
    CRU,
    DES
};
enum direction {
    N,
    E,
    S,
    W
};
struct ship {
    shipType type;//typ statku
    int size;//rozmiar statku
    int x, y;//pozycja przodu statku w x i y
    int placed;//czy postawiony 
    int engineX, engineY, cannonX, cannonY;//pola z dzialami/silnikiem
    int moves;//wykonane ruchy MOVE
    int shoots;//ile razy strzelil
    int maxMoves;//maksymalna liczba mozliwych ruchow MOVE
    direction direction;// w ktora strone skierowany 

};
struct player {
    int amount;//ile statkow ma gracz
    int parts;//ile czesci ma gracz
    ship* ships;//tablica zawierajaca statki gracza
    int CAR, BAT, CRU, DES;//ilosc typow statkow
    int carPl, batPl, cruPl, desPl;//ilosc postawionych statkow danego typu
    int shipsPlaced;//ilosc postawionych statkow gracza
    int minX, maxX, minY, maxY;//limity pol, na ktorych gracze moga stawiac swoje statki.
};
struct gameState {
    player gamers[2];//gracz A index 0  gracz B index 1
    int gridY, gridX;//wymiary planszy, do extended version
    char** grid; //plansza
    int activePlayer;// game.player do okreslenia ktory index tablicy
    int lastPlayer; //do walidacji, czy dobry gracz wykonuje ruch
    int error;//0 oznacza ze ok, 1 oznacza ze gdzies wystapil error
    int extended;//przelacza extended strzelanie itp.
};
void makeGrid(gameState* game) {
    for (int i = 0; i < game->gridY; i++) {
        for (int j = 0; j < game->gridX; j++) {
            game->grid[i][j] = ' ';
        }
    }
}
void printGrid(gameState* game) {
    //system("cls");
    for (int i = 0; i < game->gridY; i++) {
        for (int j = 0; j < game->gridX; j++) {
            cout << game->grid[i][j];
        }
        cout << endl;
    }
    cout << "PARTS REMAINING:: A : " << game->gamers[0].parts << " B : " << game->gamers[1].parts << endl;
}
void printExtendedGrid(gameState* game) {
    for (int i = 0; i < game->gridY + 2; i++) {
        for (int j = 0; j < game->gridX + 2; j++) {
            if ((i == 0 or i == 1) and (j == 0 or j == 1) and game->gridY > 10) {
                if (!(game->gridX <= 10 and i == 0)) cout << " ";
            }
            else if ((i == 0 or i == 1) and j == 0 and game->gridY <= 10) cout << " ";
            else if ((i == 0 or i == 1) and j == 1) continue; // kontynuuj
            else if (i == 0 and game->gridX > 10)
                cout << (j - 2) / 10;
            else if (i == 1 and j > 1)
                cout << (j - 2) % 10;
            else if (j == 0 and game->gridY > 10)
                cout << (i - 2) / 10;
            else if (j == 1)
                cout << (i - 2) % 10;
            else if (i > 1 and j > 1) {
                for (int z = 0; z < 2; z++) {
                    for (int k = 0; k < game->gamers[z].amount; k++) {
                        if (game->gamers[z].ships[k].y == (i - 2) and game->gamers[z].ships[k].x == (j - 2)) {
                            if (game->grid[i - 2][j - 2] == '+') {
                                game->grid[i - 2][j - 2] = '@';
                            }
                        }
                        else if (game->gamers[z].ships[k].cannonY == (i - 2) and game->gamers[z].ships[k].cannonX == (j - 2)) {
                            if (game->grid[i - 2][j - 2] == '+') {
                                if (game->gamers[z].ships[k].cannonY == game->gamers[z].ships[k].engineY and game->gamers[z].ships[k].cannonX == game->gamers[z].ships[k].engineX)
                                    game->grid[i - 2][j - 2] = '%';
                                else
                                    game->grid[i - 2][j - 2] = '!';
                            }
                        }
                        else if (game->gamers[z].ships[k].engineY == (i - 2) and game->gamers[z].ships[k].engineX == (j - 2)) {
                            if (game->grid[i - 2][j - 2] == '+') {
                                game->grid[i - 2][j - 2] = '%';
                            }
                        }
                    }
                }
                cout << game->grid[i - 2][j - 2];
            }
        }
        if (!(i == 0 and game->gridX <= 10)) cout << endl;
    }
    cout << "PARTS REMAINING:: A : " << game->gamers[0].parts << " B : " << game->gamers[1].parts << endl;
    for (int i = 0; i < game->gridY; i++) {
        for (int j = 0; j < game->gridX; j++) {
            if (game->grid[i][j] == '@' or game->grid[i][j] == '!' or game->grid[i][j] == '%')
                game->grid[i][j] = '+';
        }
    }
}
void setShips(player* gamer) {
    int car, bat, cru, des;
    cin >> car >> bat >> cru >> des;
    int amount = car + bat + cru + des;
    gamer->ships = new ship[amount];
    for (int i = 0; i < amount; i++) {
        if (i < car) {
            gamer->ships[i].size = 5;
            gamer->ships[i].type = CAR;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 2;
        }
        else if (i < car + bat) {
            gamer->ships[i].size = 4;
            gamer->ships[i].type = BAT;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        else if (i < car + bat + cru) {
            gamer->ships[i].size = 3;
            gamer->ships[i].type = CRU;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        else if (i < car + bat + cru + des) {
            gamer->ships[i].size = 2;
            gamer->ships[i].type = DES;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        gamer->ships[i].placed = 0;
        gamer->CAR = car;
        gamer->BAT = bat;
        gamer->CRU = cru;
        gamer->DES = des;
        gamer->carPl = 0;
        gamer->batPl = 0;
        gamer->cruPl = 0;
        gamer->desPl = 0;
        gamer->parts = 0;
        gamer->amount = amount;
        gamer->shipsPlaced = 0;
    }
}
void checkProximity(char D, char** grid, ship* ships, int* errorCode, int gridX, int gridY) {
    if (D == 'N') {
        for (int i = 0; i < ships->size + 2; i++) {
            for (int j = 0; j < 3; j++) {
                if (((ships->y - 1 + i) >= 0 and (ships->y - 1 + i) < gridY) and ((ships->x - 1 + j) >= 0 and (ships->x - 1 + j) < gridX)) {
                    if (grid[ships->y - 1 + i][ships->x - 1 + j] == '+' or grid[ships->y - 1 + i][ships->x - 1 + j] == 'x')
                        *errorCode = 5;
                }
            }
        }
    }
    else if (D == 'S') {
        for (int i = 0; i < ships->size + 2; i++) {
            for (int j = 0; j < 3; j++) {
                if (((ships->y + 1 - i) >= 0 and (ships->y + 1 - i) < gridY) and ((ships->x - 1 + j) >= 0 and (ships->x - 1 + j) < gridX)) {
                    if (grid[ships->y + 1 - i][ships->x - 1 + j] == '+' or grid[ships->y + 1 - i][ships->x - 1 + j] == 'x')
                        *errorCode = 5;
                }
            }
        }
    }
    else if (D == 'W') {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < ships->size + 2; j++) {
                if (((ships->y - 1 + i) >= 0 and (ships->y - 1 + i) < gridY) and ((ships->x - 1 + j) >= 0 and (ships->x - 1 + j) < gridX)) {
                    if (grid[ships->y - 1 + i][ships->x - 1 + j] == '+' or grid[ships->y - 1 + i][ships->x - 1 + j] == 'x')
                        *errorCode = 5;
                }
            }
        }
    }
    else if (D == 'E') {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < ships->size + 2; j++) {
                if (((ships->y - 1 + i) >= 0 and (ships->y - 1 + i) < gridY) and ((ships->x + 1 - j) >= 0 and (ships->x + 1 - j) < gridX)) {
                    if (grid[ships->y - 1 + i][ships->x + 1 - j] == '+' or grid[ships->y - 1 + i][ships->x + 1 - j] == 'x')
                        *errorCode = 5;
                }
            }
        }
    }
}
void shipDirection(char D, char** grid, ship* ships, int* errorCode) {
    if (D == 'N') {
        for (int k = 0; k < ships->size; k++) {
            if (grid[ships->y + k][ships->x] == '#') {
                *errorCode = 1;
            }
            grid[ships->y + k][ships->x] = '+';
            if (k == ships->size - 1) {
                ships->engineY = ships->y + k;
                ships->engineX = ships->x;
            }
            if (k == 1) {
                ships->cannonY = ships->y + k;
                ships->cannonX = ships->x;
            }
        }
        ships->direction = N;
    }
    else if (D == 'S') {
        for (int k = 0; k < ships->size; k++) {
            if (grid[ships->y - k][ships->x] == '#') {
                *errorCode = 1;
            }
            grid[ships->y - k][ships->x] = '+';
            if (k == ships->size - 1) {
                ships->engineY = ships->y - k;
                ships->engineX = ships->x;
            }
            if (k == 1) {
                ships->cannonY = ships->y - k;
                ships->cannonX = ships->x;
            }
        }
        ships->direction = S;
    }
    else if (D == 'W') {
        for (int k = 0; k < ships->size; k++) {
            if (grid[ships->y][ships->x + k] == '#') {
                *errorCode = 1;
            }
            grid[ships->y][ships->x + k] = '+';
            if (k == ships->size - 1) {
                ships->engineY = ships->y;
                ships->engineX = ships->x + k;
            }
            if (k == 1) {
                ships->cannonY = ships->y;
                ships->cannonX = ships->x + k;
            }
        }
        ships->direction = W;

    }
    else if (D == 'E') {
        for (int k = 0; k < ships->size; k++) {
            if (grid[ships->y][ships->x - k] == '#') {
                *errorCode = 1;
            }
            grid[ships->y][ships->x - k] = '+';
            if (k == ships->size - 1) {
                ships->engineY = ships->y;
                ships->engineX = ships->x - k;
            }
            if (k == 1) {
                ships->cannonY = ships->y;
                ships->cannonX = ships->x - k;
            }
        }
        ships->direction = E;
    }
}
void placeShip(player* gamer, char** grid, int* error, int gridX, int gridY) {
    int errorCode = 0;
    char C[4];
    char D;
    int x, y, i;
    cin >> y >> x >> D >> i >> C;
    if (!strcmp(C, "CAR")) {
        if (gamer->ships[i].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->carPl == gamer->CAR)) {
            errorCode = 3;
        }
        //error jesli poczatek statku jest poza plansza
        else if (x > gamer->maxX or x < gamer->minX or y > gamer->maxY or y < gamer->minY) {
            errorCode = 4;
        }
        //error N jesli koniec statku jest pod dozwolonym polem
        else if (D == 'N' and y + (gamer->ships[i].size - 1) > gamer->maxY) {
            errorCode = 4;
        }
        //error S jesli koniec statku jest ponad dozwolonym polem
        else if (D == 'S' and y - (gamer->ships[i].size - 1) < gamer->minY) {
            errorCode = 4;
        }
        //error W jesli koniec statku jest poza plansza z prawej strony
        else if (D == 'W' and x + (gamer->ships[i].size - 1) > gamer->maxX) {
            errorCode = 4;
        }
        //error E jestli koniec statku jest poza plansza z lewej strony
        else if (D == 'E' and x - (gamer->ships[i].size - 1) < gamer->minX) {
            errorCode = 4;
        }
        else {
            gamer->ships[i].x = x;
            gamer->ships[i].y = y;
            gamer->ships[i].placed = 1;
            checkProximity(D, grid, &gamer->ships[i], &errorCode, gridX, gridY);
            shipDirection(D, grid, &gamer->ships[i], &errorCode);
            gamer->parts = gamer->parts + 5;
            gamer->carPl++;
            gamer->shipsPlaced++;
        }
    }
    else if (!strcmp(C, "BAT")) {
        if (gamer->ships[i + gamer->CAR].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->batPl == gamer->BAT)) {
            errorCode = 3;
        }
        //error jesli poczatek statku jest poza plansza
        else if (x > gamer->maxX or x < gamer->minX or y > gamer->maxY or y < gamer->minY) {
            errorCode = 4;
        }
        //error N jesli koniec statku jest pod dozwolonym polem
        else if (D == 'N' and y + (gamer->ships[i + gamer->CAR].size - 1) > gamer->maxY) {
            errorCode = 4;
        }
        //error S jesli koniec statku jest ponad dozwolonym polem
        else if (D == 'S' and y - (gamer->ships[i + gamer->CAR].size - 1) < gamer->minY) {
            errorCode = 4;
        }
        //error W jesli koniec statku jest poza plansza z prawej strony
        else if (D == 'W' and x + (gamer->ships[i + gamer->CAR].size - 1) > gamer->maxX) {
            errorCode = 4;
        }
        //error E jestli koniec statku jest poza plansza z lewej strony
        else if (D == 'E' and x - (gamer->ships[i + gamer->CAR].size - 1) < gamer->minX) {
            errorCode = 4;
        }
        else {
            gamer->ships[i + gamer->CAR].x = x;
            gamer->ships[i + gamer->CAR].y = y;
            gamer->ships[i + gamer->CAR].placed = 1;
            checkProximity(D, grid, &gamer->ships[i + gamer->CAR], &errorCode, gridX, gridY);
            shipDirection(D, grid, &gamer->ships[i + gamer->CAR], &errorCode);
            gamer->parts = gamer->parts + 4;
            gamer->batPl++;
            gamer->shipsPlaced++;
        }
    }
    else if (!strcmp(C, "CRU")) {
        if (gamer->ships[i + gamer->CAR + gamer->BAT].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->cruPl == gamer->CRU)) {
            errorCode = 3;
        }
        //error jesli poczatek statku jest poza plansza
        else if (x > gamer->maxX or x < gamer->minX or y > gamer->maxY or y < gamer->minY) {
            errorCode = 4;
        }
        //error N jesli koniec statku jest pod dozwolonym polem
        else if (D == 'N' and y + (gamer->ships[i + gamer->CAR + gamer->BAT].size - 1) > gamer->maxY) {
            errorCode = 4;
        }
        //error S jesli koniec statku jest ponad dozwolonym polem
        else if (D == 'S' and y - (gamer->ships[i + gamer->CAR + gamer->BAT].size - 1) < gamer->minY) {
            errorCode = 4;
        }
        //error W jesli koniec statku jest poza plansza z prawej strony
        else if (D == 'W' and x + (gamer->ships[i + gamer->CAR + gamer->BAT].size - 1) > gamer->maxX) {
            errorCode = 4;
        }
        //error E jestli koniec statku jest poza plansza z lewej strony
        else if (D == 'E' and x - (gamer->ships[i + gamer->CAR + gamer->BAT].size - 1) < gamer->minX) {
            errorCode = 4;
        }
        else {
            gamer->ships[i + gamer->CAR + gamer->BAT].x = x;
            gamer->ships[i + gamer->CAR + gamer->BAT].y = y;
            gamer->ships[i + gamer->CAR + gamer->BAT].placed = 1;
            checkProximity(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT], &errorCode, gridX, gridY);
            shipDirection(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT], &errorCode);
            gamer->parts = gamer->parts + 3;
            gamer->cruPl++;
            gamer->shipsPlaced++;
        }
    }
    else if (!strcmp(C, "DES")) {
        if (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->desPl == gamer->DES)) {
            errorCode = 3;
        }
        //error jesli poczatek statku jest poza plansza
        else if (x > gamer->maxX or x < gamer->minX or y > gamer->maxY or y < gamer->minY) {
            errorCode = 4;
        }
        //error N jesli koniec statku jest pod dozwolonym polem
        else if (D == 'N' and y + (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].size - 1) > gamer->maxY) {
            errorCode = 4;
        }
        //error S jesli koniec statku jest ponad dozwolonym polem
        else if (D == 'S' and y - (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].size - 1) < gamer->minY) {
            errorCode = 4;
        }
        //error W jesli koniec statku jest poza plansza z prawej strony
        else if (D == 'W' and x + (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].size - 1) > gamer->maxX) {
            errorCode = 4;
        }
        //error E jestli koniec statku jest poza plansza z lewej strony
        else if (D == 'E' and x - (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].size - 1) < gamer->minX) {
            errorCode = 4;
        }
        else {
            gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].x = x;
            gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].y = y;
            gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].placed = 1;
            checkProximity(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU], &errorCode, gridX, gridY);
            shipDirection(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU], &errorCode);
            gamer->parts = gamer->parts + 2;
            gamer->desPl++;
            gamer->shipsPlaced++;
        }
    }
    if (errorCode == 1) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << "\": PLACING SHIP ON REEF" << endl;
        *error = 1;
    }
    else if (errorCode == 2) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << "\": SHIP ALREADY PRESENT" << endl;
        *error = 1;
    }
    else if (errorCode == 3) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
        *error = 1;
    }
    else if (errorCode == 4) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << "\": NOT IN STARTING POSITION" << endl;
        *error = 1;
    }
    else if (errorCode == 5) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << y << " " << x << " " << D << " " << i << " " << C << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
        *error = 1;
    }
}
void basicShooting(gameState* game, char** grid) {
    int y, x;
    cin >> y >> x;
    if (x > game->gridX or y > game->gridY) {
        cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": FIELD DOES NOT EXIST" << endl;
        game->error = 1;
    }
    else if (game->gamers[0].shipsPlaced != game->gamers[0].amount or game->gamers[1].shipsPlaced != game->gamers[1].amount) {
        cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": NOT ALL SHIPS PLACED" << endl;
        game->error = 1;
    }
    else if (grid[y][x] == '+') {
        grid[y][x] = 'x';
        //odejmowanie partsow po trafieniu w cel
        if (game->activePlayer == 0) {
            game->gamers[1].parts--;
        }
        else if (game->activePlayer == 1) {
            game->gamers[0].parts--;
        }
    }
}
void tempShipN(char** grid, char* temp, ship* ship, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (((ship->y + i) >= 0 and (ship->y + i) < gridY) and (ship->x >= 0 and ship->x < gridX)) {
            temp[i] = grid[ship->y + i][ship->x];
            grid[ship->y + i][ship->x] = ' ';
        }
    }
}
void tempShipS(char** grid, char* temp, ship* ship, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (((ship->y - i) >= 0 and (ship->y - i) < gridY) and (ship->x >= 0 and ship->x < gridX)) {
            temp[i] = grid[ship->y - i][ship->x];
            grid[ship->y - i][ship->x] = ' ';
        }
    }
}
void tempShipW(char** grid, char* temp, ship* ship, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if ((ship->y >= 0 and ship->y < gridY) and ((ship->x + i) >= 0 and (ship->x + i) < gridX)) {
            temp[i] = grid[ship->y][ship->x + i];
            grid[ship->y][ship->x + i] = ' ';
        }
    }
}
void tempShipE(char** grid, char* temp, ship* ship, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if ((ship->y >= 0 and ship->y < gridY) and ((ship->x - i) >= 0 and (ship->x - i) < gridX)) {
            temp[i] = grid[ship->y][ship->x - i];
            grid[ship->y][ship->x - i] = ' ';
        }
    }
}
void placeMovedShipN(char** grid, int* errorCode, ship* ship, char* temp, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (grid[ship->y + i][ship->x] == '#') {
            *errorCode = 3;
            break;
        }
        else if (ship->y + i >= gridY) {
            *errorCode = 2;
            break;
        }
        if (i == ship->size - 1) {
            ship->engineY = ship->y + i;
            ship->engineX = ship->x;
        }
        else if (i == 1) {
            ship->cannonY = ship->y + i;
            ship->cannonX = ship->x;
        }
        if (((ship->y + i) >= 0 and (ship->y + i) < gridY) and (ship->x >= 0 and ship->x < gridX))
            grid[ship->y + i][ship->x] = temp[i];
    }
}
void placeMovedShipS(char** grid, int* errorCode, ship* ship, char* temp, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (grid[ship->y - i][ship->x] == '#') {
            *errorCode = 3;
            break;
        }
        else if (ship->y - i < 0) {
            *errorCode = 2;
            break;
        }
        if (i == ship->size - 1) {
            ship->engineY = ship->y - i;
            ship->engineX = ship->x;
        }
        else if (i == 1) {
            ship->cannonY = ship->y - i;
            ship->cannonX = ship->x;
        }
        if (((ship->y - i) >= 0 and (ship->y - i) < gridY) and (ship->x >= 0 and ship->x < gridX))
            grid[ship->y - i][ship->x] = temp[i];
    }
}
void placeMovedShipW(char** grid, int* errorCode, ship* ship, char* temp, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (grid[ship->y][ship->x + i] == '#') {
            *errorCode = 3;
            break;
        }
        else if (ship->x + i >= gridX) {
            *errorCode = 2;
            break;
        }
        if (i == ship->size - 1) {
            ship->engineY = ship->y;
            ship->engineX = ship->x + i;
        }
        else if (i == 1) {
            ship->cannonY = ship->y;
            ship->cannonX = ship->x + i;
        }
        if ((ship->y >= 0 and ship->y < gridY) and ((ship->x + i) >= 0 and (ship->x + i) < gridX))
            grid[ship->y][ship->x + i] = temp[i];
    }
}
void placeMovedShipE(char** grid, int* errorCode, ship* ship, char* temp, int gridX, int gridY) {
    for (int i = 0; i < ship->size; i++) {
        if (grid[ship->y][ship->x - i] == '#') {
            *errorCode = 3;
            break;
        }
        else if (ship->x + i < 0) {
            *errorCode = 2;
            break;
        }
        if (i == ship->size - 1) {
            ship->engineY = ship->y;
            ship->engineX = ship->x - i;
        }
        else if (i == 1) {
            ship->cannonY = ship->y;
            ship->cannonX = ship->x - i;
        }
        if ((ship->y >= 0 and ship->y < gridY) and ((ship->x - i) >= 0 and (ship->x - i) < gridX))
            grid[ship->y][ship->x - i] = temp[i];
    }
}
void resetshipMoves(player* player) {
    for (int i = 0; i < player->amount; i++) {
        player->ships[i].moves = 0;
        player->ships[i].shoots = 0;
    }
}
void chooseShiptoMove(char t, char** grid, ship* ship, int gridX, int gridY, int* errorCode) {
    char D;
    char* tempShip = new char[ship->size];
    //errory
    if (grid[ship->engineY][ship->engineX] == 'x') {
        *errorCode = 1;
    }
    else if (ship->moves == ship->maxMoves) {
        *errorCode = 4;
    }
    //4 wersje na 4 kierunki
    if (ship->direction == N) {
        if (ship->y == 0) {
            *errorCode = 2;
        }
        if (*errorCode == 0) {
            tempShipN(grid, tempShip, ship, gridX, gridY);
            ship->y = ship->y - 1;
            if (t == 'F') {
                D = 'N';
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipN(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'L') {
                D = 'W';
                ship->direction = W;
                ship->x = ship->x - ship->size + 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipW(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'R') {
                D = 'E';
                ship->direction = E;
                ship->x = ship->x + ship->size - 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipE(grid, errorCode, ship, tempShip, gridX, gridY);
            }
        }
    }
    else if (ship->direction == S) {
        if (ship->y == gridY) {
            *errorCode = 2;
        }
        if (*errorCode == 0) {
            tempShipS(grid, tempShip, ship, gridX, gridY);
            ship->y = ship->y + 1;
            if (t == 'F') {
                D = 'S';
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipS(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'L') {
                D = 'E';
                ship->direction = E;
                ship->x = ship->x + ship->size - 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipE(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'R') {
                D = 'W';
                ship->direction = W;
                ship->x = ship->x - ship->size + 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipW(grid, errorCode, ship, tempShip, gridX, gridY);
            }
        }
    }
    else if (ship->direction == W) {
        if (ship->x == 0) {
            *errorCode = 2;
        }
        if (*errorCode == 0) {
            tempShipW(grid, tempShip, ship, gridX, gridY);
            ship->x = ship->x - 1;
            if (t == 'F') {
                D = 'W';
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipW(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'L') {
                D = 'S';
                ship->direction = S;
                ship->y = ship->y + ship->size - 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipS(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'R') {
                D = 'N';
                ship->direction = N;
                ship->y = ship->y - ship->size + 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipN(grid, errorCode, ship, tempShip, gridX, gridY);
            }
        }
    }
    else if (ship->direction == E) {
        if (ship->x == gridX) {
            *errorCode = 2;
        }
        if (*errorCode == 0) {
            tempShipE(grid, tempShip, ship, gridX, gridY);
            ship->x = ship->x + 1;
            if (t == 'F') {
                D = 'E';
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego 
                placeMovedShipE(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'L') {
                D = 'N';
                ship->direction = N;
                ship->y = ship->y - ship->size + 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipN(grid, errorCode, ship, tempShip, gridX, gridY);
            }
            else if (t == 'R') {
                D = 'S';
                ship->direction = S;
                ship->y = ship->y + ship->size - 1;
                checkProximity(D, grid, ship, errorCode, gridX, gridY); //sprawdza czy nie ma statkow w poblizu nowego pola
                placeMovedShipS(grid, errorCode, ship, tempShip, gridX, gridY);
            }
        }
    }
    delete[] tempShip;
}
void moveShip(player* gamer, char** grid, int* error, int gridX, int gridY) {
    int z, errorCode = 0;
    char C[4];
    char t;// kierunek ruchu F L W
    cin >> z >> C >> t;
    if (!strcmp(C, "CAR")) {
        chooseShiptoMove(t, grid, &gamer->ships[z], gridX, gridY, &errorCode);
        gamer->ships[z].moves++;
    }
    else if (!strcmp(C, "BAT")) {
        chooseShiptoMove(t, grid, &gamer->ships[z + gamer->CAR], gridX, gridY, &errorCode);
        gamer->ships[z + gamer->CAR].moves++;
    }
    else if (!strcmp(C, "CRU")) {
        chooseShiptoMove(t, grid, &gamer->ships[z + gamer->CAR + gamer->BAT], gridX, gridY, &errorCode);
        gamer->ships[z + gamer->CAR + gamer->BAT].moves++;
    }
    else if (!strcmp(C, "DES")) {
        chooseShiptoMove(t, grid, &gamer->ships[z + gamer->CAR + gamer->BAT + gamer->CRU], gridX, gridY, &errorCode);
        gamer->ships[z + gamer->CAR + gamer->BAT + gamer->CRU].moves++;
    }
    if (errorCode == 1) {
        cout << "INVALID OPERATION \"MOVE " << z << " " << C << " " << t << "\": SHIP CANNOT MOVE" << endl;
        *error = 1;
    }
    else if (errorCode == 2) {
        cout << "INVALID OPERATION \"MOVE " << z << " " << C << " " << t << "\": SHIP WENT FROM BOARD" << endl;
        *error = 1;
    }
    else if (errorCode == 3) {
        cout << "INVALID OPERATION \"MOVE " << z << " " << C << " " << t << "\": PLACING SHIP ON REEF" << endl;
        *error = 1;
    }
    else if (errorCode == 4) {
        cout << "INVALID OPERATION \"MOVE " << z << " " << C << " " << t << "\": SHIP MOVED ALREADY" << endl;
        *error = 1;
    }
    else if (errorCode == 5) {
        cout << "INVALID OPERATION \"MOVE " << z << " " << C << " " << t << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
        *error = 1;
    }
}
void extShooting(ship* boat, char** grid, int y, int x, int* errorCode, int gridX, int gridY, int* parts) {
    int dist;
    dist = sqrt(abs((boat->cannonY - y) * (boat->cannonY - y)) + abs((boat->cannonX - x) * (boat->cannonX - x)));
    if (dist > boat->size and boat->type != CAR) {
        *errorCode = 1;
    }
    else if (grid[boat->cannonY][boat->cannonX] == 'x') {
        *errorCode = 2;
    }
    else if (x > gridX or y > gridY) {
        *errorCode = 3;
    }
    else if (boat->shoots == boat->size) {
        *errorCode = 4;
    }
    if (grid[y][x] == '+') {
        grid[y][x] = 'x';
        (*parts)--;
    }
    boat->shoots++;
}
void extendedShooting(gameState* game, player gamer) {
    int z, y, x;
    int errorCode = 0;
    char C[4];
    cin >> z >> C >> y >> x;
    if (!strcmp(C, "CAR")) {
        extShooting(&game->gamers[game->activePlayer].ships[z], game->grid, y, x, &errorCode, game->gridX, game->gridY, &game->gamers[game->lastPlayer].parts);
    }
    else if (!strcmp(C, "BAT")) {
        extShooting(&game->gamers[game->activePlayer].ships[z + gamer.CAR], game->grid, y, x, &errorCode, game->gridX, game->gridY, &game->gamers[game->lastPlayer].parts);
    }
    else if (!strcmp(C, "CRU")) {
        extShooting(&game->gamers[game->activePlayer].ships[z + gamer.CAR + gamer.BAT], game->grid, y, x, &errorCode, game->gridX, game->gridY, &game->gamers[game->lastPlayer].parts);
    }
    else if (!strcmp(C, "DES")) {
        extShooting(&game->gamers[game->activePlayer].ships[z + gamer.CAR + gamer.BAT + gamer.CRU], game->grid, y, x, &errorCode, game->gridX, game->gridY, &game->gamers[game->lastPlayer].parts);
    }
    if (errorCode == 1) {
        cout << "INVALID OPERATION \"SHOOT " << z << " " << C << " " << y << " " << x << "\": SHOOTING TOO FAR" << endl;
        game->error = 1;
    }
    else if (errorCode == 2) {
        cout << "INVALID OPERATION \"SHOOT " << z << " " << C << " " << y << " " << x << "\": SHIP CANNOT SHOOT" << endl;
        game->error = 1;
    }
    else if (errorCode == 3) {
        cout << "INVALID OPERATION \"SHOOT " << z << " " << C << " " << y << " " << x << "\": FIELD DOES NOT EXIST" << endl;
        game->error = 1;
    }
    else if (errorCode == 4) {
        cout << "INVALID OPERATION \"SHOOT " << z << " " << C << " " << y << " " << x << "\": TOO MANY SHOOTS" << endl;
        game->error = 1;
    }
    else if (game->gamers[0].shipsPlaced != game->gamers[0].amount or game->gamers[1].shipsPlaced != game->gamers[1].amount) {
        cout << "INVALID OPERATION \"SHOOT " << z << " " << C << " " << y << " " << x << "\": NOT ALL SHIPS PLACED" << endl;
        game->error = 1;
    }
}
void makeTurn(gameState* game) {
    char input[50];
    char players[2][10] = { "[playerA]","[playerB]" };
    do {
        cin >> input;
        if (!strcmp(input, "PLACE_SHIP")) {
            placeShip(&game->gamers[game->activePlayer], game->grid, &game->error, game->gridX, game->gridY);
        }
        else if (!strcmp(input, "SHOOT")) {
            if (game->extended == 0)
                basicShooting(game, game->grid);
            else if (game->extended == 1)
                extendedShooting(game, game->gamers[game->activePlayer]);
        }
        else if (!strcmp(input, "MOVE")) {
            moveShip(&game->gamers[game->activePlayer], game->grid, &game->error, game->gridX, game->gridY);
        }
        if (game->error == 1) {
            break;
        }
        resetshipMoves(&game->gamers[game->lastPlayer]);
    } while (strcmp(input, players[game->activePlayer]));
}
void setshipsDefault(player* gamer) {
    gamer->ships = new ship[10];
    for (int i = 0; i < 10; i++) {
        if (i < 1) {
            gamer->ships[i].size = 5;
            gamer->ships[i].type = CAR;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 2;
        }
        else if (i < 3) {
            gamer->ships[i].size = 4;
            gamer->ships[i].type = BAT;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        else if (i < 6) {
            gamer->ships[i].size = 3;
            gamer->ships[i].type = CRU;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        else if (i < 10) {
            gamer->ships[i].size = 2;
            gamer->ships[i].type = DES;
            gamer->ships[i].moves = 0;
            gamer->ships[i].shoots = 0;
            gamer->ships[i].maxMoves = 3;
        }
        gamer->ships[i].placed = 0;
        gamer->CAR = 1;
        gamer->BAT = 2;
        gamer->CRU = 3;
        gamer->DES = 4;
        gamer->carPl = 0;
        gamer->batPl = 0;
        gamer->cruPl = 0;
        gamer->desPl = 0;
        gamer->parts = 0;
        gamer->amount = 10;
    }
}
void setDefault(gameState* game) {
    game->activePlayer = 0;//zaczyna gracz A(0)
    game->lastPlayer = 1;//do ustalania, ktory gracz powinien wykonac nastepny ruch
    game->error = 0;//dla 0 gra dalej, 1 konczy glowna petle
    for (int v = 0; v < 2; v++) {
        game->gamers[v].ships = new ship[10];
        setshipsDefault(&game->gamers[v]);
        game->gamers[v].shipsPlaced = 0;
        if (v == 0) {
            game->gamers[v].minY = 0;
            game->gamers[v].maxY = 9;
            game->gamers[v].minX = 0;
            game->gamers[v].maxX = 9;
        }
        else if (v == 1) {
            game->gamers[v].minY = 11;
            game->gamers[v].maxY = 20;
            game->gamers[v].minX = 0;
            game->gamers[v].maxX = 9;
        }
    }
}
void setPlayer(gameState* game) {
    char player;
    cin >> player;
    if (player == 'A') {
        game->activePlayer = 0;
        game->lastPlayer = 1;
    }
    else if (player == 'B') {
        game->activePlayer = 1;
        game->lastPlayer = 0;
    }
}
void newGrid(gameState* game) {
    int y, x;
    cin >> y >> x;
    game->gridY = y;
    game->gridX = x;
    game->grid = new char* [game->gridY];
    for (int i = 0; i < game->gridY; i++) {
        game->grid[i] = new char[game->gridX];
    }
    makeGrid(game);
}
void initializePosition(gameState* game) {
    char player;
    int minY, maxY, minX, SuperMax;//verstappen kox
    cin >> player >> minY >> minX >> maxY >> SuperMax;
    if (player == 'A') {
        game->gamers[0].minY = minY;
        game->gamers[0].minX = minX;
        game->gamers[0].maxY = maxY;
        game->gamers[0].maxX = SuperMax;
    }
    else if (player == 'B') {
        game->gamers[1].minY = minY;
        game->gamers[1].minX = minX;
        game->gamers[1].maxY = maxY;
        game->gamers[1].maxX = SuperMax;
    }
}
void placeReef(gameState* game) {
    int y, x;
    cin >> y >> x;
    if (y > game->gridY or X > game->gridX) {
        cout << "INVALID OPERATION \"REEF " << y << " " << x << "\": REEF IS NOT PLACED ON BOARD" << endl;
        game->error = 1;
    }
    game->grid[y][x] = '#';
}
void stateShip(player* gamer, char** grid, int* error, int gridX, int gridY, char player) {
    char D;
    char C[4];
    char state[6];
    int y, x, i;
    int errorCode = 0;
    cin >> y >> x >> D >> i >> C >> state;
    if (!strcmp(C, "CAR")) {
        if (gamer->ships[i].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->carPl == gamer->CAR)) {
            errorCode = 3;
        }
        gamer->ships[i].y = y;
        gamer->ships[i].x = x;
        gamer->ships[i].placed = 1;
        gamer->carPl++;
        gamer->shipsPlaced++;
        gamer->parts = gamer->parts + 5;
        checkProximity(D, grid, &gamer->ships[i], &errorCode, gridX, gridY);
        shipDirection(D, grid, &gamer->ships[i], &errorCode);
        for (int z = 0; z < gamer->ships[i].size; z++) {
            if (state[z] == '0') {
                if (D == 'N') {
                    grid[y + z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'S') {
                    grid[y - z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'W') {
                    grid[y][x + z] = 'x';
                    gamer->parts--;
                }
                else if (D == 'E') {
                    grid[y][x - z] = 'x';
                    gamer->parts--;
                }
            }
        }
    }
    else if (!strcmp(C, "BAT")) {
        if (gamer->ships[i + gamer->CAR].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->batPl == gamer->BAT)) {
            errorCode = 3;
        }
        gamer->ships[i + gamer->CAR].y = y;
        gamer->ships[i + gamer->CAR].x = x;
        gamer->ships[i + gamer->CAR].placed = 1;
        gamer->batPl++;
        gamer->shipsPlaced++;
        gamer->parts = gamer->parts + 4;
        checkProximity(D, grid, &gamer->ships[i + gamer->CAR], &errorCode, gridX, gridY);
        shipDirection(D, grid, &gamer->ships[i + gamer->CAR], &errorCode);
        for (int z = 0; z < gamer->ships[i + gamer->CAR].size; z++) {
            if (state[z] == '0') {
                if (D == 'N') {
                    grid[y + z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'S') {
                    grid[y - z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'W') {
                    grid[y][x + z] = 'x';
                    gamer->parts--;
                }
                else if (D == 'E') {
                    grid[y][x - z] = 'x';
                    gamer->parts--;
                }
            }
        }
    }
    else if (!strcmp(C, "CRU")) {
        if (gamer->ships[i + gamer->CAR + gamer->BAT].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->cruPl == gamer->CRU)) {
            errorCode = 3;
        }
        gamer->ships[i + gamer->CAR + gamer->BAT].y = y;
        gamer->ships[i + gamer->CAR + gamer->BAT].x = x;
        gamer->ships[i + gamer->CAR + gamer->BAT].placed = 1;
        gamer->cruPl++;
        gamer->shipsPlaced++;
        gamer->parts = gamer->parts + 3;
        checkProximity(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT], &errorCode, gridX, gridY);
        shipDirection(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT], &errorCode);
        for (int z = 0; z < gamer->ships[i + gamer->CAR + gamer->BAT].size; z++) {
            if (state[z] == '0') {
                if (D == 'N') {
                    grid[y + z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'S') {
                    grid[y - z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'W') {
                    grid[y][x + z] = 'x';
                    gamer->parts--;
                }
                else if (D == 'E') {
                    grid[y][x - z] = 'x';
                    gamer->parts--;
                }
            }
        }
    }
    else if (!strcmp(C, "DES")) {
        if (gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].placed == 1) {
            errorCode = 2;
        }
        else if ((gamer->desPl == gamer->DES)) {
            errorCode = 3;
        }
        gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].y = y;
        gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].x = x;
        gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].placed = 1;
        gamer->desPl++;
        gamer->shipsPlaced++;
        gamer->parts = gamer->parts + 2;
        checkProximity(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU], &errorCode, gridX, gridY);
        shipDirection(D, grid, &gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU], &errorCode);
        for (int z = 0; z < gamer->ships[i + gamer->CAR + gamer->BAT + gamer->CRU].size; z++) {
            if (state[z] == '0') {
                if (D == 'N') {
                    grid[y + z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'S') {
                    grid[y - z][x] = 'x';
                    gamer->parts--;
                }
                else if (D == 'W') {
                    grid[y][x + z] = 'x';
                    gamer->parts--;
                }
                else if (D == 'E') {
                    grid[y][x - z] = 'x';
                    gamer->parts--;
                }
            }
        }
    }
    if (errorCode == 1) {
        cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << C << "\": PLACING SHIP ON REEF" << endl;
        *error = 1;
    }
    else if (errorCode == 2) {
        cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << C << "\": SHIP ALREADY PRESENT" << endl;
        *error = 1;
    }
    else if (errorCode == 3) {
        cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << C << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
        *error = 1;
    }
    else if (errorCode == 5) {
        cout << "INVALID OPERATION \"SHIP " << player << " " << y << " " << x << " " << D << " " << i << " " << C << " " << state << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
        *error = 1;
    }
}
int main() {
    gameState game;
    int state = 0;
    //tworzenie grida
    game.gridY = Y;
    game.gridX = X;
    game.extended = 0;
    game.grid = new char* [game.gridY];
    for (int i = 0; i < game.gridY; i++) {
        game.grid[i] = new char[game.gridX];
    }
    makeGrid(&game);
    setDefault(&game);//ustawianie defaultowych wartosci dla gry
    char input[50];
    for (int z = 0; z < 100; z++) {
        cin >> input;
        if (!strcmp(input, "[state]")) {
            do {
                cin >> input;
                if (!strcmp(input, "PRINT")) {
                    cin >> state;
                    if (state == 0) {
                        printGrid(&game);
                    }
                    else if (state == 1) {
                        printExtendedGrid(&game);
                    }
                }
                else if (!strcmp(input, "SET_FLEET")) {
                    char player;
                    cin >> player;
                    if (player == 'A') {
                        delete[] game.gamers[0].ships;
                        setShips(&game.gamers[0]);
                    }
                    else if (player == 'B') {
                        delete[] game.gamers[1].ships;
                        setShips(&game.gamers[1]);
                    }
                }
                else if (!strcmp(input, "NEXT_PLAYER")) {
                    setPlayer(&game);
                }
                else if (!strcmp(input, "BOARD_SIZE")) {
                    for (int i = 0; i < game.gridY; i++) {
                        delete[] game.grid[i];
                    }
                    delete[] game.grid;
                    newGrid(&game);
                }
                else if (!strcmp(input, "INIT_POSITION")) {
                    initializePosition(&game);
                }
                else if (!strcmp(input, "REEF")) {
                    placeReef(&game);
                }
                else if (!strcmp(input, "SHIP")) {
                    char player;
                    int index = 0;
                    cin >> player;
                    if (player == 'A') {
                        index = 0;
                    }
                    else if (player == 'B') {
                        index = 1;
                    }
                    stateShip(&game.gamers[index], game.grid, &game.error, game.gridX, game.gridY, player);
                }
                else if (!strcmp(input, "EXTENDED_SHIPS")) {
                    game.extended = 1;
                }
                if (game.error == 1) {
                    break;
                }
            } while (strcmp(input, "[state]"));
        }//koniec state'a
        else if (!strcmp(input, "[playerA]")) {
            //sprawdzanie, czy dobry gracz wykonuje ruch
            if (game.lastPlayer == 0) {
                cout << "INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED" << endl;
                game.error = 1;
                break;
            }
            makeTurn(&game);
            //wygranie gry
            if (game.gamers[1].parts == 0 and game.gamers[1].shipsPlaced > 0) {
                cout << "A won" << endl;
                break;
            }
            game.lastPlayer = 0;
            game.activePlayer = 1;
        }//koniec gracza A
        else if (!strcmp(input, "[playerB]")) {
            //sprawdzanie, czy dobry gracz wykonuje ruch
            if (game.lastPlayer == 1) {
                cout << "INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED" << endl;
                game.error = 1;
                break;
            }
            makeTurn(&game);
            //wygranie gry
            if (game.gamers[0].parts == 0 and game.gamers[0].shipsPlaced > 0) {
                cout << "B won" << endl;
                break;
            }
            game.lastPlayer = 1;
            game.activePlayer = 0;
        }//koniec gracza B
        //wywalenie gry jak jest error
        if (game.error == 1) {
            break;
        }
    }//koniec glownego fora
    for (int i = 0; i < game.gridY; i++) {
        delete[] game.grid[i];
    }
    delete[] game.grid;
}