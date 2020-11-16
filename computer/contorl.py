import serial
import keyboard
import time
import paths as auto
import numpy as np

ard = serial.Serial("COM4", 9600)

sensorArr = [0,0]

while True:
    ctrl_v = 0
    ctrl_w = 0
    ctrl_d = 0
    maxRev = 55
    placeHolder = 0

    # ctrl_v, ctrl_w, ctrl_d = auto.leftOval(sensorArr[0])
    # ctrl_v, ctrl_w, ctrl_d, maxRev = auto.driveLengthOfDesks(sensorArr[0])
    # print(sensorArr, (sensorArr[0]*10.286)/12)

    if keyboard.is_pressed('w'):  # if key 'w' is pressed 
        ctrl_v = 255
        ctrl_d += 1
        # maxRev = -1
    if keyboard.is_pressed('a'):
        ctrl_w = 255
        ctrl_d += 2
        # maxRev = -1
    if keyboard.is_pressed('s'):
        ctrl_v = 255
    if keyboard.is_pressed('d'):
        ctrl_w = 255

    ard.write(bytes([ctrl_v, ctrl_w, ctrl_d,maxRev,placeHolder]))
    # print(bytes([ctrl_v, ctrl_w, ctrl_d,maxRev,placeHolder]))
    if ard.in_waiting:
        ackInfo = ard.readline()
        print(ackInfo)
        ackInfo = ackInfo.strip()
        ackInfo = ackInfo.decode("utf-8")
        try:
            sensorArr = [int(s) for s in ackInfo.split(',')]
        except:
            pass
        #print(ard.readline())
        # print(ackInfo)
        
    time.sleep(0.1)    
