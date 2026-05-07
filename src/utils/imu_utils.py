import json 
import numpy as np
from pathlib import Path

class IMUCalibrator:
    def __init__(self, config_path="/home/sohailludin/gps-denied/src/config/imu_calibration.json"):
        self.config_path = Path(config_path)
        self.load_calibration()
    
    def load_calibration(self):
        with open(self.config_path, 'r') as f:
            self.calib = json.load(f)
            #Accelerometer and Gyroskop Bias
            self.accel_bias = np.array(self.calib.get("accel_bias", [0,0,0]))
            self.gyro_bias = np.array(self.calib.get("gyro_bias",[0,0,0]))

            #Magnetometer
            self.mag_hard_iron = np.array( self.calib.get("hard_iron_bias", [0,0,0]))
            print("✅ IMU Calibrator geladen:")
            print(f"   Accel Bias: {self.accel_bias}")
            print(f"   Gyro Bias : {self.gyro_bias}")
            print(f"   Mag Hard Iron: {self.mag_hard_iron}")

    def correct(self, accel, gyro, mag):
        """Korrigiert eine einzelne Messung"""
        accel_corr = np.array(accel) - self.accel_bias
        gyro_corr  = np.array(gyro)  - self.gyro_bias
        mag_corr = np.array(mag) - self.mag_hard_iron        
        return accel_corr, gyro_corr, mag_corr