
char* lcase = "abcdefghijklmnopqrstuvwxyz";
char* ucase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char* symbols = "!$%^@#*.:;,?-_<>=";

bool save_encode_credential(fs::FS &fs, const char * path, char * key, const char * password, const char * email, const char * username ) {
  char fullkey[17];
  strcpy(fullkey, key);
  strcat(fullkey, key);
  strncat(fullkey, key, 4);

  struct storage user;
  encrypt(password, fullkey, user.password);

  strcpy(user.email, email);
  strcpy(user.username, username);
  user.style = 1;



  File file = fs.open(path, FILE_WRITE);

  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;

  }

  file.write((const uint8_t *)&user, sizeof(user));
  file.close();




  Serial.println("Saved ");
  Serial.println("\n\n Clear text: = ");
  Serial.print(password);
  Serial.println("\n");

  return true;
}

bool save_user_credential(const char * website, const char * password, const char * email, const char * username, char * key ) {

  char str[32];
  char buf[4];

  sprintf(buf, "%d", current_user_number);
  strcpy(str, "/");
  strcat(str, buf);
  strcat(str, "/");
  strcat(str, website);
  strcat(str, ".txt\0");

  Serial.println(str);
  deleteFile(SPIFFS, str);
  return save_encode_credential(SPIFFS, str, key, password, email, username );
}


bool read_decode_credential(fs::FS &fs, const char * path,  char * key ) {
  char fullkey[17];
  strcpy(fullkey, key);
  strcat(fullkey, key);
  strncat(fullkey, key, 4);

  memset( decipheredTextOutput, 0, sizeof( decipheredTextOutput ) );
  memset( user_out.password, 0, sizeof( user_out.password ) );
  memset( user_out.email, 0, sizeof( user_out.email ) );

  struct storage user;

  //read from memory
  File file = fs.open(path, FILE_READ);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;

  }
  file.read((uint8_t *)&user, sizeof(user));
  file.close();

  //  Serial.println(user.email);

  decrypt(user.password, fullkey, decipheredTextOutput);
  strcpy(user_out.email, user.email);


  for (int i = 0; i < 64; i++) {
    user_out.password[i] = (char)decipheredTextOutput[i];
    //        Serial.print((char)decipheredTextOutput[i]);
  }
  Serial.println("\n\nDeciphered text: = ");
  Serial.print(user_out.password);
  Serial.println("\n");

  return true;
}


bool read_user_credential(const char * website, char * key ) {
  //  char fullkey[17];
  //  strcpy(fullkey, key);
  //  strcat(fullkey, key);
  //  strncat(fullkey, key, 4);

  char str[32];
  char buf[4];

  sprintf(buf, "%d", current_user_number);
  strcpy(str, "/");
  strcat(str, buf);
  strcat(str, "/");
  strcat(str, website);
  strcat(str, ".txt\0");
  Serial.println(str);

  return read_decode_credential(SPIFFS, str, key );

}


bool create_user(fs::FS &fs,  char * key) {
  char fullkey[17];
  strcpy(fullkey, key);
  strcat(fullkey, key);
  strncat(fullkey, key, 4);

  char str[4] = "/";
  sprintf(&str[1], "%d", user_number);

  memset( cipherTextOutput, 0, sizeof( cipherTextOutput ) );
  encrypt("done", fullkey, cipherTextOutput);

  File file = fs.open(str, FILE_WRITE);

  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;
  }


  file.write((const uint8_t *)cipherTextOutput, sizeof(cipherTextOutput));
  file.close();
  Serial.println(" Done Writing ");
  increase_user_number(SPIFFS);


  return true;

}

bool read_user_number(fs::FS &fs) {
  File file = fs.open("/usernum.txt", FILE_READ);

  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;
  }
  int i = 0;
  char num[5] = "";
  while (file.available()) {
    num[i] = file.read();
    i++;
  }
  Serial.print("user_number = ");
  Serial.println(num);
  user_number = atoi (num);

  file.close();
  return true;


}

bool increase_user_number(fs::FS &fs) {
  File file = fs.open("/usernum.txt", FILE_WRITE);

  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;
  }
  if (file.print(String(user_number + 1).c_str())) {
    user_number++;
    Serial.println("File write success");
  } else {
    Serial.println("File write failed");
  }
  file.close();
  return true;


}

bool decrease_user_number(fs::FS &fs) {
  File file = fs.open("/usernum.txt", FILE_WRITE);

  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;
  }
  if (file.print(String(user_number - 1).c_str())) {
    Serial.println("File write success");
    user_number--;
  } else {
    Serial.println("File write failed");
  }
  file.close();
  return true;

}

bool read_create_user(fs::FS &fs, const char * path, char * key) {
  char fullkey[17];
  char done[4];
  strcpy(fullkey, key);
  strcat(fullkey, key);
  strncat(fullkey, key, 4);


  memset( cipherTextOutput, 0, sizeof( cipherTextOutput ) );
  memset( decipheredTextOutput, 0, sizeof( decipheredTextOutput ) );
  //  encrypt("done", fullkey, cipherTextOutput);

  //read from memory
  File file = fs.open(path, FILE_READ);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return false;

  }
  file.read((uint8_t *)&cipherTextOutput, sizeof(cipherTextOutput));
  file.close();


  decrypt(cipherTextOutput, fullkey, decipheredTextOutput);

  if ((char)decipheredTextOutput[0] == 'd' && (char)decipheredTextOutput[1] == 'o' && (char)decipheredTextOutput[2] == 'n' && (char)decipheredTextOutput[3] == 'e' ) {
    //    Serial.println("True User detected");
    return true;
  }

  return false;

}

bool randomString() {
  memset( password_buf, 0, sizeof( password_buf ) );
  int i = 0;

  while (i < 4) {
    password_buf[i] = ucase[random(0, strlen(ucase) - 1)];
    i++;
  }
  while (i < 8) {
    password_buf[i] = symbols[random(0, strlen(symbols) - 1)];
    i++;
  }
  while (i < 12) {
    sprintf(&password_buf[i], "%d", random(0, 9));
    i++;
  }
  while (i < 16) {
    password_buf[i] = lcase[random(0, strlen(lcase) - 1)];
    i++;
  }
  password_buf[16] = '\0';
  return true;
}

int getUserNumber(char * key) {
  int user = -1;
  char str[4] = "/";
  for (int i = 0; i < user_number; i++) {
    sprintf(&str[1], "%d", i);
    if (read_create_user(SPIFFS, str, key )) {
      user = i;
      break;
    }
  }
  return user;
}

void listStoredCredentials(fs::FS &fs, const char * dirname, uint8_t levels, int usernum) {
  String temp = "";
  char str[5] = "/";
  int index = 0;
  sprintf(&str[1], "%d", usernum);
  strcat(&str[2], "/");

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      temp = String(file.name());

      if (temp.indexOf(str) != -1) {
        list[index] = temp.substring(3, temp.length() - 4);
        Serial.print("list index = ");
        Serial.println(list[index]);
        index = index + 1;
      }


    }
    file = root.openNextFile();
  }
}

void readKey() {
  int val = analogRead(INP);
//  Serial.print("val =");
//  Serial.println(val);
  if (val > 2650 && val < 2900) {
    delay(SOFT_DEBOUNCE_MS);
    while (analogRead(INP) != 4095);
    nav.doNav(enterCmd);
    delay(SOFT_DEBOUNCE_MS);
//    Serial.println("Select");
  }
  else if (val > 1800 && val < 1835) {
    delay(SOFT_DEBOUNCE_MS);
    while (analogRead(INP) != 4095);
    nav.doNav(rightCmd);
    delay(SOFT_DEBOUNCE_MS);
//    Serial.println("Right");
  }
  else if (val ==  0) {
    delay(SOFT_DEBOUNCE_MS);
    while (analogRead(INP) != 4095);
    nav.doNav(leftCmd);
    delay(SOFT_DEBOUNCE_MS);
//    Serial.println("Left");
  }
  else if (val > 410 && val < 430) {
    delay(SOFT_DEBOUNCE_MS);
    while (analogRead(INP) != 4095);
    nav.doNav(downCmd);
    delay(SOFT_DEBOUNCE_MS);
//    Serial.println("Up");
  }
  else if (val > 1125 && val < 1195) {
    delay(SOFT_DEBOUNCE_MS);
    while (analogRead(INP) != 4095);
    nav.doNav(upCmd);
    delay(SOFT_DEBOUNCE_MS);
//    Serial.println("Down");
  }

}
