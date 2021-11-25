#define FORMAT_SPIFFS_IF_FAILED true

#include "FS.h"
#include "SPIFFS.h"
#include <string.h>
#include <stdio.h>
#include <hwcrypto/aes.h>
#include "mbedtls/aes.h"


#include <BleKeyboard.h>
BleKeyboard bleKeyboard;



#include "WiFi.h"
//#include <Http.h>
//#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>
#include <ArduinoJson.h>



int user_number = 0;
uint8_t iv[16] = {0x1f, 0xa8, 0x57, 0xe3, 0x4f, 0x78, 0xbe, 0x68, 0x8c, 0xb2, 0x44, 0x01, 0x27, 0x9b, 0xee, 0xf5};
unsigned char cipherTextOutput[64];
unsigned char decipheredTextOutput[64];
char password_buf[17];


struct cred {
  char  password[64];
  char email[64];
};

struct storage {
  char email[64];
  unsigned char password[64];
  char username[64];
  int style;
};
struct cred user_out;



char key[33] = "9696jr";
char key1[33] = "s079q7";
char key2[33] = "p19c72";
char key3[33] = "it8vu6";
int current_user_number = 0;
String list[100] = {};
bool auth = false;
char passws[33];





//////////////////////////////////////////////////////////////////////

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
#define SOFT_DEBOUNCE_MS 150

#define INP 32
/////////////////////////////////////////////////////////
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

AsyncWebSocketClient * globalClient = NULL;
//////////////////////////////////////////////////////////

//list of allowed characters
const char *const digit = "0123456789";
const char *const hexChars MEMMODE = "0123456789ABCDEF";
const char *const alphaNum[] MEMMODE = {" 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,+-_"};
const char *const alphaunNum[] MEMMODE = {" 0123456789abcdefghijklmnopqrstuvwxyz.,+-_@"};
//individual character validators
const char *constMEM validData[] MEMMODE = {hexChars, hexChars, hexChars, hexChars};

char pin[] = "n46i3j"; //field will initialize its size by this string length
char password[] = "                               ";
char email[] = "t@gmail.com";
char username[] = "tedfeggg";
char website[] = "twitter";
int state = 0;

String ssid="";
String wifi_password="";

int chooseTest = -1;
///////////////////////////////////////////////////////////////////
result doAlert(eventMask e, prompt &item);

class altPrompt: public prompt {
  public:
    altPrompt(constMEM promptShadow& p): prompt(p) {}
    Used printTo(navRoot &root, bool sel, menuOut& out, idx_t idx, idx_t len, idx_t) override {
      return out.printRaw(F("special prompt!"), len);;
    }
};


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

  int escape = 0;
  int inp = -1;;
  int index = 0;
  //  oled.clear();
  while (escape == 0) {
    inp = readKeyReturnVal();
    if (inp != -1) {

      if (inp == 5) {
        if (index != 11)//TBD
          index++;
      } else if (inp == 4) {
        if (index != 0)
          index--;
      }
      else if (inp == 3) {
        escape = -1;
      }
      else if (inp == 2) {
        escape = 1;
      }
      Serial.print(inp);
      oled.clear();
      oled.setCursor(0, 0);
      oled.println("Crendential List");
      oled.print(">");
      oled.println(list[index]);
      oled.println(list[index + 1]);
      oled.println(list[index + 2]);
    }
  }

  state = 0;
  if (escape == -1) {
    return quit;
  }
  else if (escape == 1) {

    read_user_credential(list[index].c_str(), pin);
    return proceed;
  }


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
    oled.clear();
    oled.setCursor(0, 0);

    current_user_number = getUserNumber(pin);
    if (current_user_number == -1) {
      oled.println("User not Found!");
      delay(2000);
      oled.clear();
      return quit;
    } else {
      oled.print("auth as user ");
      oled.println(current_user_number);
      listStoredCredentials(SPIFFS, "/", 0, current_user_number);
      delay(1500);
      oled.clear();
      return proceed;
    }

  }

}

result displayEvent(eventMask e)
{
  bool inp = false;
  oled.clear();
  oled.setCursor(0, 0);
  oled.println(user_out.email);
  oled.println(user_out.password);

  while (!inp) {

    int val = analogRead(INP);
    if (val > 2650 && val < 2900) {
      delay(SOFT_DEBOUNCE_MS);
      while (analogRead(INP) != 4095);
      delay(SOFT_DEBOUNCE_MS);
      inp = true;
    }
  }

  return proceed;


}

result genPasswordEvent(eventMask e)
{
  randomString();
  strcpy(password, password_buf);

  return proceed;


}
result savePasswordEvent(eventMask e)
{
  //do some validation
  Serial.println(website);
  Serial.println(password);
  Serial.println(email);
  Serial.println(username);
  save_user_credential(website, password, email, username , pin);
  listStoredCredentials(SPIFFS, "/", 0, current_user_number);
  return proceed;


}
result logOutEvent(eventMask e)
{
  oled.print("Restarting...");
  delay(2000);
  ESP.restart();
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

result createUserEvent(eventMask e)
{
  Serial.println("");
  Serial.print("createUserEvent event:");
  Serial.println(e);
  Serial.flush();
  state = -1;
  String inp = "";
  char cstr0[33];
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("Creating User...");


  itoa(random(100000000, 999999999), cstr0, 32);
  create_user(SPIFFS, cstr0 );

  Serial.println(cstr0);
  oled.print("Pass: ");
  oled.println(cstr0);

  oled.print("unum: ");
  oled.println(user_number);
  while (state == -1) {

    inp = Serial.readString();
    if (inp != "") {

      if (inp == "/\n") {
        state = 0;
      }
    }
  }

  return proceed;
}

result showUsernameEvent(eventMask e)
{
  Serial.println("Teezzan");
  if (bleKeyboard.isConnected()) {
    bleKeyboard.print("Hello world. oh");
  }
  state = 4;
  return proceed;
}
result showEmailEvent(eventMask e)
{
  Serial.println(user_out.email);
  if (bleKeyboard.isConnected()) {
    String temp = String(user_out.email) + '\0';
    bleKeyboard.print(temp);
    delay(500);
  }
  state = 5;
  return proceed;
}
result showPasswordEvent(eventMask e)
{
  Serial.println(user_out.password);
  if (bleKeyboard.isConnected()) {
    String temp = String(user_out.password) + '\0';
    bleKeyboard.print(temp);
  }
  state = 6;
  return proceed;
}

result onWSServer(eventMask e)
{
  WiFi.begin(ssid.c_str(), wifi_password.c_str() );

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin();

  return proceed;


}



///////////////////////////////////////////////////////////////////


MENU(newDetailsMenu, "Add New", doNothing, enterEvent, wrapStyle,
     EDIT("Website", website, alphaNum, noEvent, returnEvent, noStyle),
     EDIT("Email", email, alphaunNum, noEvent, returnEvent, noStyle),
     EDIT("Username", username, alphaNum, noEvent, returnEvent, noStyle),
     EDIT("Password", password, alphaNum, noEvent, returnEvent, noStyle),
     OP("Gen. Password", genPasswordEvent, enterEvent),
     OP("Save", savePasswordEvent, enterEvent),
     EXIT("<Back"));

MENU(subPassword, "Details", showListEvent, enterEvent, wrapStyle,
     OP("Username", showUsernameEvent, enterEvent),
     OP("Email", showEmailEvent, enterEvent),
     OP("Password", showPasswordEvent, enterEvent),
     OP("Display", displayEvent, enterEvent),
     EXIT("<Back"));


MENU(subMenu, "Continue", authEvent, enterEvent, wrapStyle,
     SUBMENU(subPassword),
     SUBMENU(newDetailsMenu),
     OP("Logout", logOutEvent, enterEvent),
     EXIT("<Back"));

MENU(subEnterPinMenu, "Enter Pin", showEvent, noEvent, wrapStyle,
     EDIT("Pin", pin, alphaNum, noEvent, returnEvent, noStyle),
     SUBMENU(subMenu),
     EXIT("<Exit"));

MENU(mainMenu, "Main menu", doNothing, noEvent, wrapStyle,

     SUBMENU(subEnterPinMenu),
     OP("Create New User", createUserEvent, enterEvent),
     OP("Start WebPage", onWSServer, enterEvent),
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



///////////////////////////////////////////////////













void setup() {

  Serial.begin(115200);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  std::fill_n(list, 100, "");



  Serial.println("menu 4.x test"); Serial.flush();
  Wire.begin(4, 15);
  oled.begin(&Adafruit128x64, I2C_ADDRESS, 16);
  oled.setFont(menuFont);

  oled.clear();
  oled.setCursor(0, 0);
  joystickBtns.begin();


  bleKeyboard.begin();
  read_user_number(SPIFFS);
  read_ssid(SPIFFS);
//  read_pass(SPIFFS);
//  
  //    itoa(random(100000000, 999999999), key, 32);
  //    randomString();
  //    save_user_credential("fb", password_buf, "teehazzan@gmail.com", "greentestcred", key );
  //  listStoredCredentials(SPIFFS, "/", 0, 0);
  //  testReadWrite(String(random(1000, 9999)).c_str(), pin);
}

void loop()
{
  //  readTypeCredentials("3928", pin );
  //  delay(2000);
  //  readKey();
  nav.poll();

}
