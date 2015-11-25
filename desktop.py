import serial
import time
import numpy as np
import matplotlib.pyplot as plt

arduA = serial.Serial('/dev/ttyACM0', 9600, timeout=.1)
arduB = serial.Serial('/dev/ttyACM1', 9600, timeout=.1)

d  = 77 #Distanza tra i sensori

def calcola(a, b):#Distanza oggetto da sensore B
  if a != -1:
    a = d-a
  
  if a == -1:
    return b
  elif b == -1:
    return a
  else:
    return (a+b)/2
  
  
#region Plot

plt.ion()
plt.show()

t = 0

o = 0.0
oldA = ""
oldB = ""

while True:
  valA = arduA.readline()[:-2] #the last bit gets rid of the new-line chars
  valB = arduB.readline()[:-2] #the last bit gets rid of the new-line chars
    
  if not valA:
    valA = oldA
    
  if not valB:
    valB = oldB
  
  oldA = valA
  oldB = valB
  y=0
  
  print valA + " | " + valB
  
  if valA and valB:
    y = calcola(int(valA), int(valB))
  
  plt.axis([-10+t, 20+t, 0, d])
  if y != 0:
    plt.plot([t-1, t], [o, y], marker='o', color='b', linestyle='-')
    o=y
  plt.draw()
  t += 1
  
#endregion
  
