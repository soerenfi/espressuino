#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int displayWidth;
int* tempVals{ new int[128]{0} };
int ringBufrPtr = 0;


void initOled(void) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_haxrcorp4089_tr); 
  displayWidth = u8g2.getDisplayWidth();

}

void OledStart(float smoothTemperature) {

  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 7);
    u8g2.print(F("Espressuino"));  
    u8g2.setCursor(0, 21);
    u8g2.print(F("Temperature:  "));
    u8g2.print(smoothTemperature); 
    u8g2.print("C");
   
    u8g2.setCursor(0, 28);
    u8g2.print(F("Heatcycle:  "));
    u8g2.print(heatCycle);  
    u8g2.print(" ms");
    
    // u8g2.print("°");
    drawTemp(smoothTemperature);
  } while ( u8g2.nextPage() );
}

void OledMenu( String str1, String str2) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 7);
    u8g2.print(str1);
    u8g2.setCursor(0, 21);
    u8g2.print(str2);
  } while ( u8g2.nextPage() );
};

void OledBezug( unsigned long shotTime) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 7);
    u8g2.print("Single Shot");
    u8g2.setCursor(0, 21);
    u8g2.print((float)(millis() - shotTime)*0.001);
    int shotProgress = (int) ((millis() - shotTime)/30000.0f * displayWidth);
    u8g2.drawBox(0,35,shotProgress,14);
  } while ( u8g2.nextPage() );
};


void drawTemp(float currTemp){

  u8g2.drawFrame(0,32,128,32);

  int curTempVis = (int) (currTemp / 150.0f * 32);

  tempVals[ringBufrPtr] = curTempVis;
  
  for (int tVal = ringBufrPtr + 1; tVal < ringBufrPtr + 126; tVal++)
  {
    int tValt = tVal;
    if (tValt > 127){tValt -= 127;};
    int yVal = 64 - tempVals[tValt]; 
    u8g2.drawPixel(tValt, yVal);
  }
  ringBufrPtr++;
  ringBufrPtr %= 128;
};

