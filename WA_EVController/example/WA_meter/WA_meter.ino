#include <WAEVController.h>

WAEVCONTROLLER WA_EV;

String voltageA, voltageB, voltageC;
String currentA, currentB, currentC;
String powerA, powerB, powerC;
String energy;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  Serial2.begin(115200);
  while (! Serial2);
  delay(1000);
  WA_EV.init(Serial2);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // Set phase configuration meter
  // pass 1 is set 1 phase
  // pass 3 is set 3 phase
  if(WA_EV.setPhaseMeter(1))
  {
    Serial.println("Set phase meter OK");
  }
  Serial.println();
}

void loop() {
  
  if(WA_EV.readParameter1Phase(voltageA, currentA, powerA, energy))
  {
    Serial.print("Voltage = "); Serial.println(voltageA);
    Serial.print("Current = "); Serial.println(currentA);
    Serial.print("Power = ");   Serial.println(powerA);
    Serial.print("Energy = ");  Serial.println(energy);
  }
  Serial.println();
/*
 if(WA_EV.readParameter3Phase(voltageA, voltageB, voltageC, currentA, currentB, currentC, powerA, powerB, powerC, energy))
  {
    Serial.print("Voltage A = "); Serial.println(voltageA);
    Serial.print("Voltage B = "); Serial.println(voltageB);
    Serial.print("Voltage C = "); Serial.println(voltageC);
    Serial.print("Current A = "); Serial.println(currentA);
    Serial.print("Current B = "); Serial.println(currentB);
    Serial.print("Current C = "); Serial.println(currentC);
    Serial.print("Power A = ");   Serial.println(powerA);
    Serial.print("Power B = ");   Serial.println(powerB);
    Serial.print("Power C = ");   Serial.println(powerC);
    Serial.print("Energy = ");    Serial.println(energy);
  }
  Serial.println();
*/
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}
