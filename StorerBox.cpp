#include "StorerBox.h"

/* Demo riddle in German.
 * The diffrent riddle types are used a couple of times, then the box is opened*/


//Watchdog ISR
ISR (WDT_vect) {}

StorerBox::StorerBox(Display* disp, Rotary* rotary, Lock* lock): disp(disp), rotary(rotary), lock(lock)
{
  //--------------Switching of analog comparator.-------------
  ACSR &= 0xF7; //Disable interupts
  ACSR |= 0x80; //and ADC

  //--------------Switching of ADC.-------------------------------
  ADCSRA &= 0x7F; //ADC off

  //----------Setting highest sleep mode. No functions needes during sleep except watchdog---------
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  //----------------Prepare Watchdog-----------------------------------------
  MCUSR &= 0xF7;  //Reseting reset bit
  WDTCSR |= 0x18; //Enalbe Watchdog-changes
  WDTCSR = 0x25;  //set Prescaler for 8 sek
}

void StorerBox::startRiddle()
{
  const char *  GLEICHUNGEN[] = {"2 + 3 = 8\0","9 - 3 = 2\0","6 - 1 = 7\0","7 + 2 = 9\0","4 - 1 = 5\0"};
  const char * NAMEN[]  = {"Nicola Tesla\0", "Till Schweiger\0","Thomas Eddison\0", "Harry Nyquist\0","Albert Einstein\0","Heinrich Hertz\0"};
  
  disp->outputText(F("Hallo!                Alles Gute"),1,3);
  waitForButton();
  disp->outputText(F("Um die Box zu oeffnen..."));
  waitForButton();
  disp->outputText(F("Loese jetzt die folgenden Raetsel:"));
  waitForButton();
  disp->outputText(F("Wie lang ist die Pferderennbahn?"));
  box->waitForButton();
  disp->outputText(F("Antwort in 100m"));
  numberQuestion(18);

  disp->clearAll();
  disp->outputText(F("Gut! Jetzt vielleicht was mahematisches?"));
  waitForButton();
  disp->outputText(F("Finde die richtige Gleichung"));
  waitForButton();
  disp->outputText(F("Bereit?"));
  waitForButton();
  reactionTest(GLEICHUNGEN,5,3);
  disp->clearAll();
  disp->outputText(F("Gut gemacht! Fast geschafft!"));
  waitForButton();
  disp->outputText(F("Als naechstes eine Multiple-Choice Frage"));
  waitForButton();
  
  bool first = true;  
  do
  {
    if(!first)
    {   
        disp->clearAll();
        disp->outputText(F("Leider falsch Versuchs nochmal"));
        delay(2000);
    }
    disp->outputText(F("Nach welcher Person wurde das Abtasttheorem benannt?"));
    disp->outputText(F("Nach welcher Person wurde"));
    waitForButton();
    first = false;
    disp->outputText(F("Waehle zwischen A bis F"));
  }while(!multipleChoice(NAMEN,6,3));
  disp->clearAll();
  disp->outputText(F("Bravo! Du hast es raus gefunden."));
  waitForButton();
  disp->outputText(F("Jetzt kommt auch schon die letzte Frage"));
  waitForButton();
  delay(1000);
  disp->outputText(F("Nach einem kurzen Spannungsaufbau"));
  delay(2000);
  waitForButton();
  disp->outputText(F("Wie viele gute Filme gibt es..."));
  waitForButton();
  disp->outputText(F("in denen Matthias Schweighoefer mitspielt"));
  waitForButton();
  disp->outputText(F("Zahl der Filme?"));
    numberQuestion(0);
  disp->clearAll();
  disp->outputText(F("Genau richtig ;P"));
  waitForButton();
  disp->outputText(F("Die Box wird sich jetzt entriegeln"));
  
}
void StorerBox::waitForButton()
{
  rotary->changeFunc(r1);
  rotary->enableRot();
  disp->outputText(USAGEHINT_P,USAGEHINT_C,USAGEHINT_P_R);
  pushFlag = false;
  while(!pushFlag);
  rotary->disableDreh();
  disp->clearAll();  
}


void StorerBox::waitForStart(long& wartesecondsn)
{
  pushFlag = false;
  rotary->changeFunc(r1);
  rotary->enableRot();
  for (; wartesecondsn > 0; wartesecondsn -= 8)
  {
    pushFlag = false;
    goSleep();
    if (pushFlag)
    {
      disp->activate();
      lock->setLights(true);
      disp->outputText("Noch nicht!!!", 1, 3);
      delay(8000);
    }
  }

  pushFlag = false;
  while (!pushFlag)
  {
    goSleep(); //wait until button is depressed
  }
  rotary->disableDreh();
  disp->activate();
  lock->setLights(true);
}

//Box goes to sleep
void StorerBox::goSleep()
{
  //deactivate all
  disp->deactivate();
  lock->setLights(false);

  //Prepare sleep
  sleep_enable();

  //Watchdog Timer enable (Interrupt Mode Enable)
  WDTCSR |= 0x40;

  //Go sleep
  sleep_mode();

  //Wake up
  sleep_disable();

  //Disable wd interrupt
  WDTCSR &= 0xBF;
}


const short StorerBox::readNumber(const byte& x, const byte& y, const String& hint, const bool& mask)
{
  //Output mask
  if (((x + 1) <= 2) && mask)
  {
    disp->setCursor(x + 1, y);
    disp->outputString("^^");
  }

  //Display hint
  if (hint != "")
  {
    disp->setCursor((x + 1 + mask), (COUNTCOL - (hint.length() / 2)));
    disp->outputString(hint);
  }

  //Output rotary usage instructions if space is available
  bool rotaryZeiger = ( mask && (hint != "") && (x == 0)) ||
                    ( mask && (hint == "") && (x <= 1)) ||
                    (!mask && (hint == "") && (x <= 2 ));

  if (rotaryZeiger)
  {
    disp->setCursor(USAGEHINT_C, USAGEHINT_R);
    disp->outputString(USAGEHINT);
  }

  //Enable rotary
  coolNumber = 0;
  rotary->changeFunc(r1);
  pushFlag = false;
  rotary->enableRot();

  
  short temp = 0;
  while (!pushFlag) //until button depressed
  {
    temp = coolNumber;
    //Set cursor position depending on length
    if ((temp < 10) && ( temp >= 0))
    {
      disp->setCursor(x, y);
      disp->outputString("0" + String(temp));
    }
    else if ((temp < 100) && (temp > -10))
    {
      disp->setCursor(x, y);
      disp->outputString(String(temp));
    }
    else if (temp > 100)
      coolNumber = 99;
    else if (temp < -9)
      coolNumber = -9;
  }
  rotary->disableDreh();
  //Delete mask
  disp->setCursor(x + 1, y);
  disp->outputString("  ");

  //Delete hint
  disp->setCursor(x + 2, 0);
  if (hint != "")
    disp->setCursor((x + 1 + mask), 0);
  disp->outputString(EMPTYROW);

  //Delete instructions
  if (rotaryZeiger)
  {
    disp->setCursor(USAGEHINT_C, 0);
    disp->outputString(EMPTYROW);
  }

  //Disable rotary
  rotary->disableDreh();

  return temp;
}

//Reads value in time format
void StorerBox::readTime(int& h, int& m, int& s)
{
  byte ok = 0;	
  const char * az [] = {"Nein\0", "Ja\0"};	
  do
  {
    //Output hint
    disp->clearAll();
    disp->outputText("Verriegelungszeit:");
    disp->outputText("00:00:00", COUNTROW, COUNTCOL - 4);

    //Read values
    h = readNumber(COUNTROW, COUNTCOL - 4, "Stunde", true);
    m = readNumber(COUNTROW, COUNTCOL - 1, "Minute", true);
    s = readNumber(COUNTROW, COUNTCOL + 2, "Sekunde", true);
    
    //Confirm input
    disp->clearAll();
    disp->outputText("Gewaehlte Time:");
    disp->outputText(String(h) + ":" + String(m) + ":" + String(s) + "   OK?", 1, 0);

  } while (multipleChoice(az, 2, ok));	//Repeat until user is satisfied
}

void StorerBox::numberQuestion(const byte &loesung)
{
  coolNumber = 0; 

  
  disp->setCursor(USAGEHINT_C, USAGEHINT_R);
  disp->outputString(USAGEHINT);

  
  disp->setCursor(ANSWERPOS_C, ANSWERPOS_R);
  disp->outputString(F("Antwort:"));

	
  rotary->changeFunc(r1);
  rotary->enableRot();
  pushFlag = false;

//Until correct answer
  bool geloest = false;
  while (readNumber(ANSWERPOS_C, ANSWERPOS_R + 8, "", false) != loesung)
  {
    //Output Wrong
    disp->setCursor(USAGEHINT_C, USAGEHINT_R + 2);
    disp->outputString(F("Wrong!   "));
    delay(2000);
  }
  
  //Correct
  disp->setCursor(USAGEHINT_C, USAGEHINT_R + 2);
  disp->outputString("Correct!    ");
  delay(2000);
  rotary->disableDreh();
}


void StorerBox::reactionTest(const char * dispOwn[], byte const &length, byte const &solution)
{
 
  if (!dispOwn || length == 0)
    return;

  //set ISR
  rotary->changeFunc(r1);
  rotary->enableRot();

  //Init
  bool correct = false;
  bool error = false;

  volatile bool flagTime = false;

  //Calc start position
  const byte S = 6;

  //Timer init
  Time::initSekTimer(&flagTime);
  do {
    for (int i = 0 ; i < length ; i++)
    {
      disp->clearAll();

      
      disp->setCursor(COUNTROW, S);
      disp->outputString(dispOwn[i]);

      //Start timer
      pushFlag = false;
      Time::startSekTimer();
      while (!flagTime && !(correct || error))
      {
        //If button depressed
        if (pushFlag && i == solution)
        {
          correct = true;
        }
        else if (pushFlag)
        {
          error = true;
        }
      }
      //if solved or error
      if (correct || error)
      {
        Time::stopSekTimer();
        if (correct)
        {
          disp->setCursor(USAGEHINT_C, USAGEHINT_R);
          disp->outputString("Richtig!");
        }
        else
        {
          disp->setCursor(USAGEHINT_C, USAGEHINT_R);
          disp->outputString("Falsch!");
        }
        delay(3000);
        error = false;
        break;
      }
    }
  } while (!correct);
  rotary->disableDreh();
}


bool StorerBox::multipleChoice(const char * answer[], byte const &length, byte const &solution)
{
  
  coolNumber = 0;
  pushFlag = false;

  //Enable button
  rotary->changeFunc(r1);
  rotary->enableRot();

  short temp = 0;

  //Until button depressed
  while (!pushFlag)
  {
    temp = coolNumber;
  	
    if (temp >= length)
    {
      coolNumber = 0;
      temp = coolNumber;
    }
    else if (temp < 0)
    {
      coolNumber = length - 1;
      temp = coolNumber;
    }

    //Output answer
    disp->outputText(String((char)(65 + coolNumber)) + ": " + String(answer[coolNumber]) + "    ", 2, 0);
    
  }

  //Disable rotary
  rotary->disableDreh();

  if (coolNumber == solution)
      return true;
  return false;
  
}

void r1 (const byte &welche)
{
  switch (welche)
  {
    case BUTTON_PUSHED: box->pushFlag = true; break;
    case TURNED_RIGHT: box->coolNumber++;  break;
    case TURNED_LEFT: box->coolNumber--;  break;
    default: return;
  }
}
