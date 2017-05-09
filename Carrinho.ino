#include "Carrinho.h"
#include "Auxiliar.h"
#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

Servo myservo;
SoftwareSerial bluetooth(RX, TX);
int sens_esq = 0, sens_dir = 0, dist = 0, leitura = 0;
int autonomo = 1, controlado = 1;

void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_L_DIR, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(MOTOR_R_DIR, OUTPUT);
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    SoftPWMBegin();
    SoftPWMSet(MOTOR_R, 0);

    myservo.attach(SERVO);

    bluetooth.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
    while (autonomo == 1){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        dist = readUS();
        if ((sens_esq > THRESHOLD) && (sens_dir < THRESHOLD)){//Linha na esquerda
            setMotors(STD_SPD - ACT, STD_SPD);
        } else if ((sens_esq < THRESHOLD) && (sens_dir > THRESHOLD)){
            setMotors(STD_SPD, STD_SPD - ACT);
        } else {
            setMotors(STD_SPD, STD_SPD);
        }
        if ((bluetooth.available()) && (bluetooth.read() == BREAK)) {
            autonomo = 0;
        }
    }

    setMotors(0,0);
    delay(1000);

    while (controlado == 1){
        if (bluetooth.available()){
            leitura = bluetooth.read();
        }
        switch (leitura){
            case UP:
                setMotors(STD_SPD, STD_SPD);
                break;
            case RIGHT:
                setMotors(STD_SPD, -STD_SPD);
                break;
            case DOWN:
                setMotors(-STD_SPD, -STD_SPD);
                break;
            case LEFT:
                setMotors(-STD_SPD, STD_SPD);
                break;
            case OPEN:
                myservo.write(180);
                break;
            case CLOSE:
                myservo.write(0);
            case BREAK:
                controlado = 0;
                break;
            default:
                setMotors(0, 0);
        }
    }
}
