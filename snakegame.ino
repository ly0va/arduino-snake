#define IRRECV  2
#define xSER    3
#define xRCLK   4
#define xSRCLK  5
#define ySER    6
#define yRCLK   7
#define ySRCLK  8
#define BUZZER  9
#define dSER    10
#define dRCLK   11
#define dSRCLK  12

#include <IRremote.h>
#include "snake.hpp"

IRrecv irrecv(IRRECV);
decode_results results;
Snake snake;
short tick = 200;
unsigned long lastTick = 0;
const byte digits[] = {0xBB, 0x88, 0xB5, 0x9D, 0x8E, 
                       0x1F, 0x3F, 0x89, 0xBF, 0x9F};
byte matrix[8] = {};
const byte sadFace[] = {
    0b00000000,
    0b00000110,
    0b01100110,
    0b00110000,
    0b00110000,
    0b01100110,
    0b00000110,
    0b00000000,
};
const byte happyFace[] = {
    0b00000000,
    0b00100110,
    0b01100110,
    0b01000000,
    0b01000000,
    0b01100110,
    0b00100110,
    0b00000000,
};

void shiftBit(byte ser, byte rclk, byte srclk, byte b) {
    digitalWrite(rclk, LOW);
    digitalWrite(srclk, LOW);
    digitalWrite(ser, b ? HIGH : LOW);
    digitalWrite(srclk, HIGH);
    digitalWrite(rclk, HIGH);
}

void shiftByte(byte ser, byte rclk, byte srclk, byte b) {
    digitalWrite(rclk, LOW);
    for (byte bit = 0; bit < 8; bit++) {
        digitalWrite(srclk, LOW);
        digitalWrite(ser, (b & 1) ? HIGH : LOW);
        digitalWrite(srclk, HIGH);
        b >>= 1;
    }
    digitalWrite(rclk, HIGH);
}

inline void display(byte x) {
    shiftByte(dSER, dRCLK, dSRCLK, digits[x]);
}

void draw(byte *buffer) {
    shiftByte(xSER, xRCLK, xSRCLK, 0b01111111);
    for (byte row = 0; row < 8; row++) {
        byte line = buffer[row];
        shiftByte(ySER, yRCLK, ySRCLK, line);
        delay(analogRead(5)/10);
        shiftByte(ySER, yRCLK, ySRCLK, 0);
        shiftBit(xSER, xRCLK, xSRCLK, 1);
    }
}

void onRecv() {
    if (irrecv.decode(&results)) {
        switch (results.value) {
            case 0xFFA857: snake.turn(0); break; // down
            case 0xFF22DD: snake.turn(1); break; // left
            case 0xFF629D: snake.turn(2); break; // up
            case 0xFFC23D: snake.turn(3); break; // right
        }
        irrecv.resume();
    }
}

void setup() {
    for (byte i = 3; i < 13; i++) {
        pinMode(i, OUTPUT);
    }
    pinMode(2, INPUT_PULLUP);
    irrecv.enableIRIn();
    attachInterrupt(digitalPinToInterrupt(IRRECV), onRecv, CHANGE);
    randomSeed(analogRead(0));
    snake = Snake();
}

void loop() {
    if (snake.gameOver) {
        draw(sadFace);
        digitalWrite(BUZZER, LOW);
        return;
    }
    if (snake.score == 9) {
        draw(happyFace);
        digitalWrite(BUZZER, LOW);
        return;
    }
    if (snake.apple.x == snake.tail.head().x &&
        snake.apple.y == snake.tail.head().y) {
            digitalWrite(BUZZER, HIGH);
    } else {
            digitalWrite(BUZZER, LOW);
    }
    if (millis() - lastTick >= tick) {
        snake.advance();
        lastTick = millis();
        snake.carve(matrix);
    }
    draw(matrix);
    display(snake.score % 10);
}
