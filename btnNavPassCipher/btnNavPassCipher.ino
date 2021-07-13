#define I2C_ADDRESS 0x3C
// #include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include <Arduino.h>
#include <menu.h>
#include <menuIO/serialIn.h>
#include <menuIO/serialOut.h>
#include <menuIO/altKeyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/SSD1306AsciiOut.h>
#include <menuIO/serialIO.h>


using namespace Menu;

#define LEDPIN 13
#define MAX_DEPTH 4

#define BTN_SEL 6  // Select button
#define BTN_UP 7   // Up
#define BTN_DOWN 8 // Down

#define menuFont X11fixed7x14
#define fontW 7
#define fontH 15

SSD1306AsciiWire oled;

//////////////////////////////////////////////////////////

//list of allowed characters
const char *const digit = "0123456789";
const char *const hexChars MEMMODE = "0123456789ABCDEF";
const char *const alphaNum[] MEMMODE = {" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
//individual character validators
const char *constMEM validData[] MEMMODE = {hexChars, hexChars, hexChars, hexChars};

char pin[] = "                              "; //field will initialize its size by this string length
int state = 0;

///////////////////////////////////////////////////////////////////

result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e)
{
  Serial.println("");
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}

result showListEvent(eventMask e)
{
  Serial.println("");
  Serial.print("pin event: ");
  Serial.println(e);
  state = 2; //PIn mode

  Serial.println(state);
  Serial.println(pin);
  return proceed;
}

result authEvent(eventMask e)
{

  state = 0;

  if (String(pin) == "                              ") {
    Serial.println("Enter Something");
    return quit;
  } else {
    state = 1;
    Serial.println(state);
    Serial.println(pin);
    return proceed;
  }

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

result showUsernameEvent(eventMask e)
{
  Serial.println("Teezzan");
  state = 4;
  return proceed;
}
result showEmailEvent(eventMask e)
{
  Serial.println("Email@gmail.com");
  state = 5;
  return proceed;
}
result showPasswordEvent(eventMask e)
{
  Serial.println("pasjneini");
  state = 6;
  return proceed;
}
String list[30] = {"tee", "kay", "gsg", "jjf", "trrtee2", "3rrwkay", "5gzzaadsg", "j7jf", "tje7e", "6kdrtdsway", "w5gsg", "vjjf"};
int chooseTest = -1;


MENU(subPassword, "Details", showListEvent, enterEvent, wrapStyle,
     OP("Username", showUsernameEvent, enterEvent),
     OP("Email", showEmailEvent, enterEvent),
     OP("Password", showPasswordEvent, enterEvent),
     OP("Display", doNothing, enterEvent),
     EXIT("<Back"));


MENU(subMenu, "Continue", authEvent, enterEvent, wrapStyle,
     SUBMENU(subPassword),
     OP("Enter New", showEvent, enterEvent),
     EXIT("<Back"));

//OP("Show Credentials", showListEvent, enterEvent),
MENU(subEnterPinMenu, "Enter Pin", showEvent, noEvent, wrapStyle,
     EDIT("Pin", pin, alphaNum, noEvent, returnEvent, noStyle),
     SUBMENU(subMenu),
     EXIT("<Exit"));

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,

     SUBMENU(subEnterPinMenu),
     OP("Create New User", action2, enterEvent),
     EXIT("<Back"));

keyMap joystickBtn_map[] = {
  { -BTN_SEL, defaultNavCodes[enterCmd].ch, INPUT_PULLUP},
  { -BTN_UP, defaultNavCodes[upCmd].ch, INPUT_PULLUP},
  { -BTN_DOWN, defaultNavCodes[downCmd].ch, INPUT_PULLUP},
};
keyIn<3> joystickBtns(joystickBtn_map);


//define output device
idx_t serialTops[MAX_DEPTH] = {0};
serialOut outSerial(Serial, serialTops);

//describing a menu output device without macros
//define at least one panel for menu output
const panel panels[] MEMMODE = {{0, 0, 128 / fontW, 64 / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t tops[MAX_DEPTH] = {0, 0}; //store cursor positions for each level
SSD1306AsciiOut outOLED(&oled, tops, pList, 8, 1+((fontH - 1) >> 3) ); //oled output device menu driver
menuOut* constMEM outputs[] MEMMODE = {&outOLED, &outSerial}; //list of output devices
outputsList out(outputs, sizeof(outputs) / sizeof(menuOut*)); //outputs list

serialIn serial(Serial);
menuIn *inputsList[] = {&joystickBtns, &serial};
chainStream<2> in(inputsList); //3 is the number of inputs


NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

void listCred() {
  bool escape = false;
  String inp = "";
  int index = 0;
  oled.clear();
  while (!escape) {
    inp = Serial.readString();
    if (inp != "") {

      if (inp == "+\n") {
        if (index != 11)
          index++;
      } else if (inp == "-\n") {
        if (index != 0)
          index--;
      }
      else if (inp == "/\n") {
        escape = true;
      }
      Serial.print(index);
      Serial.print(inp);
      Serial.println("end");
      oled.clear();
      oled.setCursor(0, 0);
      oled.println("Crendential List");
      oled.print(">");
      oled.println(list[index]);
      oled.println(list[index + 1]);
      oled.println(list[index + 2]);
    }
  }

  nav.poll();
  state = 0;

}
void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("menu 4.x test"); Serial.flush();
  Wire.begin(4, 5);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(menuFont);

  oled.clear();
  oled.setCursor(0, 0);
  pinMode(LEDPIN, OUTPUT);
  joystickBtns.begin();
}

#define SOFT_DEBOUNCE_MS 100

void loop()
{
  if (state != 2) {
    nav.poll(); //also do serial input
  }
  else {
    listCred();
    nav.doNav(downCmd);
    nav.doNav(downCmd);
    nav.doNav(downCmd);
    nav.doNav(upCmd);
    nav.doNav(upCmd);
    nav.doNav(upCmd);
    
  }


  nav.poll();
}
