
float targetTemp = 107.0;       // target temperature
float coffTargetTemp = 107.0;       // target temperature
float steamTargetTemp = 155.0;       // target temperature
float sensorOffset = 15.0;
float smoothTemperature = 0.0;

float heatCycle = 0;
bool bSensorError = 0;

void setup() {

  initMeas();
  initButtons();
  initLcd();
  initSsr();
  initSerial();

}

void loop() {

  MeasureTemperature();

  //get control inputs
  getBtnStates();

  //update SSR control
  updateSsr(smoothTemperature);

  //update LCD
  //LcdStart(smoothTemperature);
  //LcdMenu("Set Coffee Temp","Set Steam Temp");
  //updateSerial(coffTargetTemp,smoothTemperature);

  // wait n milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(5);

}
