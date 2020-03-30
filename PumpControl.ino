#define PUMP_PIN (10)

const long singleShotTime = 30000;    // interval at which to update temp

boolean ssrPumpState = 0;
boolean ongoingShot;
unsigned long shotStartTime = 0;

void initPump()
{ 
    pinMode(PUMP_PIN, OUTPUT);
    ongoingShot = false;
};

void pumpSingleShot() {

    unsigned long curTime = millis();

    if (ongoingShot == false) 
    {
        Serial.println("shot started");
        ongoingShot = true;
        ssrPumpState = 1;
        shotStartTime = curTime;
        digitalWrite(PUMP_PIN, 1);
    }
}
void updatePump(){

    if (ongoingShot == true)
    {
        unsigned long curTime = millis();

        if ((curTime - shotStartTime) > singleShotTime) 
        {
            Serial.println("shot stopped");
            ongoingShot = false;
            digitalWrite(PUMP_PIN, 0);
            ResetState();
            ssrPumpState = 0;
        }
    }
}
void stopShot(){

    if (ongoingShot == true)
    {
        Serial.println("shot stopped");
        ongoingShot = false;
        digitalWrite(PUMP_PIN, 0);
        ResetState();
        ssrPumpState = 0;
    }
}