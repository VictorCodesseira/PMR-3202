#define IR_L A0
#define IR_R A1

int sensor_1, sensor_2;
void setup() {
    pinMode(IR_L, INPUT);
    pinMode(IR_R, INPUT);
     Serial.begin(9600);
    pinMode(13,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    sensor_1 = analogRead(IR_L);
    sensor_2 = analogRead(IR_R);
     Serial.println(sensor_1);
    if (sensor_1 < 60){
        digitalWrite(13, HIGH);
    }
    else{

        digitalWrite(13,LOW);
    }

    }
    // Serial.print('\t');
    // Serial.println(sensor_2);
    

