#include "auxiliar.h"
#include "Carrinho.h"
#include <Arduino.h>
#include <SoftPWM.h>

SoftwareSerial bluetooth(RX, TX);

void initBT(){
    bluetooth.begin(9600);
}

void setMotors(int motor_left, int motor_right){
    analogWrite(MOTOR_L, abs(motor_left));
    if (motor_left < 0) {
        digitalWrite(MOTOR_L_DIR, LOW);
    } else {
        digitalWrite(MOTOR_L_DIR, HIGH);
    }
    SoftPWMSetPercent(MOTOR_R, (abs(motor_right)*100)/255);
    if (motor_right < 0) {
        digitalWrite(MOTOR_R_DIR, LOW);
    } else {
        digitalWrite(MOTOR_R_DIR, HIGH);
    }
}

long readUS(){
    long duration, cm;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);

    cm = (duration/2) / 29.1;

    return cm;
}

int readBT_pack(int leitura[9]){
    if (!(bluetooth.available())) return 0;
    int c = bluetooth.read();
    if (c != 255) return 0;
    while (!(bluetooth.available()));
    int length = bluetooth.read();
    for (int i = 1; length; i++){
        while(!(bluetooth.available()));
        leitura[i] = bluetooth.read();
    }
    if (leitura[8] != 254) return 0;
    int soma = leitura[2] + leitura[3] + leitura[4] + leitura[5] + leitura[6];
    if (leitura[7] != soma) return 0;
    return 1;
}

int readBT(){
    if(!(bluetooth.available())) return 0;
    return bluetooth.read();
}
