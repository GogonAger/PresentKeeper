#include "Time.h"

//TimerFlagPointer
volatile bool  * timer1OvfFlag;

//Timer ISR
ISR (TIMER1_OVF_vect)
{
  *timer1OvfFlag = true;    //set timerflag
}


Time::Time(const int &h, const int &m, const int &s): hoursn(h), minutesn(m), secondsn(s) {}
Time::~Time(){}

void Time::setHours(const int &s)
{
  hoursn = s;
}
void Time::setMinutes(const int &m)
{
  minutesn = m;
}
void Time::setSeconds(const int &s)
{
  secondsn = s;
}

int Time::getSeconds() {
  return secondsn;
}

int Time::getMinutes() {
  return minutesn;
}

int Time::getHours() {
  return hoursn;
}




long Time::retTimeInSecs()
{
  return (hoursn * 3600L ) + (minutesn * 60L) + (secondsn);
}

/
long Time::getDiffSecs (Time* a, Time* b)
{
  long sek = 0; 

  
  Time * temp = new Time(a->hoursn, a->minutesn, a->secondsn);

  
  if (temp->secondsn <= b->secondsn)
  {
    
    sek += b->secondsn - temp->secondsn;
    temp->secondsn = b->secondsn;
  }
  else
  {
    
    sek += ((60 - temp->secondsn) + b->secondsn);
    temp->minutesn++;
    temp->secondsn = b->secondsn;
  }

	
  if (temp->minutesn <= b->minutesn)
  {
    
    sek += (b->minutesn - temp->minutesn) * 60;
    temp->minutesn = b->minutesn;
  }
  else
  {

    sek += ((60 - temp->minutesn) + b->minutesn) * 60;
    temp->hoursn++;
    temp->minutesn = b->minutesn;
  }

  
  sek += (b->hoursn - temp->hoursn) * 3600;
  temp->hoursn = b->hoursn;

  
  delete temp;
  return sek;
}

bool Time::equal(Time *a, Time *b)
{
  return (a->hoursn == b->hoursn) && (a->minutesn == b->minutesn) && (a->secondsn == b->secondsn);
}



void Time::initSekTimer(volatile bool *const flag)
{
  timer1OvfFlag = flag; //set Flag pointers
  TCCR1A = 0x0;        // set Timer 1 for normal operation
  TCCR1B = 0x04;     // set prescaler for aprox 1s (clk/256) -> 2^16 /(16Mhz/256) = 1,049sek
}


void Time::startSekTimer()
{
  *timer1OvfFlag = false; //Flag reset
  TCNT1 = 0x0000;         //timer wert auf 0 setzen
  TIMSK1 = 0x01;          //enable interrupt
}

//mask timerinterrupt
void Time::stopSekTimer()
{
  TIMSK1 = 0x00; //disable OVFInterrupt
}


