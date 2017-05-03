#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

#define MOTOR_L 9
#define MOTOR_L_DIR 10
#define MOTOR_R 7
#define MOTOR_R_DIR 8
#define SERVO 3
#define IR_L A0
#define IR_R A1
#define TRIG 4
#define ECHO 5
#define RX 0
#define TX 1

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define OPEN 4
#define CLOSE 5
#define BREAK 6
#define NONE 10

#define MOTOR_L 0
#define MOTOR_R 1

#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

void setMotors(int motor_left, int motor_right);
int readUS();

Servo myservo;
SoftwareSerial bluetooth(RX, TX);
int sens_esq = 0, sens_dir = 0, dist = 0, leitura = 0;
int autonomo = 1, controlado = 1;

void setMotors(int motor_left, int motor_right){
    analogWrite(MOTOR_L, abs(motor_left));
    if (motor_left < 0) {
        analogWrite(MOTOR_L_DIR, LOW);
    } else {
        analogWrite(MOTOR_L_DIR, HIGH);
    }
    SoftPWMSetPercent(MOTOR_R, (abs(motor_right)*100)/255);
    if (motor_right < 0) {
        analogWrite(MOTOR_R_DIR, LOW);
    } else {
        analogWrite(MOTOR_R_DIR, HIGH);
    }
}

int readUS(){
    long duration, cm;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH, 30);

    cm = (duration/2) / 29.1;

    return cm;
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
        } else {
            leitura = NONE;
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
