// button pins
#define TEMP_SELECTOR_BTN_PIN (2)
#define UP_BTN_PIN (3)
#define SEL_BTN_PIN (4)
#define DOWN_BTN_PIN (5)

int tempSelectorState = 0;      // the current state of the output pin
long tempSelectorBtnTime = 0;         // the last time the output pin was toggled
long upBtnTime = 0;         // the last time the output pin was toggled
long selBtnTime = 0;         // the last time the output pin was toggled
long downBtnTime = 0;         // the last time the output pin was toggled
long btnDebounce = 250;   // the debounce time, increase if the output flickers

enum MenuOptions {SetCoffeeTemp, SetSteamTemp, SetSteamTimeout};

typedef enum {stStart, stMenuCtp, stMenuStp, stMenuSto, stMenuEs} StateType;
char* StateNames[] = {"Start", "Set Coffee Temp", "Set Steam Temp",
                      "Set Steam Timeout", "Set Energy Saver",
                     };

StateType lastState, currState, nextState;

int menuMode = 0;
int menuTimeout = 10000;
int menuTime = 0;
int menuPtr = 0;

void initButtons() {
  pinMode(TEMP_SELECTOR_BTN_PIN, INPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(SEL_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);

  currState = stStart;
}

void getBtnStates() {

  long int currentTime = millis();

  int tempSelectorBtnReading = digitalRead(TEMP_SELECTOR_BTN_PIN);  // read Button
  int tempSelectorBtnState = 0;
  int upBtnReading = digitalRead(UP_BTN_PIN);
  int selBtnReading = digitalRead(SEL_BTN_PIN);
  int downBtnReading = digitalRead(DOWN_BTN_PIN);
  bool selBtnState = 0;
  bool upBtnState = 0;
  bool downBtnState = 0;

  if (tempSelectorBtnReading == LOW && (currentTime - tempSelectorBtnTime) > btnDebounce) {
    tempSelectorBtnState = 1;
    Serial.println("Temp Selector Pressed!");
  }

  if (upBtnReading == LOW && (currentTime - upBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Up Pressed!");
    upBtnState = 1;
    upBtnTime = currentTime;
  }
  if (selBtnReading == LOW && (currentTime - selBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Sel Pressed!");
    selBtnState = 1;
    selBtnTime = currentTime;
  }
  if (downBtnReading == LOW && (currentTime - downBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Down Pressed!");
    downBtnState = 1;
    downBtnTime = currentTime;
  }


  if (menuMode == 0 && selBtnState == 1) {
    menuMode = 1;
    menuTime = currentTime;
    Serial.println("Menu 1");
  }

  if (menuMode == 0) {
    LcdStart(smoothTemperature);
  }

  if (menuMode == 1) {
    LcdMenu(StateNames[currState], StateNames[nextState]);
  }


  if (menuMode == 1 && upBtnState == 1 ) {
    menuTime = currentTime;
    currState = currState + 1;
    currState  = currState % 4;
    Serial.print("MenuPtr ");
    Serial.println(menuPtr);
  }

  if (menuMode == 1 && tempSelectorBtnState == 1 ) {
    menuTime = currentTime;
    menuPtr++;
    menuPtr = menuPtr % 3;
    Serial.print("MenuPtr ");
    Serial.println(menuPtr);
  }

  if (currentTime - menuTime > menuTimeout) {
    menuMode = 0;
    Serial.print("Menu 0");

  }

  if (menuMode == 0 && tempSelectorBtnState == 1) {
    if (tempSelectorState == 1) {
      tempSelectorState = 0;
      targetTemp = coffTargetTemp;
    }
    else {
      tempSelectorState = 1;
      targetTemp = steamTargetTemp;
    }
    tempSelectorBtnTime = currentTime;
  }
}


void ButtonControl() {
  currState = stStart;
}
