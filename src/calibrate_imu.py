import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time
from datetime import datetime
import json 


# Time and Data Definition
timestamp = datetime.now().strftime('%Y-%m-%d_%H-%M')

imu_npy = "/home/sohailludin/gps-denied/data/imu_raw_2026-05-03_13-12.npy"

imu_data = np.load(imu_npy, allow_pickle=True )


#Acceleration Calculation
acc_data = np.array([item['accel'] for item in imu_data])

acc_bias = np.mean(acc_data, axis = 0)

print(acc_bias)

## Gyroskop Calculation
gyro_data = np.array([item['gyro'] for item in imu_data])
gyro_bias = np.mean(gyro_data, axis = 0)

print(gyro_bias)

#Saving for Real-time calibration
calib = {
    "accel_bias": acc_bias.tolist(),
    "gyro_bias": gyro_bias.tolist(),
    "num_samples": len(imu_data),
    "timestamp": datetime.now().isoformat(),
    "file_used": imu_npy
}

with open("config/imu_calibration.json", "w") as f:
    json.dump(calib, f, indent=2)


#Magnetometer Calibration

cols = ["timestamp", "ax", "ay", "az", "gx", "gy", "gz", "mx", "my", "mz"]

df = pd.read_csv ("/home/sohailludin/gps-denied/data/imu_raw_2026-05-03_13-12.csv", usecols = cols)

## Define data as variabels

ax = df["ax"]
ay = df["ay"]
az = df["az"]

gx = df["gx"]
gy = df["gy"]
gz = df["gz"]

mx = df["mx"]
my = df["my"]
mz = df["mz"]


## Maximum, Minimun Berechnungen

min_x = min(mx)
max_x = max(mx)
min_y = min(my)
max_y = max(my)
min_z = min(my)
max_z = max(mz)

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



