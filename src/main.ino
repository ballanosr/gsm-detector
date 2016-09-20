#include <ArduinoJson.h>

#define enable 13
#define mixer 12

bool active = false;
long timer;

void setup(){
  pinMode(enable, OUTPUT);
  pinMode(mixer, OUTPUT);
  Serial.begin(115200);
  Serial.println("Script Begins!");
}

void loop(void){
  if(check_filters() && !active){
    timer = millis();
    active = true;
    digitalWrite(enable,active);
    digitalWrite(mixer,active);
    send_command(true);
  }
  if(active && (millis()-timer >= 10000) && !check_filters()){
    active = false;
    digitalWrite(enable,active);
    digitalWrite(mixer,active);
    send_command(false);
  }
}

bool check_filters(){
  int filtro850_1 = analogRead(A0);
  int filtro850_2 = analogRead(A1);
  int filtro1900_1 = analogRead(A2);
  int filtro1900_2 = analogRead(A3);

  int check_850 = filtro850_1-filtro850_2;
  int check_1900 = (filtro1900_1*2)-(filtro1900_2/2);

  if( check_850 <= 35 && check_850 >= (-50)){
    return true;
  }
  else if(check_1900 <= 530 && check_1900 >= 486){
    return true;
  }
  else{
    return false;
  }
}

void send_command(bool inp){
  String out;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["command"] = "detector";
  root["active"] = inp;
  root.printTo(out);
  Serial.println(out);
}
