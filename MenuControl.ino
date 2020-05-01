// button pins
#define TEMP_SELECTOR_BTN_PIN (2)
#define SEL_BTN_PIN (3)
#define UP_BTN_PIN (5)
#define DOWN_BTN_PIN (7)

#include <EEPROM.h>

unsigned long btnDebounce = 200;   // the debounce time
unsigned long standbyTimeout = 3600000;   // 60min
//unsigned long standbyTimeout = 15000;   // 5min

typedef enum {stStart, stMenu, stMenuSetValue, stBezug, stStandby, stSensorErr} StateType;
char* StateNames[] {"stStart", "stMenu", "stMenuSetValue", "stBezug", "stStandby", "stBezug"};
typedef enum {setCoffeeTemp, setSteamTemp, setSteamTimeout ,setEnergySaver} MenuOptions;
char* MenuOptionNames[] = { "Set Coffee Temp", "Set Steam Temp",
                      "Set Steam Timeout", "Set Energy Saver", "Exit"};
#define LEN_MENU (4)

void setCurrState(StateType state);

StateType lastState, currState;
MenuOptions selMenuOption;

unsigned long menuTimeout = 10000;
unsigned long menuTime = 0;

float setVal;

bool tempSelectorBtnState;
bool selBtnState;
bool upBtnState;
bool downBtnState;
bool btnEvent;
bool powerSaverMode;

bool tempSelectorState = 0;      // the current state

unsigned long tempSelectorBtnTime = 0;         // the last time the output pin was toggled
unsigned long upBtnTime = 0;         // the last time the output pin was toggled
unsigned long selBtnTime = 0;         // the last time the output pin was toggled
unsigned long downBtnTime = 0;         // the last time the output pin was toggled
unsigned long btnEventTime = 0;
unsigned long shotTime = 0;

class MenuPtr{
private:
  int _ptr = 0;
public:
  int getPtr(){return this->_ptr;};
  int getNext()
  {
    MenuPtr tmp(*this);
    tmp++;
    return tmp._ptr;
  };
  int getPtr(int step)
  {
    MenuPtr tmp(*this);
    for(int i=0; i < step; i++) {tmp++;};
    return tmp._ptr;
  };
  void resetPtr(){_ptr = 0;};
  MenuPtr& operator++()
  {
    _ptr += 1;
    _ptr = _ptr % LEN_MENU;
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
    _ptr -= 1;
    if (_ptr < 0){_ptr = (LEN_MENU - 1);};
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
    btnEventTime = currentTime;
    Serial.println("Temp Selector Pressed!");
    tempSelectorBtnTime = currentTime;
  }

  if (upBtnReading == HIGH && (currentTime - upBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Up Pressed!");
    upBtnState = 1;
    btnEvent = 1;
    btnEventTime = currentTime;
    upBtnTime = currentTime;
  }
  if (selBtnReading == HIGH && (currentTime - selBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Sel Pressed!");
    selBtnState = 1;
    btnEvent = 1;
    btnEventTime = currentTime;
    selBtnTime = currentTime;
  }
  if (downBtnReading == HIGH && (currentTime - downBtnTime) > btnDebounce) {
    menuTime = currentTime;
    Serial.println("Down Pressed!");
    downBtnState = 1;
    btnEvent = 1;
    btnEventTime = currentTime;
    downBtnTime = currentTime;
  }
}

void setCurrState(StateType state)
{
  currState = state;
};

void SetStateStart()
{
  currState = stStart;
  targetTemp = coffTargetTemp;
  menuPtr.resetPtr();
};


void setStateSensorErr()
{
  currState = stSensorErr;
};

void MenuControl() {

  long int currentTime = millis();

  switch ( currState )
  {
    case stStart:
    {        
        if (!btnEvent){
          OledStart(displayTemperature, targetTemp - sensorOffset, tempSelectorState);
          if (currentTime - btnEventTime > standbyTimeout)
          {
            currState = stStandby;
          }
        }
        else if (selBtnState)
        {
            currState = stMenu;
        }
        else if(upBtnState)
        {
          currState  = stBezug;
          pumpSingleShot();
          shotTime = millis();
        }        
        else if(downBtnState)
        {
          currState  = stBezug;
          pumpSingleShot();
          shotTime = millis();
        }
        else if(tempSelectorBtnState)
        {
            if (tempSelectorState == 1) {
              tempSelectorState = 0;
              targetTemp = coffTargetTemp;
            }
            else 
            {
              tempSelectorState = 1;
              targetTemp = steamTargetTemp;
            }
        }
    }
    break;
    case stMenu:
    {
        OledMenu(MenuOptionNames[menuPtr.getPtr()], MenuOptionNames[menuPtr.getNext()],MenuOptionNames[menuPtr.getPtr(2)],MenuOptionNames[menuPtr.getPtr(3)]);
        if(upBtnState)    {menuPtr++;}
        if(downBtnState)  {menuPtr--;}
        if(tempSelectorBtnState)  {SetStateStart();}
        if(selBtnState)
        {
          setCurrState(stMenuSetValue);
          selMenuOption = menuPtr.getPtr();
        }
    }
    break;
    case stMenuSetValue:
    {
      switch (selMenuOption)
      {
          case setCoffeeTemp:
          {
              OledSetValue(MenuOptionNames[selMenuOption],coffTargetTemp - sensorOffset);
              if(downBtnState){coffTargetTemp += 0.5f;}
              if(upBtnState){coffTargetTemp -= 0.5f;}
              if(selBtnState){EEPROM.put(EEPROM_ADDR_COFF_TEMP,coffTargetTemp); setCurrState(stMenu);}
              if(tempSelectorBtnState){setCurrState(stMenu);}
          }
          break;
          case setSteamTemp:
          {
              OledSetValue(MenuOptionNames[selMenuOption],steamTargetTemp - sensorOffset);
              if(downBtnState){steamTargetTemp++;}
              if(upBtnState){steamTargetTemp--;}
              if(selBtnState){EEPROM.put(EEPROM_ADDR_STEAM_TEMP,steamTargetTemp); setCurrState(stMenu);}
              if(tempSelectorBtnState){setCurrState(stMenu);}
          }
          break;
          case setEnergySaver:
          {
              OledSetValue(MenuOptionNames[selMenuOption],energySaverTemp - sensorOffset);
              if(downBtnState){energySaverTemp++;}
              if(upBtnState){energySaverTemp--;}
              if(selBtnState){EEPROM.put(EEPROM_ADDR_ES_TEMP,energySaverTemp); setCurrState(stMenu);}
              if(tempSelectorBtnState){setCurrState(stMenu);}
          }
          break;
      }
    }
    break;
    case stBezug:
    {
        OledBezug(shotTime);

        if (btnEvent)
        {
          stopShot();
          SetStateStart();
        }
    }
    break;
    case stStandby:
    {
      targetTemp = energySaverTemp;
      OledStandby(displayTemperature, targetTemp - sensorOffset);
      if (btnEvent)
      {
        SetStateStart();      
      }
    }
    break;
    case stSensorErr:
    {
      bSensorError = 1;
      OledSensorErr();
    }
    break;
    default:
    break;
  }
}
