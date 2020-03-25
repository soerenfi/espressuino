
unsigned long previousSerialMillis = 0;
const long serialInterval = 100;
void initSerial() {
  Serial.begin(9600);           // initialize serial communications at 9600 bps
}

void updateSerial(float coffTargetTemp, float smoothTemperature) {
  previousSerialMillis = millis();
  Serial.print(coffTargetTemp);
  Serial.print(" ");
  Serial.println(smoothTemperature);
}

void serialPrint(String string){
    Serial.println(string);
  }
