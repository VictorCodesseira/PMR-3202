#include <SoftwareSerial.h>

#define IR_L A0
#define IR_R A1

#define THRESHOLD_L 50
#define THRESHOLD_R 50

int sens_esq = 0, sens_dir = 0, dist = 0;

void setup() {
    pinMode(9,OUTPUT);
    pinMode(7,OUTPUT);
    
    Serial.begin(9600);
}

// the loop function runs over and over again forever

void loop() {
    while (1){
        int sens_esq = analogRead(IR_L);
        int sens_dir = analogRead(IR_R);
        if (sens_esq > THRESHOLD_L){
            digitalWrite(9,HIGH);
        }else{digitalWrite(9,LOW);}
        if (sens_dir > THRESHOLD_R){
            digitalWrite(7,HIGH);
        }else{digitalWrite(7,LOW);}



        Serial.print(sens_esq);
        Serial.print('\t');
        Serial.println(sens_dir);
        delay(100);
    }
}
    
