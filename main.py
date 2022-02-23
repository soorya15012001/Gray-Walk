import time

from pynput.mouse import Button, Controller
import serial
import keyboard

mouse = Controller()

a = serial.Serial('com9', 9600)
# a = serial.Serial('com7', 9600)
print(a)
while 1:
    try:
        data = str(a.readline().decode('ascii'))
        data = data.split(':')
        print(data)
        if data[0] == "data":
            if data[5] == '1':
                x = float(data[10]) * -0.6
                y = float(data[9]) * 0.6

                mouse.move(int(x), int(y))

            if float(data[1]) == 0.00:
                keyboard.release('w')
            elif 0.00 < float(data[1]) <= 1.00:
                keyboard.press('shift+w')
            elif 1.00 < float(data[1]):
                keyboard.press('w')

            if data[6] == '1':
                mouse.click(Button.right, 1)

            if data[7] == '1':
                mouse.click(Button.left, 1)

            if data[3] == "up":
                keyboard.press_and_release('r')
            elif data[3] == "down":
                keyboard.press_and_release('s')
            elif data[2] == "left":
                keyboard.press_and_release('a')
            elif data[2] == "right":
                keyboard.press_and_release('1')

            elif data[4] == '1':
                keyboard.press_and_release('space')
    except:
        print("stand by")

