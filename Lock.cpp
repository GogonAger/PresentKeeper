#include "Lock.h"

Lock::Lock(const byte &pinL, const byte &pinR): PIN_LG(pinL), PIN_LR(pinR)
{
  //Setting PIN-Modes
  pinMode(PIN_LG, OUTPUT);
  pinMode(PIN_LR, OUTPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);

  //Setting up PWM Mode on Timer2
  TCCR2A = 0x23;      //Setting Output compare mode on Pin PD3 AND fast-PWM Mode
  TCCR2B = 0x07;      //1024 prescaling and fast PWM with signal on OCRA
  MOTOR_PWM = MOTOR_OFF;       //starting with Signal off

  //Setting Status Bits
  bOpen = true;
  bLights = true;
  setLights();
}

void Lock::open()
{
  //Only if not open
  if (bOpen)
    return;

  //Open
  MOTOR_PWM = MOTOR_OPEN;
  delay(MOVE_TIME);
  MOTOR_PWM = MOTOR_OFF;

  //Set bLights
  bOpen = true;
  setLights();
}

void Lock::lock()
{
  //only if not locked
  if (!bOpen)
    return;

  //Lichter setzen
  bOpen = false;
  setLights();

  //SchlieÃŸen
  MOTOR_PWM = MOTOR_CLOSE;
  delay(MOVE_TIME);
  MOTOR_PWM = 0xFF;

  //Lichter setzen
  bOpen = false;
  setLights();
}

bool Lock::isOpen()
{
  return bOpen;
}

void Lock::overwriteStat(const bool &stat)
{
  bOpen = stat;
}

void Lock::setLights()
{
  if (bLights)
  {
    if (bOpen)
    {
      digitalWrite(PIN_LG, LOW);
      digitalWrite(PIN_LR, HIGH);
    }
    else
    {
      digitalWrite(PIN_LG, HIGH);
      digitalWrite(PIN_LR, LOW);
    }
  }
  else
  {
    digitalWrite(PIN_LG, HIGH);
    digitalWrite(PIN_LR, HIGH);
  }
}

void Lock::setLights(const bool &b)
{
  bLights = b;
  setLights();
}

bool Lock::bLightsStat()
{
  return bLights;
}

