


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
