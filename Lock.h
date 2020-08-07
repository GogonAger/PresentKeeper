#ifndef LOCK_CLASS
#define LOCK_CLASS

//Include der Board Header
#include <Arduino.h>

//***************************************************************************************
//--------------------------------------------------------------------------------------*
//-------------------------Define Motor constants---------------------------------------*
//--------------------------------------------------------------------------------------*
#define MOTOR_OFF 0xFF//-----PWM Timer Compare value to switch off----------------------*
#define MOTOR_CLOSE 0x14//---PWM Timer Compare value to lock----------------------------*
#define MOTOR_OPEN 0x08//----PWM Timer Compare value to open----------------------------*
#define MOTOR_PWM OCR2B//----Compare Register for PWM-----------------------------------*
#define MOTOR_PWM_PIN 3//----PWM output for OCB2----------------------------------------*
#define MOVE_TIME 1500//-----Time to reach position-------------------------------------*
//--------------------------------------------------------------------------------------*
//***************************************************************************************

class Lock
{
  public:
    Lock(const byte&, const byte&);
    ~Lock();
    void open();   //Open Lock
    void lock();  //lock des Lockes
    bool isOpen();  //true if open
    void overwriteStat(const bool&);  //overwrite status in case status does not resemble physical lock position
    void setLights(const bool&);      //sets lights
    bool lightsStat();          // returns light status

  private:
    bool bOpen, bLights;   //statusbits
    byte const PIN_LG, PIN_LR; // PINS for lights
    void setLights(); //refresh light status
};
#endif

