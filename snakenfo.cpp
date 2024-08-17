#include <iostream>
#include <vector>   // dynamic array used for snake body
#include <conio.h>  // for _kbhit() and _getch()
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // to set the speed of the game

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class Food { //class food
public:
    int x, y;  // co-ordinates of the food

    Food(int width, int height) {  //constructor of food
        generate(width, height); // set the initial position
    }

    void generate(int width, int height) { //for setting new random position for food
        x = rand() % width;
        y = rand() % height;
    } // set food within the board
};

class Snake {
public:                            // Class name snake
    vector<pair<int, int>> body;
    Direction dir;                 //holds the direction of the snake

    Snake(int startX, int startY) {      //Constructed This will initialize new snake
        body.push_back({startX, startY});  // Initial segment of snake,s body
        dir = STOP;   // initially the snake is in Stop motion
    }

    void setDirection(Direction d) {  // to directs the snake
        dir = d;   //when the key is pressed it will directs the snake
    }

    void move() {    //used to move the snake
        if (dir != STOP) {   // dir not stop snake supposed to move
            for (int i = body.size() - 1; i > 0; i--) {
                body[i] = body[i - 1];
            }
            switch (dir) {  // co=ordinates of snake to move
                case LEFT:  body[0].first--;  break;
                case RIGHT: body[0].first++;  break;
                case UP:    body[0].second--; break;
                case DOWN:  body[0].second++; break;
                default: break;
            }
        }
    }

    void grow() {   // increase the length of the snake
        body.push_back(body.back());  // it is an inbuilt function of std vector thats add a new segment at the last segment
    }

    bool collidesWithSelf() {  // collides with body
        for (int i = 1; i < body.size(); i++) { // this loop start from second segment
            if (body[0] == body[i]) return true;  // if head 0th segment same as the other segment return true
        }
        return false;
    }
};

class Board {// Boarders of the game
private:   // private access specifier
    int width, height; // width and height of board
public:
    Board(int w, int h) : width(w), height(h) {} //constructor to set the value of boards width and height

    void draw(Snake& snake, Food& food) { // allocate the snake and  food in drawer
        system("cls");
        for (int i = 0; i < width + 2; i++) cout << "-"; // top border
        cout << endl;

        for (int y = 0; y < height; y++) { // height of the loop
            for (int x = 0; x < width; x++) {  // width of the loop
                if (x == 0) cout << "|";   // left border of board
                if (x == food.x && y == food.y) {
                    cout << "x"; // food will be printed as 0
                } else {
                    bool printed = false;
                    for (const auto& part : snake.body) {
                        if (part.first == x && part.second == y) { // to match the current position of snake and food
                            cout << "O";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }
                if (x == width - 1) cout << "|"; //right side border
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "-";//bottom border
        cout << endl;
    }
};

class Game {  //Game class
private:
    int width, height;
    Snake snake; //it is an instance of snake class  representing the snake in the game
    Food food;  //it is an instance of food class  representing the snake in the game
    Board board; //it is an instance of board class  representing the snake in the game
    bool gameOver; // flag used for game over

public:
    Game(int w, int h) : width(w), height(h), snake(w / 2, h / 2), food(w, h), board(w, h) {
        gameOver = false;
        srand(time(0));
    }
    //width(w), height(h): Initializes the width and height of the game board.
//snake(w / 2, h / 2): Initializes the snake in the middle of the board.
//food(w, h): Initializes the food at a random position on the board.
//board(w, h): Initializes the board with the given width and height.
//gameOver = false: Sets the game over flag to false, indicating the game is not over.
//srand(time(0)): Seeds the random number generator to ensure different random numbers each time the game runs

    void input() {  // Functions to take input
        if (_kbhit()) {  // check if key
            switch (_getch()) { //check wich key is pressed
                case 'a': snake.setDirection(LEFT); break;
                case 'd': snake.setDirection(RIGHT); break;
                case 'w': snake.setDirection(UP); break;
                case 's': snake.setDirection(DOWN); break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {  //game logicwdwa
        snake.move();  //if the head of snake collides to border
        if (snake.body[0].first >= width || snake.body[0].first < 0 ||
            snake.body[0].second >= height || snake.body[0].second < 0 ||
            snake.collidesWithSelf()) {
            gameOver = true;      //this commands check the collicion
        }
        if (snake.body[0].first == food.x && snake.body[0].second == food.y) {
            snake.grow();
            food.generate(width, height); // for food
        }
    }

    void run() {  //main game loop
        while (!gameOver) {  // game run till the gameover
            board.draw(snake, food); //draw correct state boarder
            input();  // take the input
            logic();   // logic of the game
            std::this_thread::sleep_for(std::chrono::milliseconds(250)); // speed of the game
        }
        cout << "Game Over!" << endl;
    }
};

int main() {
    Game game(20, 20);  //Object name game
    game.run();  //void run and calls the board logic and inputs
    return 0;
}