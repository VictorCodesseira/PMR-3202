#define MOTOR_L A0
#define MOTOR_L_DIR 1
#define MOTOR_R A1
#define MOTOR_R_DIR 2
#define SERVO A2
#define IR_L A3
#define IR_R A4
#define US_A 3
#define US_B 4

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define BREAK 4
#define NONE 5

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
        analogWrite(MOTOR_L, -motor_left);
        analogWrite(MOTOR_L_DIR, LOW);
    } else {
        analogWrite(MOTOR_L, motor_left);
        analogWrite(MOTOR_L_DIR, HIGH);
    }

    if (motor_right < 0) {
        analogWrite(MOTOR_R, -motor_right);
        analogWrite(MOTOR_R_DIR, LOW);
    } else {
        analogWrite(MOTOR_R, motor_right);
        analogWrite(MOTOR_R_DIR, HIGH);
    }
}

void setup() {
    pinMode(MOTOR_L, OUTPUT);
    pinMode(MOTOR_L_DIR, OUTPUT);
    pinMode(MOTOR_R, OUTPUT);
    pinMode(MOTOR_R_DIR, OUTPUT);
    pinMode(SERVO, OUTPUT);
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);
    pinMode(US_A, OUTPUT);
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
        if ((Serial.available()) && (Serial.read() == BREAK)) {
            autonomo = 0;
        }
    }

    delay(1000);

    while (controlado == 1){
        if (Serial.available()){
            leitura = Serial.read();
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
            case BREAK:
                controlado = 0;
                break;
            default:
                setMotors(0, 0);
        }
    }
}
