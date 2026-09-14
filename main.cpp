#include <deque>
#include <ncurses.h>
#include <random>

struct Position {
    int row;
    int col;

    bool operator== (const Position&a) const {
        return this->col == a.col && this->row == a.row;
    }
};

enum class Direction {
    Wrong,
    Up,
    Down,
    Left,
    Right,
};

Position advance(Position p, Direction d) {
    switch (d) {
        case Direction::Up:
        {
            return {.row = p.row - 1, .col = p.col };
        }
        case Direction::Down:
        {
            return { .row = p.row + 1, .col = p.col};
        }
        case Direction::Right:
        {
            return { .row = p.row, .col = p.col + 1};
        }
        case Direction::Left:
        {
            return { .row = p.row, .col = p.col - 1};
        }
        case Direction::Wrong:
          return { .row = 0, .col = 0 };
        }
}


void moveSnakeTo(const Position& p, std::deque<Position>& body, Direction d, bool grew) {

    body.push_front(p);

    if (!grew) {
        body.pop_back();
    }
}

Direction directionFromKey(int key) {
    switch(key) {
        case KEY_UP:
            return Direction::Up;
        case KEY_DOWN:
            return Direction::Down;
        case KEY_RIGHT:
            return Direction::Right;
        case KEY_LEFT:
            return Direction::Left;
        default:
            return Direction::Wrong;
    }
}

void drawGameBoardAndSnakeAndFood(WINDOW* w, const std::deque<Position>& snakeBody, const Position& food) {
    werase(w);
    box(w, 0, 0);

    for(const auto& segment: snakeBody) {
        mvwaddch(w, segment.row, segment.col, '=');
    }

    mvwaddch(w, food.row, food.col, '*');

    wrefresh(w);
}

void drawScoreBoard(WINDOW* w, int score) {
    werase(w);
    box(w, 0, 0);

    int col = 2, row = 2;
    mvwprintw(w, row, col, "Score: %d", score);

    wrefresh(w);
}

bool hitWall(Position head, int height, int width) {
    return head.row <= 0 || head.row >= height - 1 || head.col <= 0 || head.col >= width - 1;
}

Position randomFoodPosition(std::deque<Position>& snakeBody, int height, int width, std::mt19937& rng) {
    std::uniform_int_distribution<int> rowDist(1, height - 2);
    std::uniform_int_distribution<int> colDist(1, width - 2);

    Position candidate;
    do {
        candidate = { rowDist(rng), colDist(rng) };
    } while (std::find(snakeBody.begin(), snakeBody.end(), candidate) != snakeBody.end());

    return candidate;
}

int main(int argc, char** argv) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int score = 0;

    printw("Welcome to the Best SNAKE game ever...");

    std::mt19937 rng(std::random_device{}());

    int GAMEBOARD_WIDTH = 20;
    int GAMEBOARD_HEIGHT = 30;
    int SCOREBOARD_WIDTH = 20;
    int SCOREBOARD_HEIGHT = 5;
    int GAMEBOARD_BEGIN_X = 2;
    int GAMEBOARD_BEGIN_Y = 2;
    int SCOREBOARD_BEGIN_X = 2;

    int SCOREBOARD_BEGIN_Y = GAMEBOARD_BEGIN_Y + GAMEBOARD_HEIGHT;
    WINDOW* game = newwin(GAMEBOARD_HEIGHT, GAMEBOARD_WIDTH, GAMEBOARD_BEGIN_Y, GAMEBOARD_BEGIN_X);
    WINDOW* scoreBoard = newwin(SCOREBOARD_HEIGHT, SCOREBOARD_WIDTH, SCOREBOARD_BEGIN_Y, SCOREBOARD_BEGIN_X);


    std::deque<Position> snakeBody = {
        Position {.row = 3, .col = 3},
    };

    Position food { .row = 10, .col = 10 };

    refresh();

    drawScoreBoard(scoreBoard, score);

    drawGameBoardAndSnakeAndFood(game, snakeBody, food);

    while(true) {
        int ch = getch();

        if (ch != ERR) {
            if (ch == 'q') {
                break;
            }
            else {
                Direction direction = directionFromKey(ch);

                if (direction == Direction::Wrong) {
                    continue;
                }

                Position prospectiveHead = advance(snakeBody.front(), direction);

                if (hitWall(prospectiveHead, GAMEBOARD_HEIGHT, GAMEBOARD_WIDTH)){
                    continue;
                }

                bool eatenTheFood = false;
                if (prospectiveHead == food) {
                    eatenTheFood = true;
                }

                moveSnakeTo(prospectiveHead, snakeBody, direction, eatenTheFood);

                if (eatenTheFood) {
                    score++;
                    food = randomFoodPosition(snakeBody, GAMEBOARD_HEIGHT, GAMEBOARD_WIDTH, rng);
                }

                drawGameBoardAndSnakeAndFood(game, snakeBody, food);
                drawScoreBoard(scoreBoard, score);
            }
        }
    }

    getch();
    endwin();
    return 0;
}
