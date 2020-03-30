
float targetTemp = 107.0;       // target temperature
float coffTargetTemp = 107.0;       // target temperature
float steamTargetTemp = 155.0;       // target temperature
const float sensorOffset = 15.0;
float smoothTemperature = 0.0;

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
