#ifndef SERERETEC_PH_SENSOR_H
#define SERERETEC_PH_SENSOR_H
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

class pH_Sensor
{
  public:
    void eepromget();
    void pHSensorConfigure();
    float readpH();
    float passtemp(float tempC);
    void s1read();

  private:
    float mVpHhigh  = 13.3; //pH buffer 7 in mV
    float mVpHlow  = 177.0; //pH buffer 4 in mV
    float temp   = 25;
    float caltemp = 25;
    float slope = -56.84;
    float pHbufferlow = 4.00;
    float pHbufferhigh = 6.88;
    float voltageraw = 1;
    float phvalue;
    unsigned long previousMillis = 0;
    String s1;

};

#endif
