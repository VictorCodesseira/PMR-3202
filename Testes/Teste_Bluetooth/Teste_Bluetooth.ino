#include <SoftwareSerial.h>

#define RX 11
#define TX 12

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(11, 12); // RX, TX
void setup(){
  Serial.begin(9600);
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  bluetooth.println("O que vc escreve aqui, sai lá!!!");
  Serial.println("O que vc escreve aqui, sai lá!!!");
}
void loop() {
  // check if data has been sent from the computer:
  if (bluetooth.available())
    Serial.write(bluetooth.read());
  if (Serial.available())
    bluetooth.write(Serial.read());
}
