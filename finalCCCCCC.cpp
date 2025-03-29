#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Console.h"
#include <chrono>//clock
#include <thread>
#include <cstdlib>
#include <mutex>
#include <vector>


using namespace std;

mutex mtx;

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum Direction { LEFT, RIGHT, UP, DOWN };

void goToXY(int x, int y) { // переміцює курсор в задану позицію
    COORD pos = { x, y }; // створює позицію координат
    HANDLE hOutPut = GetStdHandle(STD_OUTPUT_HANDLE); // 
    SetConsoleCursorPosition(hOutPut, pos); // встановлює курсор в задану позицію
}

void hideCurcor(bool showFlag) { // ховає курсор
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);

}

class Hero {
    int hp;
    int h, w;
    COORD position;
    char symbol;
    string name;
    Direction dir;
    int size = 0;
    void print(char filler) { // відображає в консолі переміщення або ж очищає консоль в залежності від переданого символа
        for (int y = 0; y < h; y++)
        {
            goToXY(position.X, position.Y + y);
            for (int x = 0; x < w; x++)
            {
                cout << filler;
            }
            cout << endl;
        }
    }
public:
    Hero() : hp(100), h(1), w(1), position({ 0,0 }), symbol('#'), name("No name"), dir(RIGHT) {}
    Hero(string name, int h, int w, char s) : hp(100), position({ 0,0 }), symbol(s), name(name), dir(RIGHT) {
        h > 1 ? this->h = h : this->h = 1;
        this->w = w > 1 ? w : 1;
    }
    void showInfo()const {
        cout << "========= PLayer [ " << name << " ] == HP : [ " << hp << " ] =========" << endl;
    }
    void setPosition(int x, int y) { // встановлює початкову позицію
        position.X = x >= 0 ? x : 0;
        position.Y = y >= 0 ? y : 0;
    }
    void markHero() {
        SetConsoleTextAttribute(hConsole, ConsoleColors::RED);
        print(symbol);
    }
    void changeDirection(Direction dir) { // змінює direction
        this->dir = dir;
    }
    bool moveRight() { //рухається вправо при нажатті клавіші
        if (isValidPosition(position.X + 1, position.Y)) {
            clearHero();
            position.X++;
            printHero();
            return true;
        }
        return false;
    }
    bool moveLeft() { //рухається вліво при нажатті клавіші
        if (isValidPosition(position.X - 1, position.Y)) {
            clearHero();
            position.X--;
            printHero();
            return true;
        }
        return false;
    }
    bool moveDown() { //рухається вниз при нажатті клавіші
        if (isValidPosition(position.X, position.Y + 1)) {
            clearHero();
            position.Y++;
            printHero();
            return true;
        }
        return false;
    }
    bool moveUp() { //рухається верх при нажатті клавіші
        if (isValidPosition(position.X, position.Y - 1)) {
            clearHero();
            position.Y--;
            printHero();
            return true;
        }
        return false;
    }

    void printHero() { // виводить в консоль персонажа за його позицією
        SetConsoleTextAttribute(hConsole, ConsoleColors::DARK_GREEN); // задає колір консолі
        print(symbol);
    }
    void clearHero() {  // очищає консоль
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLACK); // задає колір консолі
        print(' ');
    }
    bool isValidPosition(int x, int y) { // перевіряємо чи наступний елемент не виходить за межі
        return x >= 0 && y >= 1 && x <= 119 && y <= 28;
    }
    COORD getPos() {
        return position;
    }
    Direction getDir() {
        return dir;
    }
};

class Enemy {
    Direction dirE;
    COORD position;
    int h, w;
    int i;
    char symbol;
    void print(char filler) { // відображає в консолі переміщення або ж очищає консоль в залежності від переданого символа
        for (int y = 0; y < h; y++)
        {
            goToXY(position.X, position.Y + y);
            for (int x = 0; x < w; x++)
            {
                cout << filler;
            }
            cout << endl;
        }
    }
public:
    Enemy() : dirE(RIGHT), w(1), h(1), symbol('#'), position({ 0,0 }) {}
    bool isValidPosition(int x, int y) { // перевіряємо чи наступний елемент не виходить за межі
        return x >= 0 && y >= 1 && x <= 119 && y <= 28;
    }
    void setPosition(int x, int y) { // встановлює початкову позицію
        position.X = x >= 0 ? x : 0;
        position.Y = y >= 0 ? y : 0;
    }
    bool moveRight() { //рухається вправо при нажатті клавіші
        if (isValidPosition(position.X + 1, position.Y)) {
            clearEnemy();
            this->position.X++;
            printEnemy();
            return true;
        }
        return false;
    }
    bool moveLeft() { //рухається вліво при нажатті клавіші
        if (isValidPosition(position.X - 1, position.Y)) {
            clearEnemy();
            this->position.X--;
            printEnemy();
            return true;
        }
        return false;
    }
    bool moveDown() { //рухається вниз при нажатті клавіші
        if (isValidPosition(position.X, position.Y + 1)) {
            clearEnemy();
            this->position.Y++;
            printEnemy();
            return true;
        }
        return false;
    }
    bool moveUp() { //рухається верх при нажатті клавіші
        if (isValidPosition(position.X, position.Y - 1)) {
            clearEnemy();
            this->position.Y--;
            printEnemy();
            return true;
        }
        return false;
    }
    void move() {
        //time_t interval1 = 500;
        /*time_t interval2 = 500;
        time_t start = clock();*/
        srand(time(0));
        if (isValidPosition(position.X, position.Y)) {

            if (i == 5) {
                int randomNumber = 1 + rand() % 4;
                switch (randomNumber) {
                case 1: dirE = RIGHT; break;
                case 2: dirE = LEFT; break;
                case 3: dirE = UP; break;
                case 4: dirE = DOWN; break;
                }
                //start = clock();
                i = 0;
            }
            //cout << position.X << " " << position.Y << endl;
            Sleep(100);
            bool isMoved = false;
            i++;
            switch (dirE) {
            case LEFT: isMoved = moveLeft(); break;
            case RIGHT: isMoved = moveRight(); break;
            case UP: isMoved = moveUp(); break;
            case DOWN: isMoved = moveDown(); break;
            }

            if (!isMoved) {
                i = 4; // Зупиняємо рух, якщо виходимо за межі
            }
            //start = clock();

        }
    }
    void printEnemy() { // виводить в консоль персонажа за його позицією
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLUE); // задає колір консолі
        print(symbol);
    }
    void clearEnemy() {  // очищає консоль
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLACK); // задає колір консолі
        print(' ');
    }
    Direction getDirection() {
        return dirE;
    }
    COORD getPosE() {
        return position;
    }
};

Hero hero("King", 1, 1, char(219));
Enemy en;

void loseOrWin(COORD pB) {
    if (pB.X == hero.getPos().X && pB.Y == hero.getPos().Y) {
        cout << "You lose!!!" << endl;
        exit(0);
    }
    /*cout << hero.getPos().X << " " <<  hero.getPos().Y << endl;
    cout << pB.X << " " << pB.Y << endl;*/
    if (pB.X == en.getPosE().X && pB.Y == en.getPosE().Y) {
        cout << "You win!!!" << endl;
        exit(0);
    }
}

class Bullet {
    Direction dirB;
    COORD position;
    int h, w;
    char symbol;
    void print(char filler) { // відображає в консолі переміщення або ж очищає консоль в залежності від переданого символа
        for (int y = 0; y < h; y++)
        {
            goToXY(position.X, position.Y + y);
            for (int x = 0; x < w; x++)
            {
                cout << filler;
            }
            cout << endl;
        }
    }
public:
    Bullet() = delete;
    Bullet(Direction d) : dirB(d), w(1), h(1), symbol('*') {}
    bool isValidPosition(int x, int y) { // перевіряємо чи наступний елемент не виходить за межі
        return x >= 0 && y >= 1 && x <= 119 && y <= 28;
    }
    void setPosition(int x, int y) { // встановлює початкову позицію
        position.X = x >= 0 ? x : 0;
        position.Y = y >= 0 ? y : 0;
    }
    bool moveRight() { //рухається вправо при нажатті клавіші
        if (isValidPosition(position.X + 1, position.Y)) {
            loseOrWin(position);
            clearBullet();
            position.X++;
            printBullet();
            return true;
        }
        return false;
    }
    bool moveLeft() { //рухається вліво при нажатті клавіші
        if (isValidPosition(position.X - 1, position.Y)) {
            loseOrWin(position);
            clearBullet();
            position.X--;
            printBullet();
            return true;
        }
        return false;
    }
    bool moveDown() { //рухається вниз при нажатті клавіші
        if (isValidPosition(position.X, position.Y + 1)) {
            loseOrWin(position);
            clearBullet();
            position.Y++;
            printBullet();
            return true;
        }
        return false;
    }
    bool moveUp() { //рухається верх при нажатті клавіші
        if (isValidPosition(position.X, position.Y - 1)) {
            loseOrWin(position);
            clearBullet();
            position.Y--;
            printBullet();
            return true;
        }
        return false;
    }
    bool move() {
        /*time_t interval = 400;
        time_t start = clock();*/

        if (isValidPosition(position.X, position.Y)) {
            bool isMoved = false;
            //cout << position.X << " " << position.Y << endl;
            switch (dirB) {
            case LEFT: isMoved = moveLeft(); break;
            case RIGHT: isMoved = moveRight(); break;
            case UP: isMoved = moveUp(); break;
            case DOWN: isMoved = moveDown(); break;
            }
            if (!isMoved) {
                clearBullet();
            }
            return isMoved;

            //start = clock();

        }
    }
    void printBullet() { // виводить в консоль персонажа за його позицією
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLUE); // задає колір консолі
        print(symbol);
    }
    void clearBullet() {  // очищає консоль
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLACK); // задає колір консолі
        print(' ');
    }
    void markBullet() {
        SetConsoleTextAttribute(hConsole, ConsoleColors::BLACK);
        print(symbol);
    }
    Direction getDirection() {
        return dirB;
    }

};


void deleteBullet(vector<Bullet>& b, int i) {
    b.erase(b.begin() + i);
}
int main()
{

    hideCurcor(false);
    hero.showInfo();
    hero.setPosition(10, 10);
    hero.printHero();


    en.setPosition(20, 20);
    //thread enemyThread(&Enemy::move, en);
    //enemyThread.detach();

    vector<Bullet> bullets;

    while (true) {
        en.move();
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                switch (key) {
                case 77:
                    hero.moveRight();
                    hero.changeDirection(RIGHT);
                    break;
                case 75:
                    hero.moveLeft();
                    hero.changeDirection(LEFT);
                    break;
                case 72:
                    hero.moveUp();
                    hero.changeDirection(UP);
                    break;
                case 80:
                    hero.moveDown();
                    hero.changeDirection(DOWN);
                    break;
                }
            }
            else if (key == 32) { // Пробіл - постріл                
                Bullet temp(hero.getDir());

                switch (temp.getDirection())
                {
                case RIGHT:
                    temp.setPosition(hero.getPos().X + 1, hero.getPos().Y);
                    break;
                case LEFT:
                    temp.setPosition(hero.getPos().X - 1, hero.getPos().Y);
                    break;
                case UP:
                    temp.setPosition(hero.getPos().X, hero.getPos().Y - 1);
                    break;
                case DOWN:
                    temp.setPosition(hero.getPos().X, hero.getPos().Y + 1);
                    break;
                default:
                    break;
                }
                bullets.push_back(temp);
            }
        }
        int i = 0;
        for (auto& t : bullets) {
            Sleep(100);
            if (!t.move()) {
                bullets.erase(bullets.begin() + i);
            }
            i++;
        }

    }

}