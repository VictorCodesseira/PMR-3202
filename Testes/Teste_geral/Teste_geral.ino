void setup(){
    pinMode(6, OUTPUT);
    pinMode(2,OUTPUT);
}
void loop(){
    
    digitalWrite(2, HIGH);
    analogWrite(6,127);
    delay(400);
    digitalWrite(2, LOW);
    analogWrite(6,255);
    delay(500);
}