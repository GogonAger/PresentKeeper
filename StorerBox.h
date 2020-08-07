#ifndef STORERBOX_CLASS
#define STORERBOX_CLASS

//Arduino Includes
#include <Arduino.h>
#include <avr/sleep.h>
//---------------Include components-----------------
#include "Display.h"
#include "Rotary.h"
#include "Lock.h"

//and timer
#include "Time.h"
//************************************************************************
//-----------------------------------------------------------------------*
//-------------Screen positions------------------------------------------*
//-----------------------------------------------------------------------*
#define ANSWERPOS_C 2//--> Answer col for number riddle------------------*
#define ANSWERPOS_R 4//--> Answer row for numberriddlew------------------*
#define USAGEHINT_C 3//--> Usagehint col---------------------------------*
#define USAGEHINT_R 4//--> Usagehint row---------------------------------*
#define USAGEHINT_P_R 17//--> Usagehint (arrow only) row-----------------*
#define USAGEHINT "  (<,OK,>)"//--> Usage hint---------------------------*
#define USAGEHINT_P "-->"//--> Arrow only--------------------------------*
#define EMPTYROW "                    "//--> Empty row-------------------*
//-----------------------------------------------------------------------*
//************************************************************************


void r1(const byte&);
extern bool * timer1OvfFlag;

class StorerBox
{
  public:
    StorerBox(Display* disp, Rotary* rotary, Lock* lock);
    ~StorerBox();                                             
    void numberQuestion(const byte&);		
    void reactionTest(const char * [], const byte&, const byte&);
    bool multipleChoice(const char * [], const byte&, const byte&);	
    void goSleep(); 				
    void readTime(int&,int&,int&);    
    void startRiddle();			
    void waitForStart(long&); 
    void waitForButton();
	
    //------------Friend ISR------------------
    friend void r1 (const byte&);
    
  private:
  
    const short readNumber(const byte&, const byte&, const String& = "", const bool& = false);
    
    
    Display *disp;
    Rotary *rotary;
    Lock *lock;

   
    volatile short coolNumber;
    volatile bool pushFlag;
};

extern StorerBox * box;
#endif

