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
#define BUZZER 6
#define LEDS 2
#define STD_SPD 100
#define ACT 100
int THRESHOLD_L = 500; ////////////////////////////////////
int THRESHOLD_R = 200;
int THRESHOLD_L2 = 500; ////////////////////////////////////
int THRESHOLD_R2 = 200;

Servo myservo;
SoftwareSerial bluetooth(RX, TX);

int sens_esq = 0, sens_dir = 0, dist = 0;
int x, y, z;

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
    pinMode(BUZZER, OUTPUT);
    pinMode(LEDS, OUTPUT);
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
    Serial.begin(9600);
    //myservo.write(30);
    delay(1000);
    myservo.write(120);
}


void calibragem(){
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    int minL = 1024;
    int minR = 1024;
    int maxL = 0;
    int maxR = 0;
    unsigned long t1 = millis();
    while (millis() < t1+10000){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        minL = min(sens_esq, minL);
        maxL = max(sens_esq,maxL);
        minR = min(sens_dir,minR);
        maxR = max(sens_dir,maxR);
        }

    
    THRESHOLD_L = (minL+maxL)/2;
    THRESHOLD_R = (minR+maxR)/2;
    Serial.print(THRESHOLD_L);
    Serial.print('\t');
    Serial.print(THRESHOLD_R);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    
}

void calibragem2(){
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    int minL = 1024;
    int minR = 1024;
    int maxL = 0;
    int maxR = 0;
    unsigned long t1 = millis();
    while (millis() < t1+10000){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        minL = min(sens_esq, minL);
        maxL = max(sens_esq,maxL);
        minR = min(sens_dir,minR);
        maxR = max(sens_dir,maxR);
        }

    
    THRESHOLD_L2 = (5*minL+maxL)/6;
    THRESHOLD_L2 = THRESHOLD_L2 - 100;
    THRESHOLD_R2 = (minR+maxR)/2;
    Serial.print(THRESHOLD_L2);
    Serial.print('\t');
    Serial.print(THRESHOLD_R2);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    
}


// the loop function runs over and over again forever
int leitura[6] = {255, 0, 0, 120, 0, 254} , motor_l, motor_r, angulo = 90;
long last_millis = 0, current_millis, apitando = 0;
void loop() {
    calibragem();
    delay(7000);
    calibragem2();
    //THRESHOLD_L2 = THRESHOLD_L + 100;

    for (int k =0 ; k<3 ; k++){
        digitalWrite(BUZZER, HIGH);
        digitalWrite(LEDS, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        digitalWrite(LEDS, LOW);
        delay(100);
    }

    delay(15000);
    setMotors(-50,-50);
    delay(3500);

    while (analogRead(IR_R) > THRESHOLD_R){
        // sens_esq = analogRead(IR_L);
        // if (sens_esq > THRESHOLD_L){
        //     setMotors(-50,-30);
        // }
        // else
        //     setMotors(-30,-50);

        }
    setMotors(90,-90);
    delay(1800);





    while(1){//rosa- teste 
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        Serial.print(sens_esq);
        Serial.print("\t");
        Serial.println(sens_dir);
        while(sens_dir > THRESHOLD_R){ //// direita no branco
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            Serial.print(sens_esq);
            Serial.print("\t");
            Serial.println(sens_dir);
            if (sens_esq < THRESHOLD_L){ /// esq no preto
                setMotors(200,160);
            } else {
                setMotors(160,200);
            }
        }setMotors(80,-60); // 80, -60 funcionava

        delay(1900);

        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);                
        /// direita no preto
        while(sens_esq > THRESHOLD_L2){/// esqurda nao chegou na linha
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            Serial.print(sens_esq);
            Serial.print("\t");
            Serial.println(sens_dir);
            if (sens_dir < THRESHOLD_R2){
                setMotors(70,20);
            }else{
                setMotors(20,70);
            }
        }
            setMotors(0,0);
            
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(3000);
        }
    
    while (1){

        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        //Serial.print(sens_esq);
        //Serial.print("\t");
        //Serial.println(sens_dir);
        if ((sens_esq < THRESHOLD_L) && (sens_dir > THRESHOLD_R)){//Linha na esquerda
            setMotors(STD_SPD - ACT, STD_SPD);
        } else if ((sens_esq > THRESHOLD_L) && (sens_dir < THRESHOLD_R)){
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
    int last_z = 120;
    last_millis = millis();
    while (1){
        if (readBT_pack(leitura)){         
            x = 2*(leitura[1]-127);
            y = 2*(leitura[2]-127);
            z = leitura[3];
            setMotors(constrain(y+x, -255, 255), constrain(y-x, -255, 255));
            if (y < 0){
                current_millis = millis();
                if ((apitando == 0) && (current_millis - last_millis > 500)){
                    last_millis = millis();
                    apitando = 1;
                    digitalWrite(LEDS, HIGH);
                    digitalWrite(BUZZER, HIGH);
                } else if ((apitando == 1) && (current_millis - last_millis > 500)) {
                    last_millis = millis();
                    apitando = 0;
                    digitalWrite(LEDS, LOW);
                    digitalWrite(BUZZER, LOW);
                }
            } else {
                apitando = 0;
                digitalWrite(LEDS, LOW);
                digitalWrite(BUZZER, LOW);
            }
            if (z != last_z){
                myservo.write(z);
                last_z = z;
            }            
       }
    }
    
}


