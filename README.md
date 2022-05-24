this library is for interfacing the I2C 18bit ADC Sereretec pH Sensor electronics by an ESP32 or similar.
You can use following commands in the Serial Monitor.
The Sensor is based on I2C using a 18bit differential ADC.

cal7  
cal6.86 
cal4  
cal9.18 
+   
- 
  are the commands to enter.
+ and - are to set the solution temparature, its normally similar to the ambient temp.
to calibrate, use only 2 points according your needs and used solutions.
Like pH 7 and 4, or 6.86 / 4, or 7 and 9.18, or 6.86 / 9.18.
The calibration is stored in the EEPROM.
You can enter your soluton temparature and the code will atomatically compensate for temp changes.
Feel free to replace the temp value with a real temp probe value.

Please see the example file for how to get the readings.
this library is tested with a ESP32, for the Arduino 8bit Atmel enviroment you need to uncommnet some lines regarding the EEPROM.
