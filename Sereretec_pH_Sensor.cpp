/*
   Copyright (c) <2022> <Sereretec>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  pH_Sensor.h - Library for the sereretec pH Sensor Module.
  connect SDA SCL 5V and GND to your Arduino.
  measure the mV of your pH4 and pH7 buffer solution.
  wait till readings have stabilized, clean your probe before calibration,
  by entering + or - in the serial terminal you change the temparature.
  by entering cal7, cal6.86,cal9.18 or cal4 in the serial terminal you calibrate.
*/

#include "Arduino.h"
#include "Sereretec_pH_Sensor.h"


void pH_Sensor::eepromget() {
  EEPROM.get(0, mVpHhigh);
  EEPROM.get(4, mVpHlow);
  EEPROM.get(8, temp);
  EEPROM.get(12, caltemp);
  EEPROM.get(16, pHbufferlow);
  EEPROM.get(20, pHbufferhigh);
}

void pH_Sensor::pHSensorConfigure() {
  Wire.begin();
  Wire.beginTransmission(0x68);//i2c address MCP3421 18bit ADC
  Wire.write(B11101); // 18bit 2x gain
  Wire.endTransmission();
  eepromget();
  if (isnan(temp)) {
    temp = 25;
  }
  if (isnan(caltemp)) {
    caltemp = 25;
  }
  if (temp >= 40 or temp <= 5) {
    temp = 25;
  }
  if (caltemp >= 40 or caltemp <= 5) {
    caltemp = 25;
  }

  if (temp >= 40 or temp <= 5) {
    temp = 25;
  }

  if (isnan(pHbufferlow)) {
    pHbufferlow = 4;
  }
  if (isnan(pHbufferhigh)) {
    pHbufferhigh = 7;
  }
  if (pHbufferlow >= 16 or pHbufferlow <= 1 or pHbufferlow == 0) {
    pHbufferlow = 4;
  }
  if (pHbufferhigh >= 16 or pHbufferhigh <= 1 or pHbufferhigh == 0) {
    pHbufferhigh = 7;
  }
  if (isnan(mVpHlow)) {
    mVpHlow = 178;
  }
  if (isnan(mVpHhigh)) {
    mVpHhigh = 8;
  }
  if (mVpHlow >= 500 or mVpHlow < -500 or mVpHlow == 0) {
    mVpHlow = 177.25;
  }
  if (mVpHhigh > 500 or mVpHhigh < -500 or mVpHhigh == 0) {
    mVpHhigh = 6.25;
  }
  delay(20);
}

float pH_Sensor::readpH() {
  float result;
  Wire.requestFrom(0x68, 4);
  byte b2 = Wire.read();
  byte b3 = Wire.read();
  byte b4 = Wire.read();
  byte readybyte = Wire.read();
  Wire.endTransmission();
  long value = (long) b3 * 256;
  value = value +  b4;
  value = value + 0x10000 * b2;
  if ( b2 > 0x10 )  value = value + 0xFF000000;
  //Serial.print ("raw-value= ");
  //Serial.println (value);
  voltageraw = (value * 0.015625 / 2);
  //Serial.print("raw-mV= ");
  //Serial.println(voltageraw, 3);
  slope =  (mVpHhigh - mVpHlow) / (pHbufferhigh - pHbufferlow);
  //Serial.print("slope in mV= ");
  //Serial.println(slope);
  float offset = (7 - pHbufferhigh) * (slope);
  //Serial.print("mV to 7 off= ");
  //Serial.println(offset);
  float voltage = voltageraw - (mVpHhigh + offset);
  //Serial.print("mV zero point calibrated= ");
  //Serial.println(voltage, 3);
  float slopecompensated = slope * (temp + 273.15) / (caltemp + 273.15);
  //Serial.print("slope temp compensated= ");
  //Serial.println(slopecompensated);
  float probecheck = (slope / -59.2) * 100;
  //Serial.print("slope % = ");
  //Serial.println(probecheck);
  phvalue = 7 + (voltage / slopecompensated);
  result = phvalue;
  //Serial.print("pH= ");
  //Serial.println(phvalue, 3);
  //Serial.print("temp in C = ");
  //Serial.println(temp);
  return(result);
  }

float pH_Sensor::passtemp(float tempC) {
temp = tempC;
}

void pH_Sensor::s1read() {
  s1 = Serial.readString();
  if (s1.indexOf("-") >= 0)
  {
    temp = temp - 1;
    EEPROM.put(8, temp);
    EEPROM.get(8, temp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("temp at C ");
    Serial.println(temp);
    delay(2000);
  }

  if (s1.indexOf("+") >= 0)
  {
    temp = temp + 1;
    EEPROM.put(8, temp);
    EEPROM.get(8, temp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("temp in C ");
    Serial.println(temp);
    delay(2000);
  }

  if (s1.indexOf("cal4") >= 0) {
    mVpHlow = voltageraw;
    EEPROM.put(4, mVpHlow);
    EEPROM.get(4, mVpHlow);
    if (temp < 5) {
      pHbufferlow = 4.01;
    }
    if ((temp >= 5) and (temp < 25)) {
      pHbufferlow = 4.00;
    }
    if ((temp >= 25) and (temp < 30)) {
      pHbufferlow = 4.01;
    }
    if ((temp >= 30) and (temp < 35)) {
      pHbufferlow = 4.02;
    }
    if ((temp >= 35) and (temp < 40)) {
      pHbufferlow = 4.03;
    }
    if (temp >= 40) {
      pHbufferlow = 4.04;
    }
    EEPROM.put(16, pHbufferlow);
    EEPROM.get(16, pHbufferlow);
    caltemp = temp;
    EEPROM.put(12, caltemp);
    EEPROM.get(12, caltemp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("calibrated pH4 ");
    Serial.print("-temp at C = ");
    Serial.println(temp);
    delay(2000);
  }

  if (s1.indexOf("cal9.18") >= 0) {
    mVpHlow = voltageraw;
    EEPROM.put(4, mVpHlow);
    EEPROM.get(4, mVpHlow);
    if (temp < 5) {
      pHbufferlow = 9.33;
    }
    if ((temp >= 10) and (temp < 15)) {
      pHbufferlow = 9.33;
    }
    if ((temp >= 15) and (temp < 20)) {
      pHbufferlow = 9.28;
    }
    if ((temp >= 20) and (temp < 25)) {
      pHbufferlow = 9.23;
    }
    if ((temp >= 25) and (temp < 30)) {
      pHbufferlow = 9.18;
    }
    if ((temp >= 30) and (temp < 35)) {
      pHbufferlow = 9.14;
    }
    if ((temp >= 35) and (temp < 40)) {
      pHbufferlow = 9.10;
    }
    if (temp >= 40) {
      pHbufferlow = 9.07;
    }
    EEPROM.put(16, pHbufferlow);
    EEPROM.get(16, pHbufferlow);
    caltemp = temp;
    EEPROM.put(12, caltemp);
    EEPROM.get(12, caltemp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("calibrated pH9.18 ");
    Serial.print("temp at C = ");
    Serial.println(temp);
    delay(2000);
  }

  if (s1.indexOf("cal6.86") >= 0)
  {
    mVpHhigh = voltageraw;
    EEPROM.put(0, mVpHhigh);
    EEPROM.get(0, mVpHhigh);
    if (temp < 10) {
      pHbufferhigh = 6.92;
    }
    if ((temp >= 10) and (temp < 15)) {
      pHbufferhigh = 6.92;
    }
    if ((temp >= 15) and (temp < 20)) {
      pHbufferhigh = 6.90;
    }
    if ((temp >= 20) and (temp < 25)) {
      pHbufferhigh = 6.88;
    }
    if ((temp >= 25) and (temp < 30)) {
      pHbufferhigh = 6.86;
    }
    if ((temp >= 30) and (temp < 35)) {
      pHbufferhigh = 6.85;
    }
    if ((temp >= 35) and (temp < 40))  {
      pHbufferhigh = 6.84;
    }
    if (temp >= 40) {
      pHbufferhigh = 6.83;
    }
    EEPROM.put(20, pHbufferhigh);
    EEPROM.get(20, pHbufferhigh);
    caltemp = temp;
    EEPROM.put(12, caltemp);
    EEPROM.get(12, caltemp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("calibrated pH6.86 ");
    Serial.print("-temp at C = ");
    Serial.println(temp);
    delay(2000);
  }

  if (s1.indexOf("cal7") >= 0)
  {
    mVpHhigh = voltageraw;
    EEPROM.put(0, mVpHhigh);
    EEPROM.get(0, mVpHhigh);
    if (temp < 5) {
      pHbufferhigh = 7.13;
    }
    if ((temp >= 5) and (temp < 10)) {
      pHbufferhigh = 7.10;
    }
    if ((temp >= 10) and (temp < 15)) {
      pHbufferhigh = 7.07;
    }
    if ((temp >= 15) and (temp < 20)) {
      pHbufferhigh = 7.05;
    }
    if ((temp >= 20) and (temp < 25)) {
      pHbufferhigh = 7.03;
    }
    if ((temp >= 25) and (temp < 30)) {
      pHbufferhigh = 7.01;
    }
    if ((temp >= 30) and (temp < 35)) {
      pHbufferhigh = 7.0;
    }
    if ((temp >= 35) and (temp < 40)) {
      pHbufferhigh = 6.99;
    }
    if (temp >= 40) {
      pHbufferhigh = 6.98;
    }
    EEPROM.put(20, pHbufferhigh);
    EEPROM.get(20, pHbufferhigh);
    caltemp = temp;
    EEPROM.put(12, caltemp);
    EEPROM.get(12, caltemp);
    EEPROM.commit(); // uncomment for Arduino 8bit, Atmega328 etc.
    Serial.print("calibrated pH7 ");
    Serial.print("-temp at C = ");
    Serial.println(temp);
    delay(2000);
  }
}
