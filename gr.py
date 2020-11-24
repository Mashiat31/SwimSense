import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
import json

#initialize serial port
ser = serial.Serial()
ser.port = '/dev/tty.usbmodem83945901' #Arduino serial port
ser.baudrate = 9600
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

#print(ser.readline())
# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(2, 1, 1)
ak = fig.add_subplot(2, 1, 2)
xs = [] #store trials here (n)
xp = []

acc_x = [] #store relative frequency here
acc_y = []
acc_z = [] #for theoretical probability

la_acc_x = [] #store relative frequency here
la_acc_y = []
la_acc_z = []

# This function is called periodically from FuncAnimation
def animate(i, xs, acc_x, acc_y, #acc_z):

    #Aquire and parse data from serial port
    line=ser.readline()  
    print(line)
    #line_as_list = line.split(b',')
    data = json.loads(line)

    
	
	# Add x and y to lists
    xs.append(i)
    acc_x.append(data["acc"]["x"])
    acc_y.append(data["acc"]["y"])
    acc_z.append(data["acc"]["z"])

    # xp.append(i)
    # la_acc_x.append(data["la_acc"]["x"])
    # la_acc_y.append(data["la_acc"]["y"])
    # la_acc_z.append(data["la_acc"]["z"])


    # Limit x and y lists to 20 items
    #xs = xs[-20:]
    #ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, acc_x, label="Acceleration in X axis")
    ax.plot(xs, acc_y, label="Acceleration in Y axis")
    ax.plot(xs, acc_z, label="Acceleration in Z axis")


    # # Draw x and y lists
    # ak.clear()
    # ak.plot(xp, la_acc_x, label="Acceleration in X axis")
    # ak.plot(xp, la_acc_y, label="Acceleration in Y axis")
    # ak.plot(xp, la_acc_z, label="Acceleration in Z axis")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('This is how I roll...')
    plt.ylabel('Relative threshold')
    plt.legend()
    plt.axis([1, None, -200, 200]) #Use for arbitrary number of trials
    ak.set_ylim([-200, 200])
    ax.set_ylim([-600, 600])
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, acc_x, acc_y, acc_z), interval=10)
plt.show()
