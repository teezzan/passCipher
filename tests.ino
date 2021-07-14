
//testReadWrite(String(random(1000, 9999)).c_str());
void testReadWrite(const char * website, char * key) {

  char str[32];
  char buf[4];

  sprintf(buf, "%d", current_user_number);
  strcpy(str, "/");
  strcat(str, buf);
  strcat(str, "/");
  strcat(str, website);
  strcat(str, ".txt\0");

  randomString();
  save_user_credential(website, password_buf, "teehazzan@gmail.com", "greentestcred" , key);

  delay(300);
  read_user_credential(website, key );
  delay(10000);

//  deleteFile(SPIFFS, str);

}

void readTypeCredentials(const char * website, char * key) {
  read_user_credential(website, key);
  if (bleKeyboard.isConnected()) {
    bleKeyboard.print(user_out.email);
    bleKeyboard.write(KEY_TAB);
    delay(500);
    bleKeyboard.print(user_out.password);
    bleKeyboard.write(KEY_RETURN);
  }

}
