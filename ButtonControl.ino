// button pins
#define TEMP_SELECTOR_BTN_PIN (2)
#define UP_BTN_PIN (3)
#define SEL_BTN_PIN (5)
#define DOWN_BTN_PIN (7)

long btnDebounce = 250;   // the debounce time, increase if the output flickers

typedef enum {stStart, stMenu, stEdit, stBezug} StateType;
char* StateNames[] {"stStart", "stMenu", "stEdit", "stBezug"};
typedef enum {SetCoffeeTemp, setSteamTemp, setSteamTimeout,setEnergySaver, exitMenu} MenuOptions;
char* MenuOptionNames[] = { "Set Coffee Temp", "Set Steam Temp",
                      "Set Steam Timeout", "Set Energy Saver", "Exit"
                     };
#define LEN_MENU (4)

StateType lastState, currState;

int menuMode = 0;
int menuTimeout = 10000;
int menuTime = 0;

bool tempSelectorBtnState;
bool selBtnState;
bool upBtnState;
bool downBtnState;
bool btnEvent;

int tempSelectorState = 0;      // the current state

long tempSelectorBtnTime = 0;         // the last time the output pin was toggled
long upBtnTime = 0;         // the last time the output pin was toggled
long selBtnTime = 0;         // the last time the output pin was toggled
long downBtnTime = 0;         // the last time the output pin was toggled
    
unsigned long shotTime = 0;


class MenuPtr{
private:
  int ptr = 0;
public:
  int getPtr(){return ptr;};
  int getNext(){return this->ptr + 1;};
  void resetPtr(){this->ptr = 0;};
  MenuPtr& operator++()
  {
    this->ptr += 1;
    this->ptr = this->ptr % LEN_MENU;
    return *this;
  }
  MenuPtr operator++(int)
  {
      MenuPtr tmp(*this);
      operator++(); 
      return tmp;
  }
  MenuPtr& operator--()
  {
    this->ptr -= 1;
    if (this->ptr < 0){this->ptr = (LEN_MENU - 1);};
    return *this;
  }
  const MenuPtr operator--(int)
  {
      MenuPtr tmp(*this);
      operator--(); 
      return tmp;
  }
};

MenuPtr menuPtr = MenuPtr();

void initButtons() {
  pinMode(TEMP_SELECTOR_BTN_PIN, INPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(SEL_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);
  currState = stStart;
}

void getBtnStates() {

  long int currentTime = millis();

  tempSelectorBtnState = 0;
  selBtnState = 0;
  upBtnState = 0;
  downBtnState = 0;
  btnEvent = 0;

  int tempSelectorBtnReading = digitalRead(TEMP_SELECTOR_BTN_PIN);  // read Button
  int upBtnReading = digitalRead(UP_BTN_PIN);
  int selBtnReading = digitalRead(SEL_BTN_PIN);
  int downBtnReading = digitalRead(DOWN_BTN_PIN);


  if (tempSelectorBtnReading == HIGH && (currentTime - tempSelectorBtnTime) > btnDebounce) {
    tempSelectorBtnState = 1;
    btnEvent = 1;
    Serial.println("Temp Selector Pressed!");
    tempSelectorBtnTime = currentTime;
  }

  if (upBtnReading == HIGH && (currentTime - upBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Up Pressed!");
    upBtnState = 1;
    btnEvent = 1;
    upBtnTime = currentTime;
  }
  if (selBtnReading == HIGH && (currentTime - selBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Sel Pressed!");
    selBtnState = 1;
    btnEvent = 1;
    selBtnTime = currentTime;
  }
  if (downBtnReading == HIGH && (currentTime - downBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Down Pressed!");
    downBtnState = 1;
    btnEvent = 1;
    downBtnTime = currentTime;
  }
}

void ResetState()
{
  currState = stStart;
};

void MenuControl() {

  long int currentTime = millis();

  // Serial.println(StateNames[currState]);

  switch ( currState )
  {
    case stStart:
    {
        if (!btnEvent){
          OledStart(smoothTemperature);
        }
        else if (selBtnState)
        {
          currState = stMenu;
          menuPtr.resetPtr();
        }
        else if(upBtnState)
        {
          currState  = stBezug;
          pumpSingleShot();
          shotTime = millis();
        }
    }
    break;
    case stMenu:
    {
        OledMenu(MenuOptionNames[menuPtr.getPtr()], MenuOptionNames[menuPtr.getNext()]);
        if(upBtnState)
        {
          menuPtr++;
        }
        if(downBtnState)
        {
          menuPtr--;
        }
        if(tempSelectorBtnState)
        {
          currState = stStart;
        }
    }
    break;
    case stBezug:
    {
        OledBezug(shotTime);

        if (btnEvent)
        {
          stopShot();
        }
    }
    break;
    case stEdit:
    {

    }
    break;
    default:
    break;
  }

  // if (!btnEvent && currState == stMenu){
  //   if (currentTime - menuTime > menuTimeout)
  //   {
  //     menuTime = currentTime;
  //     menuPtr.resetPtr();
  //     currState = stStart;
  //     return;
  //   }
  //   else
  //   {
  //     return;
  //   }
  // }


  // if (currState == stStart && tempSelectorBtnState == 1) {
  //   if (tempSelectorState == 1) {
  //     tempSelectorState = 0;
  //     targetTemp = coffTargetTemp;
  //   }
  //   else 
  //   {
  //     tempSelectorState = 1;
  //     targetTemp = steamTargetTemp;
  //   }
  // }
}
