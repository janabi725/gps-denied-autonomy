import time
import board
import adafruit_icm20x


i2c = board.I2C()

icm = adafruit_icm20x.ICM20948(i2c)

while True:
	print("Beschleunigung: X:%.2f, Y: %.2f:, Z: %.2f m/s2" % (icm.acceleration))
	print("Giroskop: X:%.2f, Y: %.2f:, Z: %.2f rads/s" % (icm.gyro))
	print("Magnetometer: X:%.2f, Y: %.2f:, Z: %.2f uT" % (icm.magnetic))
	print("-" * 40)
	time.sleep(0.5)
