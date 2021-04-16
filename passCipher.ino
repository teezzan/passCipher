#define FORMAT_SPIFFS_IF_FAILED true

#include "FS.h"
#include "SPIFFS.h"
#include <string.h>
#include <stdio.h>
#include <hwcrypto/aes.h>
#include "mbedtls/aes.h"
//#include <BleKeyboard.h>
//BleKeyboard bleKeyboard;

char key[33];
int current_user_number = 0;

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

void setup() {

  Serial.begin(115200);
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  //  bleKeyboard.begin();
}
String test;
void loop()
{
  char cstr[33];
  itoa(random(100000000, 999999999), key, 32);
  randomString();

  
//  save_encode_credential(SPIFFS, "/test.txt", key, password_buf, "teehazzan@gmail.com", "greentestcred" );
  

  
  save_user_credential("fb", password_buf, "teehazzan@gmail.com", "greentestcred" );

//  read_decode_credential(SPIFFS, "/0/fb.txt", key);
  
  read_user_credential("fb" );
  
  //  char cstr1[33];
  //  char cstr2[33];
  //  char cstr0[33];
  //
  //  itoa(random(100000000, 999999999), cstr0, 32);
  //  create_user(SPIFFS, "/0", cstr0 );
  //
  //
  //  itoa(random(100000000, 999999999), cstr1, 32);
  //  create_user(SPIFFS, "/1", cstr1 );
  //
  //  itoa(random(100000000, 999999999), cstr2, 32);
  //  create_user(SPIFFS, "/2", cstr2 );
  //  delay(5000);
  //
  //
  //  Serial.println(getUserNumber(cstr0 ));
  //  Serial.println(getUserNumber(cstr1 ));
  //  Serial.println(getUserNumber(cstr2 ));
  //  //  randomString();

  Serial.print("»» LOOP ENDED. Free heap: "); Serial.println(ESP.getFreeHeap());
  delay(5000);

}
