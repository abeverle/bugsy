import numpy as np

class Map():
    def __init__(self, ftw, fth, res=(6/12)):
        lenw = int(ftw / res)
        lenh = int(fth / res)
        self.grid = np.zeros([lenw, lenh])
        self.grid[0, 0] = 2
        self.grid[4, 8] = 2
        self.res = res # feet / grid

    # robot x and y in feet
    # distance in inches
    # heading in angle from X axis
    def addObstacle(self, robX, robY, heading, dist):
        obsX = (robX + np.cos(heading) * dist / 12) / self.res
        obsY = (robY + np.sin(heading) * dist / 12) / self.res
        lenx, leny = np.shape(self.grid)

        if obsX < 0 or obsX > lenx or obsY < 0 or obsY > leny:
            return

        self.grid[int(obsY)][int(obsX)] = 1

if __name__ == "__main__":
    m = Map(10, 10, res=1)
    m.addObstacle(0, 0, 0, 24)
    m.addObstacle(0, 0, np.pi/2, 48)
    m.addObstacle(3, 3, np.pi / 4, 30)
    print(m.grid)