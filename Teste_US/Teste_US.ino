#define TRIG 4
#define ECHO 5

long readUS();

int leitura;

long readUS(){
  long duration, cm;
 
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
 
  duration = pulseIn(ECHO, HIGH);
 
  cm = (duration/2) / 29.1;
  
  return cm;
}

void setup() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  leitura = readUS();
  Serial.println(leitura);
}
