import pandas as pd
import numpy
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


cols = ["Mag_X", "Mag_Y", "Mag_Z" ]


df = pd.read_csv ("/home/sohailludin/gps-denied/src/mag_rohdaten.csv", usecols = cols)
## Define data as variabels

print(df["Mag_X"])

mag_x = df["Mag_X"]
mag_y = df["Mag_Y"]
mag_z = df["Mag_Z"]


## Display current values


## Maximum, Minimun Berechnungen

min_x = min(mag_x)
max_x = max(mag_x)
min_y = min(mag_y)
max_y = max(mag_y)
min_z = min(mag_z)
max_z = max(mag_z)

print("X Range", min_x, max_x)
print("Y Range", min_y, max_y)
print("Z Range", min_y, max_z)


## Hard-Iron correction calculation 

mag_bias = [(max_x + min_x) / 2, (max_y + min_y)/2, (max_z + min_z) /2]

print("Hard Iron Offset", mag_bias)


##Display corrected values

# Corrected magnetic values
cal_mag_x  = [x - mag_bias[0] for x in mag_x]
cal_mag_y  = [y - mag_bias[1] for y in mag_y]
cal_mag_z  = [z - mag_bias[2] for z in mag_z]

#Display the calibrated are

fig, ax = plt.subplots(1, 1)
ax.set_aspect(1)
ax.scatter(cal_mag_x, cal_mag_y, color = "r")
ax.scatter(cal_mag_y, cal_mag_z, color = "g")
ax.scatter(cal_mag_z, cal_mag_x, color = "b")
plt.show()