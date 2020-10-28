import serial
import keyboard
import time
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
while True:
    ctrl_v = 0
    ctrl_w = 0
    ctrl_d = 0

    if keyboard.is_pressed('w'):  # if key 'w' is pressed 
        ctrl_v = 255
        ctrl_d += 1
    if keyboard.is_pressed('a'):
        ctrl_w = 255
        ctrl_d += 2
    if keyboard.is_pressed('s'):
        ctrl_v = 255
    if keyboard.is_pressed('d'):
        ctrl_w = 255

    ard.write(bytes([ctrl_v, ctrl_w, ctrl_d]))
    # print(bytes([ctrl_v, ctrl_w, ctrl_d]))
    if ard.in_waiting:
        print(ard.readline())
    time.sleep(0.1)    
