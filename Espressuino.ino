#include<EEPROM.h>

#define EEPROM_ADDR_COFF_TEMP (0)
#define EEPROM_ADDR_STEAM_TEMP (4)
#define EEPROM_ADDR_ES_TEMP (8)

float targetTemp = 106.0;       // target temperature
float coffTargetTemp = 106.0;       // target temperature
float steamTargetTemp = 155.0;       // target temperature
float energySaverTemp = 80.0;       // target temperature
const float sensorOffset = 13.0;
float smoothTemperature = 0.0;
float displayTemperature = 0.0;

int heatCycle = 0;
bool bSensorError = 0;
int nLoop = 1;

unsigned long startTime;
unsigned long loopTime;
const unsigned long cycleTime = 10;


void setup() {

  initMeas();
  initButtons();
  //initLcd();
  initOled();
  initSsr();
  initSerial();
  initPump();

  //  for (int i = 0 ; i < EEPROM.length() ; i++) {
  //   EEPROM.write(i, 0);
  // }

  // EEPROM.put(EEPROM_ADDR_COFF_TEMP,106.0f);
  // EEPROM.put(EEPROM_ADDR_STEAM_TEMP,160.0f);
  // EEPROM.put(EEPROM_ADDR_ES_TEMP,80.0f);

  EEPROM.get(EEPROM_ADDR_COFF_TEMP,coffTargetTemp);
  EEPROM.get(EEPROM_ADDR_STEAM_TEMP,steamTargetTemp);
  EEPROM.get(EEPROM_ADDR_ES_TEMP,energySaverTemp);

}

void loop() {

  startTime = millis();
  MeasureTemperature();

  if (nLoop > 3)
  {
    //get control inputs
    getBtnStates();
    // LcdStart(smoothTemperature);

    MenuControl();
    //update SSR control

    updatePump();
    //update LCD
    //LcdStart(smoothTemperature);
    //LcdMenu("Set Coffee Temp","Set Steam Temp");
    //updateSerial(coffTargetTemp,smoothTemperature);

    nLoop = 0;
  }

  updateSsr(smoothTemperature);

  // wait n milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  nLoop++;

  loopTime = millis() - startTime;
  //Serial.print("loop Time: ");
  //Serial.println(loopTime);

  if (loopTime < cycleTime){
      delay(cycleTime - loopTime);
  }
}
