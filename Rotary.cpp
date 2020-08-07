#include "Rotary.h"

//------------------Globals for ISR----------------
byte  PIN_A, PIN_B, PIN_Tast;       //PINs
long button_milli;
long rotary_milli;

//------Functionhandle for ISRs------
void (*EVENTFUNC)(const byte&);

//PIN A Interrupt on PCINT0
ISR (PCINT0_vect)
{
  if (!(digitalRead(PIN_B)  || digitalRead(PIN_A))) //PIN_B already triggered --> Rotating CCW
  {
    if (millis() - rotary_milli > 50)
    {
      rotary_milli = millis();
      EVENTFUNC(TURNED_LEFT);
    }
  }
}

//PIN B or Button Interrupt on PCINT2
ISR (PCINT2_vect)
{
  if (!(digitalRead(PIN_B) || digitalRead(PIN_A)))  //PIN_A already triggered --> Rotating CW
  {
    if (millis() - rotary_milli > 50)
    {
      rotary_milli = millis();
      EVENTFUNC(TURNED_RIGHT);
    }
  }
  else if (!digitalRead(PIN_Tast))                  //Button was depressed
  {
    if ((millis() - button_milli) > 500)
    {
      EVENTFUNC(BUTTON_PUSHED);
      button_milli = millis();
    }
  }
}

Rotary::Rotary(const byte &pinA, const byte &pinB, const byte &pinTast, void (*inputFunction)(const byte &inputType))
{
  //--------------Setting ISR globals-----------------
  PIN_A = pinA;
  PIN_B = pinB;
  PIN_Tast = pinTast;
  EVENTFUNC = inputFunction;


  //--------------Init Rotary Pins---------------------
  pinMode(PIN_Tast, INPUT_PULLUP);
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  //--------------Init PCInterrupts-------------------------

  //PIN Mask 0 for PIN A (PCINT0)
  if ((PIN_A < 8) || (PIN_A > 13))      //inop pins
    return;
  PCMSK0 = (0x01 << (PIN_A - 8));

  //PIN Mask for PIN B UND Taster (PCINT2)
  if ((PIN_B < 2) || (PIN_B > 7))       //inop pins
    return;
  PCMSK2 = (0x01 << PIN_B);

  //Taster
  if ((PIN_Tast < 2) || (PIN_Tast > 7)) //inop pins
    return;
  PCMSK2 |= (0x01 << PIN_Tast);
}

void Rotary::changeFunc (void (*inputFunction)(const byte &inputType))
{
  EVENTFUNC = inputFunction;
}

void Rotary::enableRot()
{
  PCICR = 0x05;          //Enable Pin-Change Interrupt 0 and 2
}

void Rotary::disableRot()
{
  PCICR = 0x00;         //Disable Pin-Change Interrupt 0 and 2
}

