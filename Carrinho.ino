#define MOTOR_LA 1
#define MOTOR_LB 2
#define MOTOR_RA 3
#define MOTOR_RB 4
#define SERVO 5
#define IR_L 6
#define IR_R 7
#define US_A 8
#define US_B 9

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define BREAK 4

#define MOTOR_L 0
#define MOTOR_R 1

#define STD_SPD 100
#define ACT 20
#define THRESHOLD 500

void setMotors(int motor_left, int motor_right);
int autonomo = 1, controlado = 1;
int sens_esq = 0;
int sens_dir = 0;
int leitura = 0;

void setMotors(int motor_left, int motor_right){
    if (motor_left < 0) {
        analogWrite(MOTOR_LA, 0);
        analogWrite(MOTOR_LB, -motor_left);
    } else {
        analogWrite(MOTOR_LA, motor_left);
        analogWrite(MOTOR_LB, 0);
    }

    if (motor_right < 0) {
        analogWrite(MOTOR_RA, 0);
        analogWrite(MOTOR_RB, -motor_right);
    } else {
        analogWrite(MOTOR_RA, motor_right);
        analogWrite(MOTOR_RB, 0);
    }
}

void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(SERVO, OUTPUT);
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);
    pinMode(US_A, INPUT);
    pinMode(US_B, INPUT);
    Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
    while (autonomo == 1){
        sens_esq = analogRead(IR_L);
        sens_dir = analogRead(IR_R);
        if ((sens_esq > THRESHOLD) && (sens_dir < THRESHOLD)){//Linha na esquerda
            setMotors(STD_SPD - ACT, STD_SPD);
        } else if ((sens_esq < THRESHOLD) && (sens_dir > THRESHOLD)){
            setMotors(STD_SPD, STD_SPD - ACT);
        } else {
            setMotors(STD_SPD, STD_SPD);
        }
        if (Serial.read() == BREAK) {
            autonomo = 0;
        }
    }

    delay(1000);

    while (controlado == 1){
        leitura = Serial.read();
        switch (leitura){
            case UP:
                setMotors(STD_SPD, STD_SPD);
            case RIGHT:
                setMotors(STD_SPD, -STD_SPD);
            case DOWN:
                setMotors(-STD_SPD, -STD_SPD);
            case LEFT:
                setMotors(-STD_SPD, STD_SPD);
            case BREAK:
                controlado = 0;
        }
    }
}
