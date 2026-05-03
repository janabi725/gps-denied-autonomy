import time
import adafruit_icm20x
import csv
import board
import numpy as np
from datetime import datetime

# I2C und Sensor initialisieren
i2c = board.I2C()  
icm = adafruit_icm20x.ICM20948(i2c)

# Dateinamen mit Zeitstempel 
timestamp = datetime.now().strftime('%Y-%m-%d_%H-%M')
csv_data = f"imu_raw_{timestamp}.csv"
npy_data = f"imu_raw_{timestamp}.npy"

imu_data = []

messpunkte = 1000 # Wie viele Punkte wir sammeln wollen
i = 0


with open(csv_data, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["timestamp", "ax", "ay", "az", "gx", "gy", "gz", "mx", "my", "mz"]) # Kopfzeile

    print("Logging gestartet")

    try:
        while True:
            t = time.time()

            #Rohdaten lesen
            ax, ay, az = icm.acceleration
            gx, gy, gz = icm.gyro
            mx, my, mz = icm.magnetic

            # 1. Für CSV (Analyse)
            writer.writerow([t, ax, ay, az, gx, gy, gz, mx, my, mz ])

            # 2. Für Echtzeit (In-Memory)
            imu_data.append({
                't': timestamp,
                'accel': np.array([ax, ay, az]),
                'gyro': np.array ([gx, gy, gz]),
                'mag': np.array([mx, my, mz]),

            })
            i+= 1
             # Fortschrittsanzeige in der Konsole
            if i % 100 == 0:
                print(f"[{i}/{messpunkte}] Daten werden gesammelt...")
            
            if i - messpunkte == 0:
                print(f"{messpunkte} Samples gesammelt...")


            time.sleep(0.01)     




    except KeyboardInterrupt:

        print("\n\n🛑 Recording gestoppt.")
        


data_array = np.array(imu_data, dtype=object) 
np.save(npy_data, data_array)

print(f"✅ Gespeichert:")
print(f"   • CSV:  {csv_data}   (für Excel / Plotten)")
print(f"   • NPY:  {npy_data}   (für Python / schnelle Analyse)")