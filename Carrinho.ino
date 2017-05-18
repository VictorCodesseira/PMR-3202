#include "Auxiliar.h"
#include "Carrinho.h"
#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

Servo myservo;

int sens_esq = 0, sens_dir = 0, dist = 0;
int x, y, z, luz, buzina;
int autonomo = 1, controlado = 1;
int pack[9] = {255, 9, 127, 127, 0, 0, 0, 254, 254};

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

    initBT();
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
        if (readBT() == BREAK) autonomo = 0;
    }

    setMotors(0,0);
    delay(1000);

    while (controlado == 1){
        if (readBT_pack(pack)!=0){
            x = 2*(pack[2]-127);
            y = 2*(pack[3]-127);
            z = pack[4];
            luz = pack[5];
            buzina = pack[6];
            setMotors(constrain(y+x, -255, 255), constrain(y-x, -255, 255));
            myservo.write(map(z, 0, 255, 0, 180));
        }
    }
}
