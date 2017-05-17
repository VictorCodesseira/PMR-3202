#include "auxiliar.h"
#include "Carrinho.h"
#include <Arduino.h>
#include <SoftPWM.h>

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
