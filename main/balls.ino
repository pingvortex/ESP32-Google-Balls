#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>

#define SCREEN_W 128
#define SCREEN_H 64
#define BALL_QTY 65

Adafruit_SSD1306 oled(SCREEN_W, SCREEN_H, &Wire, -1);
Adafruit_MPU6050 imu;

struct Dot {
    float x, y;
    float ox, oy; // home pos
    float tx, ty;
    float vx, vy;
    float base_r;
    
    float air_drag = 0.82; 
    float tension = 0.15;

    void tick() {
        vx += (tx - x) * tension;
        vy += (ty - y) * tension;
        
        vx *= air_drag;
        vy *= air_drag;
        
        x += vx;
        y += vy;
    }
};

Dot dots[BALL_QTY];

// {x, y, radius}
const float raw_logo[65][3] = {
    {202,78,9}, {348,83,9}, {256,69,9}, {214,59,9}, {265,36,9}, {300,78,9}, {294,59,9}, {45,88,9}, {268,52,9}, {73,83,9}, 
    {294,6,9}, {235,62,9}, {353,42,8}, {336,52,8}, {208,41,8}, {321,70,8}, {8,60,8}, {180,81,8}, {146,65,8}, {145,49,8}, 
    {246,34,8}, {169,69,8}, {273,99,8}, {248,120,8}, {294,41,8}, {267,114,8}, {78,67,8}, {294,23,8}, {117,83,8}, {137,80,8}, 
    {14,71,8}, {331,80,8}, {25,82,8}, {233,46,8}, {73,13,8}, {327,35,6}, {319,46,6}, {256,81,6}, {244,88,6}, {194,32,6}, 
    {97,56,6}, {105,75,6}, {42,4,6}, {10,27,6}, {166,55,6}, {266,88,6}, {178,34,6}, {100,65,6}, {343,32,6}, {59,5,6}, 
    {27,9,6}, {233,116,6}, {123,32,6}, {6,38,6}, {63,62,6}, {6,49,6}, {108,36,6}, {169,43,6}, {137,37,6}, {318,58,6}, 
    {226,100,5}, {101,46,5}, {226,108,5}, {17,17,5}, {232,93,5}
};

void setup() {
    Serial.begin(115200);

    if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("oled failed");
        for(;;);
    }

    if (!imu.begin()) {
        Serial.println("mpu6050 failed");
        oled.clearDisplay();
        oled.setCursor(0,0);
        oled.print("IMU Error!");
        oled.display();
        while (1) yield();
    }

    float sc = 0.32; 
    float shiftX = 8; 
    float shiftY = 12;

    for (int i = 0; i < BALL_QTY; i++) {
        dots[i].ox = (raw_logo[i][0] * sc) + shiftX;
        dots[i].oy = (raw_logo[i][1] * sc) + shiftY;
        dots[i].x = dots[i].ox;
        dots[i].y = dots[i].oy;
        dots[i].tx = dots[i].ox;
        dots[i].ty = dots[i].oy;
        dots[i].base_r = raw_logo[i][2] * sc;
        if(dots[i].base_r < 1) dots[i].base_r = 1;
    }
}

void loop() {
    sensors_event_t a, g, t;
    imu.getEvent(&a, &g, &t);

    float mx = map(a.acceleration.x, -7, 7, 0, SCREEN_W);
    float my = map(a.acceleration.y, -7, 7, 0, SCREEN_H);

    oled.clearDisplay();

    for (int i = 0; i < BALL_QTY; i++) {
        float dx = mx - dots[i].x;
        float dy = my - dots[i].y;
        float d2 = (dx * dx) + (dy * dy);

        if (d2 < 600) { 
            dots[i].tx = dots[i].x - dx;
            dots[i].ty = dots[i].y - dy;
        } else {
            dots[i].tx = dots[i].ox;
            dots[i].ty = dots[i].oy;
        }

        dots[i].tick();
        oled.fillCircle((int)dots[i].x, (int)dots[i].y, (int)dots[i].base_r, WHITE);
    }

    oled.display();
}
