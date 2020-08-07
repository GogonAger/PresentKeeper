
#ifndef DISPALY_CLASS
#define DISPALY_CLASS

//----------Display Lib----------
#include <LiquidCrystal_I2C.h>

//---------------------Displaypins--------------------
//Set pins for I2C Display communication

#define I2C_ADDR      0x27  // I2C address of PCF8574A
#define BACKLIGHT_PIN 3     // Backlight
#define En_pin        2
#define Rw_pin        1
#define Rs_pin        0
#define D4_pin        4
#define D5_pin        5
#define D6_pin        6
#define D7_pin        7

//************************************************************************
//------------------DEFINE CONSTANTS-------------------------------------*
//-----------------------------------------------------------------------*
#define ROWS 4  //--> Number of Displayrows------------------------------*
#define COLS 20//--> Number of Displaycolumns----------------------------*
#define COUNTROW 1//--> Rows for countdown-------------------------------*
#define COUNTCOL 9//--> Columns for countdown----------------------------*
//-----------------------------------------------------------------------*
//************************************************************************

class Display
{
  public:
    Display(const byte &pinPower);         
    ~Display();                    
    void clearAll();            
    void outputString(const String&);     
    void outputText(const String&, byte c = 0, const byte &r = 0);   
    void setCursor(const byte&, const byte&);        
    void activate();              //switches display on
    void deactivate();            //switches display off
    void bLightsOn();                 //backlight on
    void bLightsOff();                //backlight off
    void countDown(byte);           //starts countdown from byte

  private:
    void init();                    //(Re-)Initializes
    byte const PIN_POWER;           //Power PIN
    LiquidCrystal_I2C * disp;       //Display control objekt

};
#endif

