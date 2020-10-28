import serial
import keyboard
import time

CIRCUMFERENCE = 10.286

v = 0
w = 0
d = 0
rm = 0

def driveLengthOfDesks(revolutions):
    global v,w,d,rm
    if (revolutions <= 18):
        v = 255
        d = 1
        rm = 18
    else:
        v = 0
        w = 0
        d = 0

    return v,w,d,rm
        


def leftOval(revolutions):
    global v, w, d
    v = 255
    d = 1
    if (revolutions >= 18 and revolutions < 24):
        w = 255
        d += 2
    # if (revolutions >= 19 and revolutions < 25):
    #     w = 0
    #     d = 1
    # if (revolutions >= 25 and revolutions < 37):
    #     w = 255
    #     d += 2
    # if(revolutions > 18 and revolutions < 20):
    #     v = 255
    #     d = 0
    #     w = 0
    if(revolutions >= 24):
        v = 0
        d = 0 
        w = 0

    return v, w, d
    

# dist should be passed in as feet
def distToRotations(dist):
    dist = dist * 12
    rotations = dist/CIRCUMFERENCE
    return int(rotations)