#define SSR_PIN (6)

const long interval = 2000;    // interval at which to update temp

boolean ssrState = 0;

float pTerm = 80;
float dTerm = 120;
float iTerm = 7;
float windupGuard = 100;

float pState;
float dState;
float iState;
float curTemp;
float lastTemp;

unsigned long previousMillis = 0;

void initSsr(){  pinMode(SSR_PIN, OUTPUT);
};

void updateSsr(float curTemp) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // save the last time temp was updated
    digitalWrite(SSR_PIN, 1);

    pState = targetTemp - curTemp;
    dState = lastTemp - curTemp;
    iState += pState;
    iState = min(iState, windupGuard);
    iState = max(0, iState);
    heatCycle = (int)(pState * pTerm + dState * dTerm + iState * iTerm);
    heatCycle = min(heatCycle, 2000);
    heatCycle = max(0, heatCycle);

    lastTemp = curTemp;

    if (bSensorError)
    {
      heatCycle = 0;
      digitalWrite(SSR_PIN, 0);
    }
  }

  if (currentMillis - previousMillis >= heatCycle) {
    digitalWrite(SSR_PIN, 0);
    ssrState = 0;
  }
}
