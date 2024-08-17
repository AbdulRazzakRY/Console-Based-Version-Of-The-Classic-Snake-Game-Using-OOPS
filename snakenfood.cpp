#include <iostream>
#include <vector>
#include <conio.h>  // for _kbhit() and _getch()
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::milliseconds

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class Food {
public:
    int x, y;

    Food(int width, int height) {
        generate(width, height);
    }

    void generate(int width, int height) {
        x = rand() % width;
        y = rand() % height;
    }
};

class Snake {
public:
    vector<pair<int, int>> body;
    Direction dir;

    Snake(int startX, int startY) {
        body.push_back({startX, startY});
        dir = STOP;
    }

    void setDirection(Direction d) {
        dir = d;
    }

    void move() {
        if (dir != STOP) {
            for (int i = body.size() - 1; i > 0; i--) {
                body[i] = body[i - 1];
            }
            switch (dir) {
                case LEFT:  body[0].first--;  break;
                case RIGHT: body[0].first++;  break;
                case UP:    body[0].second--; break;
                case DOWN:  body[0].second++; break;
                default: break;
            }
        }
    }

    void grow() {
        body.push_back(body.back());
    }

    bool collidesWithSelf() {
        for (int i = 1; i < body.size(); i++) {
            if (body[0] == body[i]) return true;
        }
        return false;
    }
};

class Board {
private:
    int width, height;
public:
    Board(int w, int h) : width(w), height(h) {}

    void draw(Snake& snake, Food& food) {
        system("cls");  // For Windows use system("cls");
        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0) cout << "#";
                if (x == food.x && y == food.y) {
                    cout << "F";
                } else {
                    bool printed = false;
                    for (const auto& part : snake.body) {
                        if (part.first == x && part.second == y) {
                            cout << "O";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }
                if (x == width - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;
    }
};

class Game {
private:
    int width, height;
    Snake snake;
    Food food;
    Board board;
    bool gameOver;

public:
    Game(int w, int h) : width(w), height(h), snake(w / 2, h / 2), food(w, h), board(w, h) {
        gameOver = false;
        srand(time(0));
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': snake.setDirection(LEFT); break;
                case 'd': snake.setDirection(RIGHT); break;
                case 'w': snake.setDirection(UP); break;
                case 's': snake.setDirection(DOWN); break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        snake.move();
        if (snake.body[0].first >= width || snake.body[0].first < 0 ||
            snake.body[0].second >= height || snake.body[0].second < 0 ||
            snake.collidesWithSelf()) {
            gameOver = true;
        }
        if (snake.body[0].first == food.x && snake.body[0].second == food.y) {
            snake.grow();
            food.generate(width, height);
        }
    }

    void run() {
        while (!gameOver) {
            board.draw(snake, food);
            input();
            logic();
            std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Sleep for 100 milliseconds
        }
        cout << "Game Over!" << endl;
    }
};

int main() {
    Game game(20, 20);
    game.run();
    return 0;
}