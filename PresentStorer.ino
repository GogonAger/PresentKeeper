/*
  Author:             Lukas Scholz

  Startup program.
  Initializes everthing. Sets pin-numbers for connected parts

  ---------------------REQUIREMENTS----------------------------------------
   ->Motor PWM has to be connected to PD3 to be able to use timer 2 Fast-Mode PWM

   ->I2C Display has to be connected to I2C SDL und SCL

   ->Rotary encoder needs to use one of the PC0 pins as PIN A
     and the button function at PC2
*/

//----------------Includes---------------------------------
#include "StorerBox.h"

//*********************************************************
//---------------------PINDEFINITIONS---------------------*
//--------------------------------------------------------*

#define BUTTON 5 //------------Button on Rotary Encoder---*
#define LEDG 9   //-----------------Green LED-------------*
#define LEDR 10  //------------------RED LED--------------*
#define ROTARY_PIN_A 8//-------PIN A of RotaryEnc-----*
#define ROTARY_PIN_B 7//-------PIN B of RotaryEnc-----*
#define DISPLAY_PWR 6  //------------MotorPWM-(PD3)-------*
#define MOTOR_PWR 11 //-------------Motor Power-----------*
//--------------------------------------------------------*
//*********************************************************

//----------------Box object------------------
StorerBox *box;

//---------------Components-------------------------------
Display * disp;
Rotary* rotary;
Lock* lock;

//-------------Inits--------------
void setup()
{
  
  disp = new Display(DISPLAY_PWR);
  rotary = new Rotary(ROTARY_PIN_A, ROTARY_PIN_B, BUTTON, r1);
  lock = new Lock(LEDG, LEDR);

  
  box = new StorerBox(disp, rotary, lock);
delay(500);
}


//------------------Main loop----------------------
void loop()
{

  //----------------Start------------------------
  //Wait for start
  long waitSec = 0;
  box->waitForStart(waitSec);

  //Wartezeit einlesen
  int hours, minutes, seconds;
  box->readTime(hours, minutes, seconds);

  Time * wartezeit = new Time(hours, minutes, seconds);
  waitSec = wartezeit->retTimeInSecs();
  delete wartezeit;
  //Box verlock
  lock->lock();
  box->waitForStart(waitSec);

  //RÃ¤tsel starten
  box->startRiddle();
  lock->open();
  disp->clearAll();
  disp->outputText(F("Opened"),2,3);
  delay(10000);

}

