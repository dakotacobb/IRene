/**
 * RemoteIR library - Test program.
 *
 * Copyright (C) 2010 Shinichiro Nakamura (CuBeatSystems)
 * http://shinta.main.jp/
 */

#include <mbed.h>

#include "ReceiverIR.h"
#include "TransmitterIR.h"
#include "Motor.h"
#include "rtos.h"
#include "LSM9DS1.h"
#include "uLCD_4DGL.h"

#define TEST_LOOP_BACK  0

Serial pc(USBTX, USBRX);
ReceiverIR ir_rx(p15);
//DigitalOut myled(LED1);
Motor mRight(p23, p6, p5); // pwm, fwd, rev
Motor mLeft(p22,p8,p7);
PwmOut myled(p21);
uLCD_4DGL uLCD(p28,p27,p30);
float speed = 0.5;

Thread accel;
Thread lcd;


int mode = 0;


void crash() {
    //LSM9DS1 lol(p9, p10, 0x6B, 0x1E);
    LSM9DS1 lol(p9, p10, 0xD6, 0x3C);
    lol.begin();
    if (!lol.begin()) {
        pc.printf("Failed to communicate with LSM9DS1.\n");
    }
    lol.calibrate();
    while(1) {
        lol.readAccel();  
        pc.printf("accel: %d %d %d\n\r", lol.ax, lol.ay, lol.az);
        if(mode == 2) {
            myled = 1;
            wait(1);
            }
        myled = 0;
        wait(0.1);
    }
}

void forward() {
    mRight.speed(speed); 
    mLeft.speed(speed); 
    mode = 1;
    uLCD.cls();
    uLCD.locate(1,2);
    uLCD.printf("Moving forward");
    }
void stop() {
    mRight.speed(0); 
    mLeft.speed(0); 
    mode = 0;
    uLCD.cls();
    uLCD.locate(1,2);
    uLCD.printf("Stopped");
    }
void reverse() {
    mRight.speed(-speed); 
    mLeft.speed(-speed); 
    mode = 2;
    uLCD.cls();
    uLCD.locate(1,2);
    uLCD.printf("Moving backwards");
    }
void CW() {
    mRight.speed(-speed); 
    mLeft.speed(speed); 
    mode = 3;
    uLCD.cls();
    uLCD.locate(1,2);
    uLCD.printf("Turning Right");
    }
void CCW() {
    mRight.speed(speed); 
    mLeft.speed(-speed); 
    mode = 4;
    uLCD.cls();
    uLCD.locate(1,2);
    uLCD.printf("Turning Left");
    }

int receive(RemoteIR::Format *format, uint8_t *buf, int bufsiz, int timeout = 100) {
    int cnt = 0;
    while (ir_rx.getState() != ReceiverIR::Received) {
        cnt++;
        if (timeout < cnt) {
            return -1;
        }
    }
    return ir_rx.getData(format, buf, bufsiz * 8);
}




int main(void) {
    LSM9DS1 lol(p9, p10, 0xD6, 0x3C);
    lol.begin();
    accel.start(crash);
//0 is stop, 1 is fwd, 2 is back, 3 is clockwise, 4 is CCW
    pc.printf("RemoteIR        ");
    pc.printf("Program example.");
    wait(2);

    pc.printf("Press a button  ");
    pc.printf("on a controller.");

    /*
     * Execute.
     */
    while (1) {
        uint8_t buf1[32];
        uint8_t buf2[32];
        int bitlength1;
        int bitlength2;
        RemoteIR::Format format;

        memset(buf1, 0x00, sizeof(buf1));
        memset(buf2, 0x00, sizeof(buf2));

        {
            bitlength1 = receive(&format, buf1, sizeof(buf1));
            if (bitlength1 < 0) {
                continue;
            }
        }
        
        
        pc.printf("%02X", buf1[3]);
        if (buf1[3] == 0x66 && mode != 1) {
            forward();
            }
        else if ((buf1[3] == 0xD5 || buf1[3] == 0x55) && mode != 0) {
            stop();
            pc.printf("%02X", buf1[3]);
            }
        else if (buf1[3] == 0xCC && mode != 2) {
            reverse();
            }
        else if (buf1[3] == 0x52 && mode != 3) {
            CW();
            }
        else if (((buf1[3] == 0x61) || (buf1[3] == 0xE1)) && mode != 4) {
            CCW();
            }
    }
}

