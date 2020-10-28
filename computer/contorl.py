import serial
import keyboard
import time
import paths as auto
import numpy as np

ard = serial.Serial("COM4", 9600)

fourPoint = [
    (1, 0, 0.5),
    (1, 1, 0.1),
    (1, -1, 0.1),
    (1, 1, 2),
    (-1, -1, 2),
    (1, 1, 2),
    (-1, -1, 2)
]

motionTest = []

for w in [-1, 0, 1]:
    for v in [-1, 1]:
        for t in np.linspace(1, 2, num=5):
            for _ in range(0, 2):
                motionTest.append((v, w, t))
                motionTest.append((0, 0, 10))

# cmd in the form of (v, w, t) 
# -1 <= v, w, <= 1
# t is sec
def runCmds(cmds):
    for cmd in cmds:
        v, w, t = cmd
        ctrl_v = int(abs(v) * 255)
        ctrl_w = int(abs(w) * 255)
        ctrl_d = 0
        if v > 0:
            ctrl_d += 1
        if w < 0:
            ctrl_d += 2

        print(cmd)        
        cmdstr = bytes([ctrl_v, ctrl_w, ctrl_d])
        for _ in range(int(t*100)):
            ard.write(cmdstr)
            time.sleep(0.01)

# runCmds(fourPoint)

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
