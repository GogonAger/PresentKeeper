#include "Display.h"

//Konstruktor
Display::Display(const byte &pinPower): PIN_POWER(pinPower)
{
  disp = new LiquidCrystal_I2C( I2C_ADDR,   //erzeugen des Steuerungsobjekts
                                En_pin,
                                Rw_pin,
                                Rs_pin,
                                D4_pin,
                                D5_pin,
                                D6_pin,
                                D7_pin,
                                BACKLIGHT_PIN,
                                POSITIVE);
  pinMode(PIN_POWER, OUTPUT); //setzen des PWR Pins als Ausgang
  activate();               //einschalten der Stromversorgung
}

//Neuinitialsisierung fuer Konstruktor und Powerup
void Display::init()
{
  disp->begin(COLS, ROWS); //Kommunikation eropen mit Spalte, Zeile
  clearAll();               //Bei Startup Display loeschen
}

//einschalten der Stromversorgung
void Display::activate()
{
  digitalWrite(PIN_POWER, LOW);   //Stromversorgung einschalten
  delay(300);                      //warten fuer stabilisierung
  init();                         //Neu initialisieren
}

void Display::deactivate()
{
  digitalWrite(PIN_POWER, HIGH);    //Abschalten der Stromversorgung
}

void Display::setCursor(const byte &line, const byte &coulumn)
{
  disp->setCursor(coulumn, line);
}

void Display::outputString(const String& text)
{
  disp->print(text);
}

void Display::clearAll()
{
  disp->clear();
}

void Display::bLightsOn()
{
  disp->backlight();
}

void Display::bLightsOff()
{
  disp->noBacklight();
}

//Zaehlt von n bis 1 und gibt dann Los aus
void Display::countDown(byte n)
{
  if (n < 1)
    return;

  clearAll(); //Display clearen

  //Von n bis 1
  for (; n > 0; n--)
  {
    setCursor(COUNTROW, COUNTCOL);          //Output to center of screen
    outputString(String(n) + "   ");             //Output number
    delay(1000);
  }
  setCursor(COUNTROW, COUNTCOL - 2);       //Display 'Go!' to center of screen
  outputString("GO!");
  delay(1000);
}

void Display::outputText(const String& texter, byte r, const byte &c)
{
   String text = String(texter);
  //Check if stringlength exceeds display
  if (text.length() > 80)
    return;

 
  String temp;
  bool firstOut = true;

  //-----------String processing--------------------
  byte i = 20 - c; //max length until end of row
  setCursor(r, c); //set cursor to starting point

  //until text fits on one line
  //keeping in mind if startpoint is restricted line length is further limited
  while ( ((text.length() > 20) && !firstOut) ||
          ((text.length() > (20 - c)) && firstOut) )
  {

    //Position of last emptyspace before end of line
    while (text.charAt(i) != ' ')
    {
      i--;
      if (i == -1)      //if no whitespace -> too long
        return;
    }

    firstOut = false;  //from no always use entire line
    temp = text;        //Stringcopy for output
    temp.remove(i);     //cut last part and output beginning
    outputString(temp);
    if (++r > 3)        //jump to next row
      return;           //when last line reached-> break
    text.remove(0, i + 1); //Remove parts that were already displayed
    setCursor(r, 0);  //move cursor to beginning of next line
    i = 20;             //line fully available
  }
  outputString(text);  //output last row
}

