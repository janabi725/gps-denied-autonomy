import time
import board
import adafruit_icm20x
import csv

# I2C und Sensor initialisieren
i2c = board.I2C()  
icm = adafruit_icm20x.ICM20948(i2c)

# CSV Datei vorbereiten
dateiname = "mag_rohdaten.csv"
messpunkte = 1000 # Wie viele Punkte wir sammeln wollen

print("ACHTUNG: Sobald es losgeht, bewege den Sensor in Form einer 3D-Acht in ALLE Richtungen!")
time.sleep(3)
print("Starte Messung...")

with open(dateiname, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Mag_X", "Mag_Y", "Mag_Z"]) # Kopfzeile
    
    for i in range(messpunkte):
        mag_x, mag_y, mag_z = icm.magnetic
        writer.writerow([mag_x, mag_y, mag_z])
        
        # Fortschrittsanzeige in der Konsole
        if i % 100 == 0:
            print(f"[{i}/{messpunkte}] Daten werden gesammelt...")
            
        time.sleep(0.05) # Kurze Pause, ca. 20 Messungen pro Sekunde

print(f"Fertig! {messpunkte} Messpunkte in {dateiname} gespeichert.")