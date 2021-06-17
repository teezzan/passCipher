#define FORMAT_SPIFFS_IF_FAILED true

#include "FS.h"
#include "SPIFFS.h"
#include <string.h>
#include <stdio.h>
#include <hwcrypto/aes.h>
#include "mbedtls/aes.h"
#include <BleKeyboard.h>
BleKeyboard bleKeyboard;


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



char key[33];
char key1[33] = "s079q7";
char key2[33] = "p19c72";
char key3[33] = "it8vu6";
int current_user_number = 0;

void setup() {

  Serial.begin(115200);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  bleKeyboard.begin();
  
  read_user_number(SPIFFS);
  itoa(random(100000000, 999999999), key, 32);
  randomString();
  save_user_credential("fb", password_buf, "teehazzan@gmail.com", "greentestcred", key2 );
}
String test;
void loop()
{
  readTypeCredentials("fb", key2 );
  
  


  Serial.print("»» LOOP ENDED. Free heap: "); Serial.println(ESP.getFreeHeap());
  delay(5000);

}
