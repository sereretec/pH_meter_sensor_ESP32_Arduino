#include "Sereretec_pH_Sensor.h"
pH_Sensor pH_Sensor;
float phresult;
float temp;

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512); // uncomment for Arduino 8bit, Atmega328 etc.
  pH_Sensor.eepromget();
  pH_Sensor.pHSensorConfigure();
}
void loop() {
  pH_Sensor.s1read();
  // pH_Sensor.readpH();
  temp = 22;
  pH_Sensor.passtemp(temp);
  phresult = pH_Sensor.readpH();
  Serial.print("pH= ");
  Serial.println(phresult, 3);
}
