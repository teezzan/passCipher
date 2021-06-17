
//testReadWrite(String(random(1000, 9999)).c_str());
void testReadWrite(const char * website){

  char str[32];
  char buf[4];

  sprintf(buf, "%d", current_user_number);
  strcpy(str, "/");
  strcat(str, buf);
  strcat(str, "/");
  strcat(str, website);
  strcat(str, ".txt\0");
  
  randomString();
  save_user_credential(website, password_buf, "teehazzan@gmail.com", "greentestcred" );

  delay(300);
  read_user_credential(website );
  delay(1000);
  
  deleteFile(SPIFFS, str);
  
}
