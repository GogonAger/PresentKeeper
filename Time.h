#ifndef TIMER_CLASS
#define TIMER_CLASS


#include <AVR/interrupt.h>

class Time
{
  public: 
    Time(const int &h, const int &m, const int &s);
    ~Time();

    void setHours(const int &s);
    void setMinutes(const int &m);
    void setSeconds(const int &s);
    
    int getSeconds();            
    int getMinutes();             
    int getHours();            
    long retTimeInSecs();  

    static long getDiffSecs (Time* a, Time* b); 

    static void initSekTimer(volatile bool *const flagT);  
    static void startSekTimer();          
    static void stopSekTimer();          
    
    bool equal(Time *, Time *);

  private:
  
    int hoursn, minutesn, secondsn;
}; 

#endif

