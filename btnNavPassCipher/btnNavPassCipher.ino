/* Arduino menu library example
Oct. 2016 Rui Azevedo (ruihfazevedo@gmail.com) www.r-site.net

Digital keypad (3 buttons) using the menu keyIn driver
*/

#include <Arduino.h>
#include <menu.h>
#include <menuIO/serialIn.h>
#include <menuIO/serialOut.h>
#include <menuIO/altKeyIn.h>
#include <menuIO/chainStream.h>

using namespace Menu;

#define LEDPIN 13
#define MAX_DEPTH 4

#define BTN_SEL 6  // Select button
#define BTN_UP 7   // Up
#define BTN_DOWN 8 // Down

//////////////////////////////////////////////////////////

//list of allowed characters
const char *const digit = "0123456789";
const char *const hexChars MEMMODE = "0123456789ABCDEF";
const char *const alphaNum[] MEMMODE = {" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
//individual character validators
const char *constMEM validData[] MEMMODE = {hexChars, hexChars, hexChars, hexChars};

char pin[] = "                              "; //field will initialize its size by this string length

///////////////////////////////////////////////////////////////////

result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e)
{
  Serial.println("");
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}

int test = 1;

result action1(eventMask e, navNode &nav, prompt &item)
{
  Serial.println("");
  Serial.print("action1 event:");
  Serial.println(e);
  Serial.flush();
  return proceed;
}

result action2(eventMask e)
{
  Serial.println("");
  Serial.print("action2 event:");
  Serial.println(e);
  Serial.flush();
  return quit;
}

// int ledCtrl = LOW;

// result myLedOn()
// {
//   ledCtrl = HIGH;
//   return proceed;
// }
// result myLedOff()
// {
//   ledCtrl = LOW;
//   return proceed;
// }

// TOGGLE(ledCtrl, setLed, "Led: ", doNothing, noEvent, wrapStyle,
//        VALUE("On", HIGH, doNothing, noEvent),
//        VALUE("Off", LOW, doNothing, noEvent));

// int selTest = 0;
// SELECT(selTest, selMenu, "Select", doNothing, noEvent, wrapStyle,
//        VALUE("Zero", 0, doNothing, noEvent),
//        VALUE("One", 1, doNothing, noEvent),
//        VALUE("Two", 2, doNothing, noEvent));
String list[100] = {"tee", "kay", "gsg", "jjf", "tee2", "3kay", "5gsg", "j7jf", "tje7e", "6kay", "5gsg", "vjjf"};
int chooseTest = -1;
CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, wrapStyle,
       VALUE("First", 1, doNothing, noEvent),
       VALUE("Second", 2, doNothing, noEvent),
       VALUE("Third", 3, doNothing, noEvent),
       VALUE("Last", -1, doNothing, noEvent));

MENU(subMenu, "Continue", doNothing, anyEvent, wrapStyle,
     OP("Show Credentials", showEvent, enterEvent),
     OP("Enter New", showEvent, enterEvent),
     OP("action1", action1, enterEvent),
     OP("action2", action2, enterEvent),
     EXIT("<Back"));

MENU(subEnterPinMenu, "Enter Pin", showEvent, noEvent, wrapStyle,
     OP("test 2", showEvent, noEvent),
     EDIT("Pin", pin, alphaNum, showEvent, returnEvent, noStyle),
     OP("test 3", showEvent, noEvent),
     SUBMENU(subMenu),
     EXIT("<Exit"));

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,

     SUBMENU(subEnterPinMenu),
     FIELD(test, "Test", "%", 0, 10, 2, 1, doNothing, noEvent, wrapStyle),
     OP("test 3", showEvent, noEvent),
     OP("Create New User", action2, enterEvent),
     EXIT("<Back"));

keyMap joystickBtn_map[] = {
    {-BTN_SEL, defaultNavCodes[enterCmd].ch, INPUT_PULLUP},
    {-BTN_UP, defaultNavCodes[upCmd].ch, INPUT_PULLUP},
    {-BTN_DOWN, defaultNavCodes[downCmd].ch, INPUT_PULLUP},
};
keyIn<3> joystickBtns(joystickBtn_map);

serialIn serial(Serial);
menuIn *inputsList[] = {&joystickBtns, &serial};
chainStream<2> in(inputsList); //3 is the number of inputs

MENU_OUTPUTS(out, MAX_DEPTH, SERIAL_OUT(Serial), NONE);

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(LEDPIN, OUTPUT);
  joystickBtns.begin();
}

#define SOFT_DEBOUNCE_MS 100

void loop()
{
  nav.poll(); //also do serial input
}
