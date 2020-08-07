# Present keeper
This is a simple Arduino project to use a connected servo motor and I2C controlled display to create a box to keep birthday/weddingday or any other present away from the persons for whom they are intended until finally the day is reached. Why you need a box and an adruino for that? Well usually you don't but maybe you already know you won't be there at the big day but also not willing to give the present later. In this case you can store it in the box and set it up so that it can be accessed until the day has came. And maybe you include some riddles that need to get solved, to make it look even more desireable.

## Hardware
I used this code with an ATmega chip that was not really on an original Arduino setup. However the code should be adoptable to most ATmega series which include the watchdog, and a sufficient sleep mode for energy saving. The display used was a 4 row I2C controlled display.

## Dependencies
To control the display the LiquidCrystalLibrary was used. Other dependencies should be included within the standard components of the arduino IDE.

### Disclaimer
The entire setup was more or less experimental and I am in no doubt that the code is neither optimal designed nor hardware independent at all. Maybe you should lock at it as a kickoff point from which similar developments are possible with reduced effort.
Also the orignial code included tons of german comments and variable names which I decided were not really adding to understandability, so I changed them to english ones. It is more than likley i mist some....