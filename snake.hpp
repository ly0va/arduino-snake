#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_
#include <Arduino.h>

struct Point {
    byte x, y;
};

template <typename T, size_t N>
struct Queue {
    T buffer[N];
    size_t front, back;
    inline const T& pop() { return buffer[(front++) % N]; }
    inline void push(const T& data) { buffer[(back++) % N] = data; }
    inline T& operator[](size_t i) { return buffer[(front+i) % N]; }
    inline size_t size() { return (back-front+N) % N; }
    inline T& head() { return buffer[(back+N-1) % N]; }
    Queue() { front = back = 0; }
};

struct Snake {
    byte vx, vy;
    byte score;
    Queue<Point, 64> tail;
    Point apple;
    bool gameOver;
    Snake();
    void advance();
    void turn(byte dir);
    void putApple();
    bool checkIntersection(byte x, byte y);
    void carve(byte *matrix);
};

#endif
