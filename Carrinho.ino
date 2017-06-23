#include <Servo.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>

void setMotors(int motor_left, int motor_right);
int readBT_pack(int leitura[7]);

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
int THRESHOLD_L = 500;
int THRESHOLD_R = 200;
int THRESHOLD_L2 = 500;
int THRESHOLD_R2 = 200;
int rosa = 0, azul = 0;
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

int readBT_pack(int leitura[7]){
    if (!(bluetooth.available())) return 0;
    int c = bluetooth.read();
    if (c != 255) return 0;
    for (int i = 1; i<6 ; i++){
        while(!(bluetooth.available()));
        leitura[i] = bluetooth.read();
    }
    if (leitura[6] != 254) return 0;
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
    while (millis() < t1+5000){
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
    while (millis() < t1+5000){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        minL = min(sens_esq, minL);
        maxL = max(sens_esq,maxL);
        minR = min(sens_dir,minR);
        maxR = max(sens_dir,maxR);
        }

    
    THRESHOLD_L2 = (5*minL+maxL)/6;
    THRESHOLD_L2 = THRESHOLD_L2 - 100;
    THRESHOLD_R2 = (5*minR+maxR)/2;
    THRESHOLD_R2 = THRESHOLD_R2 - 100;
    Serial.print(THRESHOLD_L2);
    Serial.print('\t');
    Serial.print(THRESHOLD_R2);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    
}


// the loop function runs over and over again forever
int leitura[7] = {255, 0, 0, 120, 0, 0, 254} , motor_l, motor_r, angulo = 90;
long last_millis = 0, current_millis, apitando = 0;
void loop() {
    calibragem();
    delay(5000);
    calibragem2();

    for (int k =0 ; k<3 ; k++){
        digitalWrite(BUZZER, HIGH);
        digitalWrite(LEDS, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        digitalWrite(LEDS, LOW);
        delay(100);
    }
    int sair = 0;
    while(sair == 0){
        if (readBT_pack(leitura)){
            if (leitura[4] == 1) {
                azul = 1;
                sair = 1;
            } else if (leitura[5] == 1) {
                rosa = 1;
                sair = 1;
            } 
        }
    }

    unsigned long start_time = millis();
    while (millis() - start_time < 30000){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        if (rosa == 1){//rosa- teste 
            setMotors(-50,-50);
            delay(3500);
            while (analogRead(IR_R) > THRESHOLD_R){}
            setMotors(90,-90);
            delay(1800);
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            myservo.write(40);
            while(sens_dir > THRESHOLD_R){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_esq < THRESHOLD_L){
                    setMotors(150,100);
                } else {
                    setMotors(100,150);
                }
            }setMotors(60,-70);
            myservo.write(120);

            delay(2500);
                            
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            unsigned long t2=millis();
            while(millis() < t2 + 4000){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_dir < THRESHOLD_R){
                    setMotors(70,20);
                }else{
                    setMotors(35,60);
                }
            }

            while((sens_esq > THRESHOLD_L2)&& (millis() < start_time + 29000)){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_dir < THRESHOLD_R){
                    setMotors(70,20);
                }else{
                    setMotors(20,70);
                }
            }
            setMotors(0,0);
        } else if (azul == 1){
            setMotors(-50,-54);
            delay(3500);
            while (analogRead(IR_L) > THRESHOLD_L){}
            setMotors(-90,90);
            delay(1800); 
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            myservo.write(40);
            while(sens_esq > THRESHOLD_L){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_dir < THRESHOLD_R){
                    setMotors(100,150);
                } else {
                    setMotors(150,100);
                }
            }setMotors(-60,60);
            myservo.write(120);
            delay(2300);
                            
            /// direita no preto
            sens_esq = analogRead(IR_L);
            sens_dir = analogRead(IR_R);
            unsigned long t2=millis();
            while(millis() < t2 + 4000){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_esq < THRESHOLD_L2){
                    setMotors(70,30);
                }else{
                    setMotors(30,70);
                }
            }
            while((sens_dir > THRESHOLD_R2) && (millis() < start_time + 29000)){
                sens_esq = analogRead(IR_L);
                sens_dir = analogRead(IR_R);
                if (sens_esq < THRESHOLD_L2){
                    setMotors(70,30);
                }else{
                    setMotors(30,70);
                }
            }
            setMotors(0,0);
        }
        rosa = 0;
        azul = 0;
        setMotors(0,0);
        
    }
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LEDS, HIGH);
    delay(200);    
    digitalWrite(BUZZER, LOW);
    delay(3000);


    int last_z = 120;
    last_millis = millis();
    while (1){
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


