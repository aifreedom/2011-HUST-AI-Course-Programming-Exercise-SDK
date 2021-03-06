#!/usr/bin/python

# Huazhong University of Science and Technology 
# 2011 AI Course Programming Exercise SDK
# Author: Song Xie<mail@xiesong.me>
# Date: October 20, 2011
#    
# This file is part of HUST AI Course Programming Exercise SKD.
#
# HUST AI Course Programming Exercise SKD is free software: you can
# redistribute it and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
# HUST AI Course Programming Exercise SKD is distributed in the hope
# that it will be useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with HUST AI Course Programming Exercise SKD. If not, see
# <http://www.gnu.org/licenses/>.

from subprocess import Popen, PIPE, STDOUT
import sys
import datetime
import os.path

def Translate(tile):
    if tile == '#':
        return False
    elif tile == '.':
        return True
    else:
        raise ValueError

def ReadMap(mapfile):
    f = open(mapfile, 'r')
    H, W = [int(i) for i in f.readline().strip().split(' ')]
    startX, startY = [int(i)-1 for i in f.readline().strip().split(' ')]

    m = []
    for i in range(H):
        line = f.readline().strip()
        if len(line) != W:
            raise ValueError
        m.append([Translate(t) for t in line])

    # start point is not with the board's range
    if startX < 0 or startX >= H or startY < 0 or startY >= W:
        raise ValueError

    # start point is an obstacle
    if not m[startX][startY]:
        raise ValueError

    for i in range(H):
        if m[i][0] or m[i][W-1]:
            raise ValueError

    for i in range(W):
        if m[0][i] or m[H-1][i]:
            raise ValueError

    return m, (startX, startY)

def GetMapState(m, x, y):
    if x == 0 or x == len(m)-1 or y == 0 or y == len(m[0])-1:
        raise Exception('Unknown error. Please contact mail@xiesong.me')

    return ''.join(('1' if t else '0' for t in (m[x-1][y-1], m[x-1][y], m[x-1][y+1], m[x][y+1], m[x+1][y+1], m[x+1][y], m[x+1][y-1], m[x][y-1])))
    
def Valid(m, x, y):
    return x >= 0 and x < len(m) and y >= 0 and y < len(m[0]) and m[x][y]

def PrintMap(out, m, startP):
    x, y = startP
    out.write('%d %d\n' % (len(m), len(m[0])))
    out.write('%d %d\n' % (x+1, y+1))
    for line in m:
        out.write(''.join(('.' if t==True else '#' for t in line)) + '\n')
    

def RunRobot(robot, m, startP, totalStep, currentTime):
    debug = False

    replayFileName = 'RobotReplay_%s.log' % currentTime
    errorLogFileName = 'RobotErrorLog_%s.log' % currentTime
    
    maxUserDataLen = 10000
    robot = os.path.realpath(robot)
    x, y = startP
    stepList = []
    
    p = Popen([robot], stdin=PIPE, stdout=PIPE, bufsize=1)
    for step in range(totalStep):
        if debug:
            print x, y, robot, GetMapState(m, x, y) + '\n' + userData + '\n'
            pass

        try:
            p.stdin.write(GetMapState(m, x, y) + '\n')
            p.stdin.flush()
        except IOError:
            pass
        
        output = p.stdout.readline().strip()

        
        if output == 'N':
            newx = x - 1
            newy = y
        elif output == 'S':
            newx = x + 1
            newy = y
        elif output == 'W':
            newx = x
            newy = y - 1
        elif output == 'E':
            newx = x
            newy = y + 1
        elif output == '$':
            break
        else:
            print 'runrobot: Error! "%s" is not a valid move direction.' % output
            raise ValueError

        if Valid(m, newx, newy):
            x, y = newx, newy
        else:
            errorLog = open(errorLogFileName, 'w')
            PrintMap(errorLog, m, startP)
            errorLog.write('%d\n' % step)
            errorLog.write(''.join(stepList) + '\n')
            errorLog.write('\nThe last step:\n')
            errorLog.write('Pos: (%d, %d)\n' % (x+1, y+1))
            errorLog.write(output+'\n')
            errorLog.close()
            returnCode = p.poll()
            if returnCode == 0:
                p.wait()
            else:
                p.kill()
            print 'runrobot: Error! Your robot moves into an obstacle. Please check the error log %s.' % errorLogFileName
            raise ValueError
        
        stepList.append(output)

    replay = open(replayFileName, 'w')
    PrintMap(replay, m, startP)
    replay.write('%d\n' % totalStep)
    replay.write(''.join(stepList) + '\n')
    replay.close()
    returnCode = p.poll()
    if returnCode == 0:
        p.wait()
    else:
        p.kill()
# end of RunRobot
    
def Run(robot, mapfile, step):
    currentTime = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    replayFileName = 'RobotReplay_%s.log' % currentTime
    try:
        m, (startX, startY) = ReadMap(mapfile)
    except IOError:
        print 'runrobot: Error! Mapfile "%s" does not exist!' % mapfile
        sys.exit()
    except ValueError:
        print 'runrobot: Error! Mapfile "%s" is not a valid mapfile!' % mapfile
        sys.exit()

    try:
        totalStep = int(step)
    except ValueError:
        print 'runrobot: Error! "%s" is not a valid integer for step!' % step
        sys.exit()

    try:
        RunRobot(robot, m, (startX, startY), totalStep, currentTime)
    except ValueError:
        print 'runrobot: Wrong output from robot.'
        sys.exit()
        
    print 'runrobot: Congratulations! The log is written to %s.' % replayFileName

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'runrobot: use "%s robot mapfile step" to run your robot' % (sys.argv[0])
    else:
        try:
            Run(sys.argv[1], sys.argv[2], sys.argv[3])
        except SystemExit:
            print "runrobot: runrobot will now be terminated."
            pass
        except:
            print "Unexpected error:", sys.exc_info()[0]
            raise 

