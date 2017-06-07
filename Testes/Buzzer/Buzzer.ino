int buzzer_pin = 6;

#define TERC 142
#define TEMPO 462
#define SHRT 100
#define SEMI 270
#define LONG 390



const float B3  = 247;
const float C4  = 262;
const float Db4 = 277;
const float D4  = 294;
const float Eb4 = 311;
const float E4  = 330;
const float F4  = 349;
const float Gb4 = 370;
const float G4  = 392;
const float Ab4 = 415;
const float lA4 = 440;
const float Bb4 = 466;
const float B4  = 494;
const float C5  = 523;
const float Db5 = 554;
const float D5  = 587;
const float Eb5 = 622;
const float E5  = 659;
const float F5  = 698;
const float Gb5 = 740;
const float G5  = 784;
const float Ab5 = 831;
const float lA5 = 880;
const float Bb5 = 932;
const float B5  = 988;

long long int current_micros, previous_micros;
int i = 0;

void setup(){
    pinMode(buzzer_pin, OUTPUT);
    i = 0;
    previous_micros = millis();
}

int notes[] =     {E4,   E4,   E4,   G4,   E4,   G4,   B5,   lA5,  G4,   lA5,  G4,   Gb4,  E4,   E4,   E4,   G4,   E4,   G4,   D4,  lA4, E4,   E4,   E4,   G4,   E4,   G4,   B5,   lA5,  G4,   lA5,  G4,   Gb4,  E4,   C4,   C4,  E4,   B3,   B3};
int delays[] =    {0,    TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TEMPO, TEMPO, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, TERC, 2*TERC, TEMPO, TERC, 2*TERC};
int durations[] = {SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, LONG,  LONG,  SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SHRT, SEMI,   LONG,  SHRT, SEMI,   LONG};
void loop(){
    current_micros = millis();
    if (i>37){
        i = 0;
        delay(1000);
    }
    if (current_micros - delays[i] > previous_micros){
        previous_micros = current_micros;
        tone(buzzer_pin, notes[i], durations[i]);
        i += 1;
        previous_micros = current_micros;
    }

}
