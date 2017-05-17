# -*- coding: utf-8 -*-
"""
Created on Tue May 16 08:25:51 2017

@author: Sanches
"""
import time
import serial
import pygame

class sDevice:
    def __init__(self, port, speed=9600):
        self.connection = serial.Serial(port,speed)
        
           
    def kill(self):
        self.connection.close()
    
    def read(self):
        
        while self.connection.in_waiting>0:
            print(int.from_bytes(self.connection.read(), byteorder='big'))
        
    def writeBytes(self, nun):
        self.connection.write([nun])
        
def send(info, device):
    b = info
    device.writeBytes([b])
    
def play(car):
    
    pygame.init()
    clock = pygame.time.Clock()
    done = False
    pygame.joystick.init()
    i = 0
    joystick = pygame.joystick.Joystick(i)
    joystick.init()
    joystick.get_name()
    
    xtrim =0
    ytrim =0
    ztrim =0
    
    last_pack = [127,127,0,0,0]
    counter =0
    while not done:
        for event in pygame.event.get(): # User did something
            if event.type == pygame.QUIT: # If user clicked close
                done=True # Flag that we are done so we exit this loop
        
        pack = [127,127,0,0,0]
           
        #for event in pygame.event.get(): # User did something
        pack[0] = x = 127 + (127 * (joystick.get_axis(1) - xtrim))
        pack[1] = y = 127 + (127 * (joystick.get_axis(0) - ytrim))
        pack[2] = z = 127 + (127 * (joystick.get_axis(2) - ztrim))
        
        if (joystick.get_button(0)):
            pack[2] = z = 255
        if (joystick.get_button(2)):
            pack[3] = 1
        if (joystick.get_button(3)):
            pack[4] = 1
        
        if (joystick.get_button(11)):
            done = True
            break
        if (joystick.get_button(10)):
            xtrim = joystick.get_axis(1)
            ytrim = joystick.get_axis(1)
            ztrim = joystick.get_axis(2)
            
        if pack != last_pack:
            print(z)
            z = int(z)
            
            if z in range(0,256):
                #car.connection.write([z])  #send(z , car)
                pass
            last_pack = pack
        clock.tick(20)
        
def test(device):
    for i in range(255):
        device.writeBytes(i)
    for i in range(255,0,-1):
        device.writeBytes(i)
        