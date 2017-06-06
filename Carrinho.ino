#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

void setMotors(int motor_left, int motor_right);
int readBT_pack(int leitura[6]);

#define MOTOR_L 9
#define MOTOR_L_DIR 10
#define MOTOR_R 7
#define MOTOR_R_DIR 8
#define SERVO 3
#define IR_L A0
#define IR_R A1
#define RX 11
#define TX 12

#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

Servo myservo;
SoftwareSerial bluetooth(RX, TX);

int sens_esq = 0, sens_dir = 0, dist = 0;
int x, y, z;
int autonomo = 1, controlado = 1;

void setMotors(int motor_left, int motor_right){
    SoftPWMSet(MOTOR_L,(abs(motor_left)));
    if (motor_left < 0) {
        digitalWrite(MOTOR_L_DIR, HIGH);
    } else {
        digitalWrite(MOTOR_L_DIR, LOW);
    }
    SoftPWMSet(MOTOR_R, (abs(motor_right)));
    if (motor_right < 0) {
        digitalWrite(MOTOR_R_DIR, HIGH);
    } else {
        digitalWrite(MOTOR_R_DIR, LOW);
    }
}

int readBT_pack(int leitura[6]){
    if (!(bluetooth.available())) return 0;
    int c = bluetooth.read();
    if (c != 255) return 0;
    for (int i = 1; i<6 ; i++){
        while(!(bluetooth.available()));
        leitura[i] = bluetooth.read();
    }
    if (leitura[5] != 254) return 0;
    return 1;
}

void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_L_DIR, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(MOTOR_R_DIR, OUTPUT);
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);

    SoftPWMBegin();
    SoftPWMSet(MOTOR_L, 0);
    SoftPWMSet(MOTOR_R, 0);

    myservo.attach(SERVO);
    bluetooth.begin(9600);
}

// the loop function runs over and over again forever
int leitura[6] = {255, 0, 0, 84, 0, 254} , motor_l, motor_r, angulo = 90;
void loop() {
    while (1 == 0){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        if ((sens_esq > THRESHOLD) && (sens_dir < THRESHOLD)){//Linha na esquerda
            setMotors(STD_SPD - ACT, STD_SPD);
        } else if ((sens_esq < THRESHOLD) && (sens_dir > THRESHOLD)){
            setMotors(STD_SPD, STD_SPD - ACT);
        } else {
            setMotors(STD_SPD, STD_SPD);
        }
        if (readBT_pack(leitura)){
            if (leitura[4] == 1) break;
        } 
    }

    setMotors(0,0);
    delay(1000);
    int last_z = 84;
    while (controlado == 1){
        if (readBT_pack(leitura)){         
            x = 2*(leitura[1]-127);
            y = 2*(leitura[2]-127);
            z = leitura[3];
            setMotors(constrain(y+x, -255, 255), constrain(y-x, -255, 255));
            if (z != last_z){
                myservo.write(z);
                last_z = z;
            }            
       }
    }
}
