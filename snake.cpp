#include "snake.hpp"

Snake::Snake() {
    vx = 1;
    vy = 0;
    score = 0;
    gameOver = false;
    byte x = random(0, 7);
    byte y = random(0, 8);
    tail.push({x, y});
    tail.push({x+1, y});
    putApple();
}

void Snake::advance() {
    bool eaten = false;
    Point &head = tail.head();
    if (head.x == apple.x && head.y == apple.y) {
        score++;
        eaten = true;
    }
    if (!eaten) tail.pop();
    byte newX = (head.x + vx + 8) % 8;
    byte newY = (head.y + vy + 8) % 8;
    if (checkIntersection(newX, newY)) {
        gameOver = true;
        return;
    }
    tail.push({newX, newY});
    if (eaten) putApple();
}

void Snake::carve(byte *matrix) {
    memset(matrix, 0, 8);
    for (byte i = 0; i < tail.size(); i++) {
        matrix[tail[i].x] |= (1 << tail[i].y);
    }
    matrix[apple.x] |= (1 << apple.y);
}

void Snake::turn(byte dir) {
    if (vy == 0) { // horizontal movement
        if (dir == 0) { // down
            vy = 1;
            vx = 0;
        } else if (dir == 2) { // up
            vy = -1;
            vx = 0;
        }
    } else {  // vertical movement
        if (dir == 1) { // left
            vx = 1;
            vy = 0;
        } else if (dir == 3) { // right
            vx = -1;
            vy = 0;
        }
    }
}

void Snake::putApple() {
    bool collision;
    byte x, y;
    do {
        collision = false;
        x = random(0, 8);
        y = random(0, 8);
        for (byte i = 0; i < tail.size(); i++) {
            if (tail[i].x == x && tail[i].y == y) {
                collision = true;
                break;
            }
        }
    } while (collision);
    apple = {x, y};
}

bool Snake::checkIntersection(byte x, byte y) {
    for (byte i = 0; i < tail.size(); i++) {
        if (tail[i].x == x && tail[i].y == y) {
            return true;
        }
    }
    return false;
}
