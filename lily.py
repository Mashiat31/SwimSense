import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
import json

#initialize serial port
ser = serial.Serial()
ser.port = '/dev/tty.usbmodem83940101' #Arduino serial port
ser.baudrate = 9600
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(3, 1, 1)
ay= fig.add_subplot(3, 1, 2)
xs = [] #store trials here (n)
xp = []

ang_x = [] #store relative frequency here
ang_y = []
ang_z = [] #for theoretical probability
acc_x = [] #store relative frequency here
acc_y = []
acc_z = [] #for theoretical probability

# This function is called periodically from FuncAnimation
def animate(i, xp, xs, acc_x, acc_y, acc_z, ang_x, ang_y, ang_z):

    #Aquire and parse data from serial port
    line=ser.readline()  
    print(line)    #ascii
    #line_as_list = line.split(b',')
    data = json.loads(line)
    
	
	# Add x and y to lists
    xp.append(i)
    acc_x.append(data["acc"]["x"])
    acc_y.append(data["acc"]["y"])
    acc_z.append(data["acc"]["z"])

    xs.append(i)
    ang_x.append(data["ang"]["roll"])
    ang_y.append(data["ang"]["pit"])
    ang_z.append(data["ang"]["yaw"])

    # Limit x and y lists to 20 items
    #xs = xs[-20:]
    #ys = ys[-20:]

    # Draw x and y lists
    ay.clear()
    ay.plot(xp, acc_x)
    ay.plot(xp, acc_y)
    ay.plot(xp, acc_z)
    # ay.plot(xp, acc_x, label="Acceleration in X axis")
    # ay.plot(xp, acc_y, label="Acceleration in Y axis")
    # ay.plot(xp, acc_z, label="Acceleration in Z axis")


    ax.clear()
    ax.plot(xs, ang_x, label="Roll")
    ax.plot(xs, ang_y, label="Pitch")
    ax.plot(xs, ang_z, label="Yaw")


    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('')
    plt.ylabel('Relative threshold')
    plt.legend()
    plt.axis([1, None, -600, 600]) #Use for arbitrary number of trials
    ay.set_ylim([-200, 200])
    ax.set_ylim([-600, 600])
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, xp, acc_x, acc_y, acc_z, ang_x, ang_y, ang_z), interval=10)
plt.show()


# This function is called periodically from FuncAnimation
