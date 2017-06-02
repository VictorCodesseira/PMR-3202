import msvcrt
import serial
import struct

def connect():
    global arduino
    arduino = serial.Serial('COM14', 9600)
def kill():
    global arduino
    arduino.close()
def play():
    global arduino
    on = True
    while on:
        
        mode = msvcrt.getch().decode('utf-8').lower()
        if mode == 'p':
            on = False
        else:
            arduino.write(struct.pack('>B', ord(mode)))

connect()
play()
kill()
        
            

