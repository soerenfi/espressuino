#include <ExponentialFilter.h>

const int THERMISTORPIN = A0;  // Analog input pin that the potentiometer is attached to
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT (3450)
// value of series resistor
#define SERIESRESISTOR (10000)
// resistance at 25 degrees C
#define THERMISTORNOMINAL (10000)
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL (25)

float offsetTemp = 0.0;        // offset temp boiler to portafilter
float sensorErrorThreshold = -20.0;        // offset temp boiler to portafilter

// first arg is the weight (20 => 20%)
// sec arg is the initial value of the filter
ExponentialFilter<float> TempFilter(5, targetTemp);

void initMeas() {
  analogReference(EXTERNAL);    // set reference Voltage AREF
}

void MeasureTemperature() {
  // read the analog in value:
  float sensorValue = analogRead(THERMISTORPIN);
  float curResistance = (1023 / sensorValue) - 1;     // (1023/ADC - 1)
  curResistance = SERIESRESISTOR / curResistance;  // 10K / (1023/ADC - 1)
  float rawTemperature;
  rawTemperature = curResistance / THERMISTORNOMINAL;     // (R/Ro)
  rawTemperature = log(rawTemperature);                  // ln(R/Ro)
  rawTemperature /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  rawTemperature += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  rawTemperature = 1.0 / rawTemperature;                 // Invert
  rawTemperature -= 273.15;                         // convert to C
  rawTemperature -= offsetTemp;

  if (rawTemperature < sensorErrorThreshold)
  {
    bSensorError = 1;   // Sensor Error prevention
    setStateSensorErr();
  }

  TempFilter.Filter(rawTemperature);   // filter current temp
  smoothTemperature = TempFilter.Current();
}
