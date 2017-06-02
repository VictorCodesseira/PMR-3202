#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

void initBT();
void setMotors(int motor_left, int motor_right);
long readUS();
int readBT_pack(int leitura[5]);
int readBT();

#define MOTOR_L 9
#define MOTOR_L_DIR 10
#define MOTOR_R 7
#define MOTOR_R_DIR 8
#define SERVO 3
#define IR_L A0
#define IR_R A1
#define TRIG 4
#define ECHO 5
#define RX 11
#define TX 12

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define OPEN 4
#define CLOSE 5
#define BREAK 6
#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

Servo myservo;
SoftwareSerial bluetooth(RX, TX);

int sens_esq = 0, sens_dir = 0, dist = 0;
int x, y, z, luz, buzina;
int autonomo = 1, controlado = 1;


void initBT(){
    bluetooth.begin(9600);
}

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

int readBT_pack(int leitura[5]){
    if (!(bluetooth.available())) return 0;
    int c = bluetooth.read();
    if (c != 255) return 0;
    for (int i = 1; i<5 ; i++){
        while(!(bluetooth.available()));
        leitura[i] = bluetooth.read();
    }
    if (leitura[4] != 254) return 0;
    return 1;
}

int readBT(){
    if(!(bluetooth.available())) return 0;
    return bluetooth.read();
}

void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_L_DIR, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(MOTOR_R_DIR, OUTPUT);
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(13, OUTPUT);
    
    digitalWrite(13, HIGH);

    SoftPWMBegin();
    SoftPWMSet(MOTOR_L, 0);
    SoftPWMSet(MOTOR_R, 0);
    Serial.begin(9600);
    myservo.attach(SERVO);
    initBT();

    digitalWrite(13, LOW);
}

// the loop function runs over and over again forever
int leitura[5] = {255, 0, 0, 84, 254} , motor_l, motor_r, angulo = 90;
void loop() {
    /*
    if (bluetooth.available()){ //batatapython
        leitura = bluetooth.read();
        switch (leitura){
            case 'r':
                motor_l = 0;
                motor_r = 0;
                break;
            case 'w':
                motor_l = 255;
                motor_r = 255;
                break;
            case 's':
                motor_l = -255;
                motor_r = -255;
                break;
            case 'a':
                motor_l = -255;
                motor_r = 255;
                break;
            case 'd':
                motor_l = 255;
                motor_r = -255;
                break;
            case 'q':
                angulo = 60;
                break;
            case 'e':
                angulo = 120;
                break;
        }
        angulo = constrain(angulo, 0, 180);
        setMotors(motor_l, motor_r);
        myservo.write(angulo);
        Serial.print("Motor_l: ");
        Serial.print(motor_l);
        Serial.print(" Motor_r: ");
        Serial.print(motor_r);
        Serial.print(" Angulo: ");
        Serial.println(angulo);
        }
        */
    // while (autonomo == 1){
    //     sens_esq = analogRead(IR_L);
    //     sens_dir = analogRead(IR_R);
    //     dist = readUS();
    //     if ((sens_esq > THRESHOLD) && (sens_dir < THRESHOLD)){//Linha na esquerda
    //         setMotors(STD_SPD - ACT, STD_SPD);
    //     } else if ((sens_esq < THRESHOLD) && (sens_dir > THRESHOLD)){
    //         setMotors(STD_SPD, STD_SPD - ACT);
    //     } else {
    //         setMotors(STD_SPD, STD_SPD);
    //     }
    //     if (readBT() == BREAK) autonomo = 0;
    // }

    // setMotors(0,0);
    // delay(1000);
    int last_z = 84;
    while (controlado == 1){
        if (readBT_pack(leitura)){         
            x = 2*(leitura[1]-127);
            y = 2*(leitura[2]-127);
            z = leitura[3];
            setMotors(constrain(y+x, -255, 255), constrain(y-x, -255, 255));
            Serial.println(z);
            if (z != last_z){
                myservo.write(z);
                last_z = z;
            }            
       }
    }
}
