# -*- coding: utf-8 -*-
"""
Created on Tue May 16 08:25:51 2017

@author: Sanches
"""
import time
import serial
import pygame

def connect():
    global device
    port = "COM11"
    device = serial.Serial(port, 9600)

def send(pack):
    global device
    device.write(pack)

def kill():
    global device
    device.close()

def joystick():
    global device
    pygame.init()
    clock = pygame.time.Clock()
    done = False
    pygame.joystick.init()
    i = 0
    joystick = pygame.joystick.Joystick(i)
    joystick.init()
    joystick.get_name()

    xtrim = 0
    ytrim = 0
    ztrim = 0

    last_pack = [255, 0, 0, 0, 0, 0, 254]

    while not done:
        while(device.in_waiting > 0):
            print(device.readline())
        for event in pygame.event.get(): # User did something
            if event.type == pygame.QUIT: # If user clicked close
                done = True # Flag that we are done so we exit this loop

        azul = rosa = 0

        #for event in pygame.event.get(): # User did something
        x = int(127 + (127 * (joystick.get_axis(0) - ytrim)))
        y = int(127 + (127 * -(joystick.get_axis(1) - xtrim)))
        z = int(127 + (127 * (joystick.get_axis(2) )))
        if x > 253:
            x = 253
        if y > 253:
            y = 253
        if z > 253:
            z = 253

        if joystick.get_button(0):
            z = 0
        if joystick.get_button(2):
            azul = 1
        if joystick.get_button(3):
            rosa = 1

        if joystick.get_button(11):
            if joystick.get_button(6):
                done = True

        if joystick.get_button(10):
            xtrim = joystick.get_axis(1)
            ytrim = joystick.get_axis(1)
            

        z = ((z*80)/255)+40
        z = int(z)
        pack = [255, x, y, z, azul, rosa, 254]
        if pack != last_pack:
            print(pack)
            send(pack)
            last_pack = pack
        clock.tick(20)

def playstation():
    global device
    pygame.init()
    clock = pygame.time.Clock()
    done = False
    pygame.joystick.init()
    i = 0
    joystick = pygame.joystick.Joystick(i)
    joystick.init()
    joystick.get_name()

    z = 84

    last_pack = [127, 127, 0, 0, 0]

    while not done:
        # while(device.in_waiting > 0):
        #     print(device.readline())
        for event in pygame.event.get(): # User did something
            if event.type == pygame.QUIT: # If user clicked close
                done = True # Flag that we are done so we exit this loop

        luz = buzina = 0

        #for event in pygame.event.get(): # User did something
        x = int(127 + (127 * (joystick.get_axis(2))))
        y = int(127 + (127 * -(joystick.get_axis(3))))
        
        if x > 253:
            x = 253
        if y > 253:
            y = 253
        

        if joystick.get_button(4):
            z = 60
        else:
            z = 110
        if joystick.get_button(2):
            luz = 1
        if joystick.get_button(3):
            buzina = 1

        if joystick.get_button(9):
            done = True

        
        chksum = (x + y + z + luz + buzina)%256

        if z < 50:
            z = 30
        elif z > 110:
            z= 110 
        z = int(z)
        pack = [255, x, y, z, luz, 254]
        if pack != last_pack:
            print(pack)
            send(pack)
            last_pack = pack
        clock.tick(20)
connect()
joystick()
#playstation()
kill()
