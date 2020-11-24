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
ax = fig.add_subplot(1, 1, 1)
xs = [] #store trials here (n)
anv_x = [] #store relative frequency here
anv_y = []
anv_z = [] #for theoretical probability

# This function is called periodically from FuncAnimation
def animate(i, xs, anv_x, anv_y, anv_z):

    #Aquire and parse data from serial port
    line=ser.readline()  
    print(line)    #ascii
    #line_as_list = line.split(b',')
    data = json.loads(line)
    
	
	# Add x and y to lists
    xs.append(i)
    anv_x.append(data["av"]["x"])
    anv_y.append(data["av"]["y"])
    anv_z.append(data["av"]["z"])

    # Limit x and y lists to 20 items
    #xs = xs[-20:]
    #ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, anv_x, label="Angular V in X axis")
    ax.plot(xs, anv_y, label="Angular V in Y axis")
    ax.plot(xs, anv_z, label="Angular V in Z axis")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('This is how I roll...')
    plt.ylabel('Relative threshold')
    plt.legend()
    plt.axis([1, None, -600, 600]) #Use for arbitrary number of trials
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, anv_x, anv_y, anv_z), interval=10)
plt.show()
