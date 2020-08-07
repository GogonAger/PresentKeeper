#ifndef ROTARY_CLASS
#define ROTARY_CLASS

//-----------------------Include der Systemheader------------------------------
#include <AVR/interrupt.h>
#include <Arduino.h>

//-----------------------Include der StorerBox-------------------------------

extern void (*EVENTFUNC)(const byte&);

//*****************************************************************************
//----------------------------------------------------------------------------*
//-------------------Define ISR return values----------------------*
//----------------------------------------------------------------------------*
#define BUTTON_PUSHED 0 //-----> Button depressed-----------------------*
#define TURNED_RIGHT 1  //-----> Rotation CW-----------------------------------*
#define TURNED_LEFT 2   //-----> Rotation CCW----------------------------------*
//----------------------------------------------------------------------------*
//*****************************************************************************

class Rotary
{
  public:
    
    Rotary(const byte &pinA, const byte &pinB, const byte &pinTast, void (*inputFunction)(const byte &inputType));
    ~Rotary();   
    void changeFunc(void (*inputFunction)(const byte &inputType));   //Sets ISR to call
    void enableRot();      //enables interrupts
    void disableRot();     //disables interrupts
    
    
  private:
    
};
#endif

