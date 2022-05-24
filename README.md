this Arduino IDE library is for interfacing the I2C 18bit ADC Sereretec pH Sensor electronics by an ESP32 or similar.
You can use following commands in the Serial Monitor.
The Sensor is based on I2C using a 18bit differential MCP3421 ADC.

cal7  
cal6.86   
cal4  
cal9.18   
to change the temp use + or -

confirm with enter.

plus and minus are to set the solution temparature, 
its normally similar to the ambient temp.   
To calibrate, use only 2 points according your calibration solutions.   
f.e. pH 7 and 4, or 6.86 / 4, or 7 and 9.18, or 6.86 / 9.18.  
The calibration is stored in the EEPROM.  
You can enter your solution temparature and the code will atomatically compensate for temp changes.  
Please enter the temparature before calibration.    
Feel free to replace the temp value with a real temp probe value. 

Please see the example file for how to get the readings.
this library is tested with an ESP32, for the Arduino 8bit Atmel enviroment you need to uncommnet some lines regarding the EEPROM.
