#pip3 install Adafruit_Python_DHT

import pigpio
from time import sleep

pi = pigpio.pi()


import Adafruit_DHT

sensor = Adafruit_DHT.DHT22

pin = 21

print('Program starting to poll for temperature and humidity')

for x in range(0, 5):
	humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)
	
	if humidity is not None and temperature is not None:
		temp = (temperature * 1.8) + 32	
		print('{0}. Temp={1:0.1f}f  Humidity={2:0.1f}%'.format(x+1, temp, humidity))
	else:
		print('{}. Failed to get reading.'.format(x+1));

	sleep(3)
	
print('Program finished')
