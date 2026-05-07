import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import time
from datetime import datetime
import json 
import math
import os 
import sys
from scipy import linalg
import argparse




# Time and Data Definition
timestamp = datetime.now().strftime('%Y-%m-%d_%H-%M')

imu_npy = "/home/sohailludin/gps-denied/data/imu_raw_2026-05-03_13-12.npy"

imu_data = np.load(imu_npy, allow_pickle=True )


#Acceleration Calculation
acc_data = np.array([item['accel'] for item in imu_data])

acc_bias = np.mean(acc_data, axis = 0)

print("====Accelerometer Bias equals====","\n","ax", acc_bias[0],"\n", "ay", acc_bias[1],"\n", "az", acc_bias[2],"\n","============================")

## Gyroskop Calculation
gyro_data = np.array([item['gyro'] for item in imu_data])
gyro_bias = np.mean(gyro_data, axis = 0)

print("====Gyroskop Bias equals====","\n","gx", gyro_bias[0],"\n", "gy", gyro_bias[1],"\n", "gz", gyro_bias[2],"\n","============================")

#Magnetometer Calibration

mag_data = np.array([item['mag'] for item in imu_data])

hard_iron = np.mean(mag_data, axis=0)
print("Einfacher Hard Iron Bias:", hard_iron)

# Korrigierte Werte
mag_cal = mag_data - hard_iron



#Saving for Real-time calibration
calib = {
    "accel_bias": acc_bias.tolist(),
    "gyro_bias": gyro_bias.tolist(),
    "mag_hard_iron": mag_cal.tolist(),
    "hard_iron_bias": hard_iron.tolist(),
    "num_samples": len(imu_data),
    "timestamp": datetime.now().isoformat(),
    
}

with open("config/imu_calibration.json", "w") as f:
    json.dump(calib, f, indent=2)

print("Magnitude vor Korrektur:", np.mean(np.linalg.norm(mag_data, axis=1)))
print("Magnitude nach Korrektur:", np.mean(np.linalg.norm(mag_cal, axis=1)))