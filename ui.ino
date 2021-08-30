


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


void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {

  if (type == WS_EVT_CONNECT) {

    Serial.println("Websocket client connection received");
    Serial.println("Generating PassKey");
    globalClient = client;
    itoa(random(100000000, 999999999), passws, 32);
    Serial.println(passws);
    auth = true;

  } else if (type == WS_EVT_DISCONNECT) {

    Serial.println("Websocket client connection finished");
    globalClient = NULL;
    auth = false;

  } else if (type == WS_EVT_DATA) {

    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if (info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      for (size_t i = 0; i < info->len; i++) {
        msg += (char) data[i];
      }
      if (msg == "ping")
      {
        ws.textAll("Pong Pong PONg, PUnk!");
        return;
      }

      Serial.println(msg);
      DynamicJsonDocument  doc(2000);
      DeserializationError error = deserializeJson(doc, msg);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      JsonObject obj = doc.as<JsonObject>();

      const char* command = obj["command"];
      // Is there an error after all?
      if (command != nullptr) {

        if (strcmp(command, "restart")  == 0 && auth) {
          //reply
          Serial.println("restart");
          delay(100);
          send_state(true, "restart");
          ESP.restart();
        }
        else if (strcmp(command, "auth")  == 0 ) {

          //reply
          Serial.print("Authing with ");
          Serial.println(passws);
          if (strcmp(obj["pass"], passws)  == 0 ) {
            Serial.println("AUth Successful");
            send_state(true, "auth");
            auth = true;
          } else {
            delay(100);
            Serial.println("AUth Fail");
            send_state(false, "auth");

          }

        }
        else if ((strcmp(command, "login")  == 0) && auth) {
          char pin[32];
          strcpy(pin, obj["pass"]);
          current_user_number = getUserNumber(pin);
          if (current_user_number != -1 ) {
            Serial.println("Login Successful");
            send_state(true, "login");
          } else {
            delay(100);
            Serial.println("Login Fail");
            send_state(false, "login");
          }

        }

        else if ((strcmp(command, "list")  == 0) && auth && (current_user_number >= 0)) {
          send_credential_list();
        }

        else if ((strcmp(command, "get_credential")  == 0) && auth && (current_user_number >= 0)) {
          char dir[32];
          strcpy(dir, obj["dir"]);
          read_user_credential(dir, pin);
          send_credential();
        }
        else if ((strcmp(command, "add_credential")  == 0) && auth) {
          char email[64];
          char username[64];
          char password[64];
          strcpy(email, obj["email"]);
          strcpy(username, obj["username"]);
          strcpy(password, obj["password"]);

          save_user_credential(website, password, email, username , pin);
          listStoredCredentials(SPIFFS, "/", 0, current_user_number);
          send_state(true, "add_credential");
          

        }

      }
    }
  }
}

void send_state(bool state, const char *msg) {
  DynamicJsonDocument  doc(600);
  if (state) {
    doc["state"] = "success" ;
  }
  else {
    doc["state"] = "failed" ;
  }
  doc["resultof"] = msg ;

  String buf((char *)0);
  buf.reserve(1 + measureJson(doc));

  serializeJson(doc, buf);
  ws.textAll(buf.c_str());


  return;
}

void send_credential_list() {
  DynamicJsonDocument  doc(700);
  doc["command"] = "list";
  JsonArray data = doc.createNestedArray("credentials");
  for (int i = 0; i < 100; i++) {
    data.add(list[i]);
  }

  String buf((char *)0);
  buf.reserve(1 + measureJson(doc));
  serializeJson(doc, buf);
  ws.textAll(buf.c_str());
  return;

}
void send_credential() {
  DynamicJsonDocument  doc(300);
  doc["command"] = "credential";
  doc["email"] = user_out.email;
  doc["password"] = user_out.password;
  String buf((char *)0);
  buf.reserve(1 + measureJson(doc));
  serializeJson(doc, buf);
  ws.textAll(buf.c_str());
  return;

}
