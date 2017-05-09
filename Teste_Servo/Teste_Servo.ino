#include <Servo.h>

#define SERVO 3

Servo myservo;
int leitura, angulo = 90;

void setup() {
    myservo.attach(SERVO);

    Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    if (Serial.available()){
        leitura = Serial.read();
        if (leitura == 'w'){
            angulo += 5;
        }
        else if (leitura == 's'){
            angulo -= 5;
        }
    }
    myservo.write(angulo);
}
