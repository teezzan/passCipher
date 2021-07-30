


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
    globalClient = client;

  } else if (type == WS_EVT_DISCONNECT) {

    Serial.println("Websocket client connection finished");
    globalClient = NULL;

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
