#include <SoftPWM.h>


void setMotors(int motor_left, int motor_right);

#define MOTOR_L 9
#define MOTOR_L_DIR 10
#define MOTOR_R 7
#define MOTOR_R_DIR 8

void setMotors(int motor_left, int motor_right){
    analogWrite(MOTOR_L,(abs(motor_left)));
    if (motor_left < 0) {
        digitalWrite(MOTOR_L_DIR, LOW);
    } else {
        digitalWrite(MOTOR_L_DIR, HIGH);
    }
    SoftPWMSet(MOTOR_R, (abs(motor_right)));
    if (motor_right < 0) {
        digitalWrite(MOTOR_R_DIR, LOW);
    } else {
        digitalWrite(MOTOR_R_DIR, HIGH);
    }
}

int leitura, motor = 0;
void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_L_DIR, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(MOTOR_R_DIR, OUTPUT);

    SoftPWMBegin();
    SoftPWMSet(MOTOR_L, 0);
    SoftPWMSet(MOTOR_R, 0);
    Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    if (Serial.available()){
        leitura = Serial.read();
        if (leitura == 'w'){
            motor += 5;
        }
        else if (leitura == 's'){
            motor -= 5;
        }
        Serial.println(motor);
    }
    setMotors(motor, motor);
}
